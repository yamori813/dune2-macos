// duned.c
//
// Time-stamp: <07/01/04 01:28:54 keuchel@rainer-keuchel.de on mediadev>

#ifdef LINUX
#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64
#include <features.h>
#endif

#ifdef WIN32
#include <windows.h>
#include <share.h>
#endif

#ifdef LINUX
#include <stdarg.h>
#include <pthread.h>
#endif

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>
#include <ctype.h>
#include <time.h>

#include "server.h"
#include "duned.h"
#include "dunelib.h"

#ifdef LINUX
#define stricmp strcasecmp
#endif

// 4096 is max read/write in libusb and devio.c has also a limit of
// PAGE_SIZE, which is 4096 (1 << 12).  I tried async submit urb which
// has a buffer of 0x4000, it's a bit faster and it's possible to read
// the next data while the other chunk gets transmitted.

// Seems that a kernel driver can call usb_bulk_msg() directly without
// this limit... Also copy_from/to_user is not needed.

#define LINUX_ASYNC

#ifdef LINUX_ASYNC
#define VBUFSIZE    0x4000
#define FILEBUFSIZE 0x4000
#else
#define VBUFSIZE    0x1000
#define FILEBUFSIZE 0x1000
#endif

#define MEG (1024*1024)

dunedev_t *bdev;
duneparams_t params;

int recording;
int stop_recording;

int playing;
int stop_playing;
int pause_playing;
int play_loop;
int play_slow;

uint64 play_skip;
uint64 play_offset;
uint64 repeat_start;
uint64 repeat_end;

int rec_time;
uint64 rec_size;

int ir_running;
int stop_ir;

char rec_filename[256];
char play_filename[256];

#ifdef WIN32
HANDLE rec_thread_handle;
DWORD rec_thread_id;
HANDLE play_thread_handle;
DWORD play_thread_id;
HANDLE ir_thread_handle;
DWORD ir_thread_id;
#endif

#ifdef LINUX
pthread_t rec_thread_handle;
pthread_t play_thread_handle;
pthread_t ir_thread_handle;
#endif

#ifdef LINUX
#define THREADRET void *
void *rec_thread_func(void *);
void *play_thread_func(void *);
void *ir_thread_func(void *);
#endif

#ifdef WIN32
#define THREADRET int
int rec_thread_func();
int play_thread_func();
int ir_thread_func();
#endif

void ok_reply(const char *fmt, ...);
void error_reply(const char *fmt, ...);
void error(const char *fmt, ...);

char *program_name;

static int
msleep(int millis)
{
#ifdef WIN32
  Sleep(millis);
#else
  struct timeval tv;

  tv.tv_sec = 0;
  tv.tv_usec = millis*1000;

  select(0, NULL, NULL, NULL, &tv);
#endif
  return 0;
}

int
duned_init()
{
  DuneInit();

  if((bdev = DuneOpen()) == NULL)
    {
      error("Cannot open dune");
      return -1;
    }

  DuneInitParams(&params);

  params.rectype = DUNE_DVD;
  params.videostandard = DUNE_PAL;
  params.inputsource = DUNE_COMPOSITE;

  return 0;
}

int
duned_shutdown()
{
  if(recording)
    cmd_stop_recording();
  if(playing)
    cmd_stop_playing();

  DuneClose(bdev);
  return 0;
}

int
duned_handle_command(char *s)
{
  char *cmd;
  char *tmp;

  tmp = strdup(s);
  cmd = strtok(tmp, " \t");

  if(stricmp(cmd, "startrec") == 0)
    {
      char *p = strtok(NULL, " \t");

      if(p == NULL)
	{
	  error_reply("Usage: startrec <fname>");
	  return -1;
	}

      strcpy(rec_filename, p);
      cmd_start_recording();
    }
  else if(stricmp(cmd, "stoprec") == 0)
    {
      cmd_stop_recording();
    }
  else if(stricmp(cmd, "startplay") == 0)
    {
      char *p = strtok(NULL, "");

      if(p == NULL)
	{
	  error_reply("Usage: startplay <fname>");
	  return -1;
	}

      strcpy(play_filename, s + (p - tmp));
      cmd_start_playing();
    }
  else if(stricmp(cmd, "stopplay") == 0)
    {
      cmd_stop_playing();
    }
  else if(stricmp(cmd, "pauseplay") == 0)
    {
      cmd_pause_playing();
    }
  else if(stricmp(cmd, "skip") == 0)
    {
      char *p = strtok(NULL, " \t");

      if(p == NULL)
	{
	  error_reply("Usage: skip <offset>");
	  return -1;
	}

      cmd_skip(p);
    }
  else if(stricmp(cmd, "repeat") == 0)
    {
      char *p = strtok(NULL, " \t");

      if(p == NULL)
	{
	  error_reply("Usage: repeat [start|end|clear]");
	  return -1;
	}

      cmd_repeat(p);
    }
  else if(stricmp(cmd, "setfreq") == 0)
    {
      char *p = strtok(NULL, " \t");

      if(p == NULL)
	{
	  error_reply("Usage: setfreq <freq|channelname>");
	  return -1;
	}
      cmd_setfreq(p);
    }
  else if(stricmp(cmd, "setinput") == 0)
    {
      char *p = strtok(NULL, " \t");

      if(p == NULL)
	{
	  error_reply("Usage: setinput <input>");
	  return -1;
	}
      cmd_setinput(p);
    }
  else if(stricmp(cmd, "status") == 0)
    {
      cmd_status();
    }
  else if(stricmp(cmd, "shutdown") == 0)
    {
      duned_shutdown();
      exit(0);
    }
  else if(stricmp(cmd, "getstream") == 0)
    {
      cmd_getstream();
    }
  else if(stricmp(cmd, "putstream") == 0)
    {
      cmd_putstream();
    }
  else if(stricmp(cmd, "startir") == 0)
    {
      cmd_start_ir();
    }
  else if(stricmp(cmd, "stopir") == 0)
    {
      cmd_stop_ir();
    }
  else if(stricmp(cmd, "loopon") == 0)
    {
      cmd_loop(1);
    }
  else if(stricmp(cmd, "loopoff") == 0)
    {
      cmd_loop(0);
    }
  else if(stricmp(cmd, "slowon") == 0)
    {
      cmd_slow(1);
    }
  else if(stricmp(cmd, "slowoff") == 0)
    {
      cmd_slow(0);
    }

  else if(stricmp(cmd, "setvideoformat") == 0)
    {
      char *format;
      char *standard;

      if((format = strtok(NULL, ",")) == NULL ||
	 (standard = strtok(NULL, ",")) == NULL)
	{
	  error_reply("Usage: setvideoformat [svcd|dvd],[pal|ntsc|pal60]");
	  return -1;
	}

      cmd_setvideoformat(format, standard);
    }
#ifdef NOT_YET
  else if(stricmp(cmd, "setaudiodec") == 0)
    {
      char *mode;

      if((mode = strtok(NULL, ",")) == NULL)
	{
	  error_reply("Usage: setaudiodec [mpeg|lpcm]");
	  return -1;
	}

      cmd_setaudiodec(mode);
    }
#endif
  else
    {
      error_reply("Unhandled command");
      return -1;
    }

  free(tmp);

  return 0;
}

#ifdef _COMMANDLINE
void
ok_reply(const char *fmt, ...)
{
  va_list ap;
  int len;

  fprintf(stdout, "OK: ");
  va_start(ap, fmt);
  len = vfprintf(stdout, fmt, ap);
  va_end(ap);
  fprintf(stdout, "\n");
}

void
error_reply(const char *fmt, ...)
{
  va_list ap;
  int len;

  fprintf(stdout, "ERROR: ");
  va_start(ap, fmt);
  len = vfprintf(stdout, fmt, ap);
  va_end(ap);
  fprintf(stdout, "\n");
}

void
error(const char *fmt, ...)
{
  va_list ap;
  int len = 0;
  char buf[512];

  va_start(ap, fmt);
  len += sprintf(buf+len, "ERROR: ");
  len += vsprintf(buf+len, fmt, ap);
  len += sprintf(buf+len, "\n");
  va_end(ap);

#ifdef _SERVER
  sylog(LOG_ERR, "%s", buf);
#else
  fprintf(stderr, "%s", buf);
#endif
}

#endif

#ifdef _SERVER
void
ok_reply(const char *fmt, ...)
{
  va_list ap;
  int len = 0;
  char buf[512];

  va_start(ap, fmt);
  len += sprintf(buf+len, "OK: ");
  len += vsprintf(buf+len, fmt, ap);
  len += sprintf(buf+len, "\n");
  va_end(ap);

  server_reply("%s", buf);
}

void
error_reply(const char *fmt, ...)
{
  va_list ap;
  int len = 0;
  char buf[512];

  va_start(ap, fmt);
  len += sprintf(buf+len, "ERROR: ");
  len += vsprintf(buf+len, fmt, ap);
  len += sprintf(buf+len, "\n");
  va_end(ap);

  server_reply("%s", buf);
}

void
error(const char *fmt, ...)
{
  va_list ap;
  int len = 0;
  char buf[512];

  va_start(ap, fmt);
  len += sprintf(buf+len, "ERROR: ");
  len += vsprintf(buf+len, fmt, ap);
  len += sprintf(buf+len, "\n");
  va_end(ap);

  syslog(LOG_ERR, "%s", buf);
}

#endif

#ifdef _COMMANDLINE
int
main(int argc, char **argv)
{
  char buf[1024];
  char *p;

  program_name = argv[0];

  if(duned_init() < 0)
    exit(1);

  while(1)
    {
      printf("> ");
      fflush(stdout);

      if(fgets(buf, sizeof(buf), stdin) == NULL)
	break;
      if((p = strrchr(buf, '\n')) != NULL)
	*p = 0;

      if(strcmp(buf, "quit") == 0)
	break;
      if(strcmp(buf, "exit") == 0)
	break;
      if(strcmp(buf, "bye") == 0)
	break;

      duned_handle_command(buf);
    }

  duned_shutdown();

  return 0;
}
#endif

int
cmd_start_recording()
{
  uint32 saa7114;

  if(recording)
    {
      error_reply("Recorder is already active.");
      return -1;
    }

  if(DuneInitializeDevice(bdev, &params) < 0)
    {
      error_reply("Cannot init device");
      return -1;
    }

  saa7114 = SAA7114_GetStatus(bdev);

  if((saa7114 & 0x01) == 0)
    {
      error_reply("Video ADC not ready (%x). Probably no input signal.",
		  saa7114);
      return -1;
    }

  stop_recording = 0;

#ifdef WIN32
  if((rec_thread_handle = CreateThread(NULL, 0, 
				       (LPTHREAD_START_ROUTINE) rec_thread_func, 
				       NULL, 0, &rec_thread_id)) == NULL)
    {
      error_reply("Failed to create thread: %d", GetLastError());
      return -1;
    }
#endif

#ifdef LINUX
  if(pthread_create(&rec_thread_handle, NULL, rec_thread_func, NULL) < 0)
    {
      error_reply("Failed to create thread: %d", errno);
      return -1;
    }
#endif

  ok_reply("Recorder started");

  return 0;
}

int
cmd_start_playing()
{
#ifdef NOT_YET
  if(access(play_filename, 0) != 0)
    {
      error_reply("%s: %s", play_filename, strerror(errno));
      return -1;
    }

  // stop?
  if(playing)
    {
      error_reply("Player is already active.");
      return -1;
    }

  stop_playing = 0;
  pause_playing = 0;

#ifdef WIN32
  if((play_thread_handle = CreateThread(NULL, 0, 
					(LPTHREAD_START_ROUTINE) play_thread_func, 
					NULL, 0, &rec_thread_id)) == NULL)
    {
      error_reply("Failed to create thread: %d", GetLastError());
      return -1;
    }
#endif

#ifdef LINUX
  if(pthread_create(&play_thread_handle, NULL, play_thread_func, NULL) < 0)
    {
      error_reply("Failed to create thread: %d", errno);
      return -1;
    }
#endif

  ok_reply("Player started");
#else
  error_reply("Not implemented");
#endif

  return 0;
}

int
cmd_stop_recording()
{
  if(!recording)
    {
      error_reply("Recorder not active");
      return -1;
    }

  stop_recording = 1;
  
#ifdef WIN32
  WaitForSingleObject(rec_thread_handle, INFINITE);
  CloseHandle(rec_thread_handle);
#endif

#ifdef LINUX
  pthread_join(rec_thread_handle, NULL);
#endif

  ok_reply("Recorder stopped");

  return 0;
}

int
cmd_stop_playing()
{
  if(!playing)
    {
      error_reply("Player not active");
      return -1;
    }

  stop_playing = 1;

#ifdef WIN32
  WaitForSingleObject(play_thread_handle, INFINITE);
  CloseHandle(play_thread_handle);
#endif

#ifdef LINUX
  pthread_join(play_thread_handle, NULL);
#endif

  ok_reply("Player stopped");

  return 0;
}

int
cmd_pause_playing()
{
  if(!playing)
    {
      error_reply("Player not active");
      return -1;
    }

  pause_playing = !pause_playing;

  if(pause_playing)
    ok_reply("Player paused");
  else
    ok_reply("Player continued");

  return 0;
}

int
cmd_setfreq(const char *arg)
{
  uint32 freq;

  if(recording)
    {
      error_reply("Cannot change freq when recorder is active"); // ??
      return -1;
    }

  if(isdigit(arg[0]))
    freq = atol(arg);
  else
    freq = DuneGetChannelFreq(arg);

  if(freq == 0)
    {
      error_reply("Invalid freq");
      return -1;
    }

  if(DuneSetFrequency(bdev, freq) < 0)
    error_reply("Command failed");
  else
    ok_reply("Command finished");

  return 0;
}

int
cmd_setinput(const char *arg)
{
  if(recording)
    {
      error_reply("Cannot change input source when recorder is active"); // ??
      return -1;
    }

  params.inputsource = atoi(arg);

  return 0;
}

//////////////////////////////////////////////////////////////////////

#ifdef LINUX
void *rec_thread_func(void *arg)
#endif
#ifdef WIN32
int rec_thread_func()
#endif
{
  int i;
  int len;
  unsigned char vbuf[VBUFSIZE];
  char *filename = rec_filename;
  FILE *outfile;
  time_t tstart, tnow;
  int errcount = 0;

#ifdef WIN32
  if((outfile = _fsopen(filename, "wb", _SH_DENYWR)) == NULL)
#else
  if((outfile = fopen(filename, "wb")) == NULL)
#endif
    {
      error("%s: %s", strerror(errno));
      return (THREADRET) -1;
    }

  if(setvbuf(outfile, NULL, _IOFBF, FILEBUFSIZE) != 0)
    {
      error("setvbuf: %s", strerror(errno));
      return (THREADRET) -1;
    }

  DuneStartRecording(bdev);

  recording = 1;
  rec_size = 0;
  time(&tstart);

  while(!stop_recording)
    {
      if((len = DuneReadStream(bdev, vbuf, sizeof(vbuf))) < 0)
	{
	  error("ReadVideo failed");
	  if(errcount++ > 3)
	    break;
	  else
	    continue;
	}

      errcount = 0;

      if(fwrite(vbuf, 1, len, outfile) != len)
	{
	  error("fwrite: %s", strerror(errno));
	  break;
	}

      time(&tnow);
      rec_time = tnow - tstart;
      rec_size += len;
    }

  DuneStopRecording1(bdev);

#if defined(LINUX) || defined(PATCHED_EZUSB)
  while((len = DuneReadStream(bdev, vbuf, sizeof(vbuf))) >= 0)
    {
      fwrite(vbuf, 1, len, outfile);
    }
#endif

  DuneStopRecording2(bdev);

  fclose(outfile);

  recording = 0;

  return 0;
}

#ifdef NO_YET

#ifdef LINUX
void *play_thread_func(void *arg)
#endif
#ifdef WIN32
int play_thread_func()
#endif
{
  int i;
  int len;
  unsigned char vbuf[VBUFSIZE];
  FILE *infile;
  char *filename = play_filename;
  int retry = 0;
  uint32 nvid, naud;
  uint32 pcount = 0;
  time_t tstart, tend;
  int waitcount = 0;
  int ret;

  if((infile = fopen(filename, "rb")) == NULL)
    {
      error("%s: %s", filename, strerror(errno));
      return (THREADRET) -1;
    }

  if(setvbuf(infile, NULL, _IOFBF, FILEBUFSIZE) != 0)
    {
      error("setvbuf: %s", strerror(errno));
      return (THREADRET) -1;
    }

  DuneStartPlaying(bdev);

  playing = 1;

  time(&tstart);

 loop:
  while(!stop_playing)
    {
      if(pause_playing)
	{
	  sleep(1);
	  continue;
	}

      if(play_skip)
	{
#ifdef LINUX
	  fseeko(infile, play_skip, SEEK_CUR);
#else
	  fseek(infile, play_skip, SEEK_CUR);
#endif
	  play_skip = 0;
	}
      else if(repeat_end && play_offset > repeat_end)
	{
#ifdef LINUX
	  fseeko(infile, repeat_start, SEEK_SET);
#else
	  fseek(infile, repeat_start, SEEK_SET);
#endif
	}

#ifdef LINUX
      play_offset = ftello64(infile);
#else
      play_offset = ftell(infile);
#endif

#if 1
      if((len = fread(vbuf, 1, sizeof(vbuf), infile)) <= 0)
#else
      // not faster
      if((len = read(fileno(infile), vbuf, sizeof(vbuf))) <= 0)
#endif
	{
	  if(len < 0)
	    error("fread");
	  break;
	}

      // delay loop

#if 1
      // disabling this gives 40 kb/sec...
      if((pcount % 20) == 0)
	{
	  int i;

	  // this delays dvd playback too much, so we do it
	  // only after every nth packet...

	  // possible that nvid does not drop...
	  for(i = 0; i < 3; i++)
	    {
	      if(DuneGetMPEGDecoderStatus(bdev, &nvid, &naud) < 0)
		break;

	      //DuneDebug("NVID: 0x%x NAUD:0x%x: %d", nvid, naud, i);

	      // just a guess, maximum seen here is 0x62
	      if(nvid < 0x40)
		break;
	    }
	}
#endif

#ifdef LINUX_ASYNC
      if(pcount > 0)
	{
	again:
#if 1
	  if((ret = DuneWriteStreamAsyncWaitForCompletion(bdev)) < 0)
#else
	  // EPERM?
	  if((ret = DuneWriteStreamAsyncWaitForCompletionNonBlock(bdev)) < 0)
#endif
	    {
	      if(ret == -EAGAIN)
		{
		  waitcount++;
		  goto again;
		}

	      error("AsyncWait failed: %d", ret);
	      break;
	    }
	}

      if((ret = DuneWriteStreamAsyncNoWait(bdev, vbuf, len)) < 0)
#else
      if((ret = DuneWriteStream(bdev, vbuf, len)) < 0)
#endif
	{
	  error("WriteVideo failed: %d", ret);
	  break;
	}

      if(play_slow)
	msleep(100);

      pcount++;
    }

  if(play_loop && !stop_playing)
    {
      rewind(infile);
      goto loop;
    }


  time(&tend);

#ifdef LINUX_ASYNC
  if(pcount)
    ret = DuneWriteStreamAsyncWaitForCompletion(bdev);
#endif

  // wait until stream played, how can we detect this?
#if 0
  sleep(2);
#endif

#if 1
  while(1)
    {
      // nvid is usually never zero...
      if(DuneGetMPEGDecoderStatus(bdev, &nvid, &naud) < 0)
	break;
      if(naud == 0 || retry == 3)
	break;

      sleep(1);
      retry++;
    }
#endif

  DuneStopPlaying(bdev);

  fclose(infile);

  playing = 0;

  // 750 kb/s on my 333 MHz VCR... 64 byte bulk has max 1216 kb
  // 830 kb/s when using submiturb with 16k buffers, version 1
  // 840 kb/s when using submiturb with 16k buffers, version 2
  // 950 kb/s but still too slow for dvd with lpcm audio...

  // 0 waits
  DuneDebug("Played %u bytes in %u secs, %u bytes/sec, %d waits",
	      sizeof(vbuf) * pcount, (tend - tstart),
	      sizeof(vbuf) * pcount / (tend - tstart),
	      waitcount);

  return 0;
}

#endif // NOT_YET

int
cmd_status()
{
  // one line...
  ok_reply("Recorder: %d Player: %d PlayLoop: %d PlayOff: %Ld MB "
	   "RecTime: %d Min RecSize: %Ld MB", 
	   recording, playing, play_loop, play_offset / MEG,
	   rec_time/60, rec_size / MEG);
  return 0;
}

//////////////////////////////////////////////////////////////////////
// stream to/from socket

#ifdef _SERVER
extern FILE *netin;
extern FILE *netout;
#endif

int
cmd_getstream()
{
  int i;
  int len;
  unsigned char vbuf[VBUFSIZE];
  int lasterrno = 0;
  int n = 0;

#ifdef _SERVER
  FILE *outfile = netout;
#else
  FILE *outfile = stdout;
#endif

  uint32 saa7114;

  if(recording)
    {
      error_reply("Recorder is already active.");
      return -1;
    }

  if(DuneInitializeDevice(bdev, &params) < 0)
    {
      error_reply("Cannot init device");
      return -1;
    }

  saa7114 = SAA7114_GetStatus(bdev);

  if((saa7114 & 0x01) == 0)
    {
      error_reply("Video ADC not ready. Probably no input signal.");
      return -1;
    }

  DuneStartRecording(bdev);

  while(1)
    {
      if((len = DuneReadStream(bdev, vbuf, sizeof(vbuf))) < 0)
	{
	  error("ReadVideo failed");
	  break;
	}

      if(fwrite(vbuf, 1, len, outfile) != len)
	{
	  lasterrno = errno;

	  error("fwrite: %s", strerror(errno));
	  break;
	}
    }

  syslog(LOG_NOTICE, "StopRecording1...");
  if(DuneStopRecording1(bdev) < 0)
    {
      error("StopRecording1 failed: %s", strerror(errno));
    }

#if defined(LINUX) || defined(PATCHED_EZUSB)
  syslog(LOG_NOTICE, "Reading end of stream...");

  n = 0;

  while((len = DuneReadStream(bdev, vbuf, sizeof(vbuf))) >= 0)
    {
      // inhibit SIGPIPE logging...
      if(lasterrno == 0)
	fwrite(vbuf, 1, len, outfile);

      n++;
    }
  syslog(LOG_NOTICE, "Read end of stream...%d buffers", n);
#endif

  syslog(LOG_NOTICE, "StopRecording2...");
  if(DuneStopRecording2(bdev) < 0)
    {
      error("StopRecording2 failed: %s", strerror(errno));
    }

  return 0;
}

int
cmd_putstream()
{
  int i;
  int len;
  unsigned char vbuf[VBUFSIZE];
  FILE *infile;
  int retry = 0;

#ifdef _SERVER
  infile = netin;
#else
  infile = stdin;
#endif

  DuneStartPlaying(bdev);

  while(1)
    {
      if((len = fread(vbuf, 1, sizeof(vbuf), infile)) <= 0)
	{
	  if(len < 0)
	    error("fread: %s", strerror(errno));
	  break;
	}

#if 0
      if(DuneAnnouncePlayPacket(bdev) < 0)
	{
	  error("Announce failed");
	  break;
	}
#endif
	
      if(DuneWriteStream(bdev, vbuf, len) < 0)
	{
	  error("WriteVideo failed");
	  break;
	}
    }

  // wait until stream played, how can we detect this?
#if 0
  sleep(5);
#endif

#if 1
  while(1)
    {
      uint32 nvid, naud;

      // nvid is usually never zero...
      if(DuneGetMPEGDecoderStatus(bdev, &nvid, &naud) < 0)
	break;
      if(naud == 0 || retry == 3)
	break;

      sleep(1);
      retry++;
    }
#endif

  DuneStopPlaying(bdev);

  return 0;
}

//////////////////////////////////////////////////////////////////////

int
cmd_start_ir()
{
#ifdef NOT_YET
  if(ir_running)
    {
      error_reply("IR thread already active.");
      return -1;
    }

  stop_ir = 0;

#ifdef WIN32
  if((ir_thread_handle = CreateThread(NULL, 0, 
				      (LPTHREAD_START_ROUTINE) ir_thread_func, 
				      NULL, 0, &ir_thread_id)) == NULL)
    {
      error_reply("Failed to create thread: %d", GetLastError());
      return -1;
    }
#endif

#ifdef LINUX
  if(pthread_create(&ir_thread_handle, NULL, ir_thread_func, NULL) < 0)
    {
      error_reply("Failed to create thread: %d", errno);
      return -1;
    }
#endif

  ok_reply("IR started");
#else
  error_reply("Not implemented");
#endif
  return 0;
}

#ifdef NOT_YET

#ifdef LINUX
void *ir_thread_func(void *arg)
#endif
#ifdef WIN32
int ir_thread_func()
#endif
{
  int len;
  unsigned char vbuf[64];
  unsigned char *p;
  unsigned char seq[2];
  int nseq = 0;
  int code;

  ir_running = 1;

  while(!stop_ir)
    {
      if((len = DuneReadRemoteControl(bdev, vbuf, sizeof(vbuf))) < 0)
	{
	  error("Read IR failed");
	  break;
	}

      if(!len)
	{
	  continue;
	}

      for(p = vbuf; p < vbuf + len; p++)
	{
	  if(*p == 0xFE)
	    {
	      nseq = 0;
	    }
	  else if(nseq == 0)
	    {
	      seq[nseq++] = *p;
	    }
	  else
	    {
	      seq[nseq++] = *p;

	      code = (seq[0] << 8) | seq[1];
	      duned_handleircode(code);
	      nseq = 0;
	    }
	}
    }

  ir_running = 0;

  return 0;
}

#endif // NOT_YET

int
cmd_loop(int on)
{
  play_loop = on;

  ok_reply("Loop set to %d", play_loop);
  return 0;
}

int
cmd_slow(int on)
{
  play_slow = on;

  ok_reply("Slow set to %d", play_slow);
  return 0;
}

int
cmd_skip(char *arg)
{
  int n = atoi(arg) * MEG;
  uint64 newoff = play_offset + n;

  play_skip = n;

  ok_reply("Skipping to %Ld MB", newoff / MEG);
  return 0;
}

int
cmd_repeat(char *arg)
{
  if(stricmp(arg, "start") == 0)
    {
      repeat_start = play_offset;
    }
  else if(stricmp(arg, "end") == 0)
    {
      repeat_end = play_offset;
    }
  else if(stricmp(arg, "clear") == 0)
    {
      repeat_start = repeat_end = 0;
    }
  else 
    {
      error_reply("Usage: repeat [start|end|clear]");
      return -1;
    }

  ok_reply("Start: %d End: %d", (int) (repeat_start/MEG), 
	   (int) (repeat_end/MEG));
  return 0;
}

int
cmd_setvideoformat(char *rectype, char *videostandard)
{
  if(recording || playing)
    {
      error_reply("Recorder or player is active");
      return -1;
    }

  if(stricmp(rectype, "dvd") == 0)
    params.rectype = DUNE_DVD;
  else if(stricmp(rectype, "dvdlong") == 0)
    params.rectype = DUNE_DVD_LONG;
  else if(stricmp(rectype, "svcd") == 0)
    params.rectype = DUNE_SVCD_SVCD;
  else if(stricmp(rectype, "svcd2") == 0)
    params.rectype = DUNE_SVCD_MPEG2;
  else if(stricmp(rectype, "vcd") == 0)
    params.rectype = DUNE_VCD_VCD;
  else if(stricmp(rectype, "vcd2") == 0)
    params.rectype = DUNE_VCD_MPEG1;
  else
    fprintf(stderr, "Invalid rectype\n");

  if(stricmp(videostandard, "pal") == 0)
    params.videostandard = DUNE_PAL;
  else if(stricmp(videostandard, "ntsc") == 0)
    params.videostandard = DUNE_NTSC;
  else
    fprintf(stderr, "Invalid videostandard\n");

  ok_reply("Videoformat set");
  return 0;
}

#ifdef NOT_YET
int
cmd_setaudiodec(char *mode)
{
  if(stricmp(mode, "lpcm") == 0)
    {
      // Either my machine or USB 1.1 is too slow for DVD with 48 kHz LPCM...
      // 8051 can DMA at 24 MHz (page 6-21)

      L64105_AudioLPCM(bdev);
      ok_reply("Audiodecoder mode set");
      return 0;
    }
  else if(stricmp(mode, "mpeg") == 0)
    {
      L64105_AudioMPEG(bdev);
      ok_reply("Audio decoder mode set");
      return 0;
    }

  error_reply("Invalid audio decoder mode");
  return -1;
}

#endif

int
cmd_stop_ir()
{
  if(!ir_running)
    {
      error_reply("IR not active");
      return -1;
    }

  stop_ir = 1;
  
#ifdef WIN32
  WaitForSingleObject(ir_thread_handle, INFINITE);
  CloseHandle(ir_thread_handle);
#endif

#ifdef LINUX
  pthread_join(ir_thread_handle, NULL);
#endif

  ok_reply("IR stopped");

  return 0;
}

int
duned_handleircode(uint32 code)
{
  char *ircodename;

  ircodename = DuneGetIRCodeName(code);

  return 0;
}

//////////////////////////////////////////////////////////////////////
