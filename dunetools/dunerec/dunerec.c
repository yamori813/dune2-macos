// dunerec.c - a dune recorder for Linux and Win32
//
// Time-stamp: <07/01/04 01:38:33 keuchel@rainer-keuchel.de on mediadev>

#ifdef LINUX
#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64
#include <features.h>
#endif

#if defined(LINUX) || defined(MACOS)
#include <unistd.h>
#endif

#ifdef WIN32
#include <windows.h>
#include "getopt.h"
#endif

#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#include "dunelib.h"

#define VERSION "1.0"

// max value in libusb?!
#define VBUFSIZE    4096

#ifndef WIN32
#define stricmp strcasecmp
#endif

extern char *dunedir;

char *progname;
int debug;
int verbose;

char *filename;

char *videostandard;
char *rectype;
char *inputsource;
char *vbiflag;

char *configfile;

int seconds;
int antennafreq;

// actions
int remotecontrol;
int readflag;
int writeflag;
int scanflag;
int infoflag;
long maxfilesize;

int bufsize = 0x4000;

dunedev_t *bdev;
duneparams_t params;

int abortflag;
FILE *outfile;
FILE *infile;

// timeout for bulk commands
int timeout = 3000;

#define MAXCHANNELS 126
static dune_channel_t scanchannels[MAXCHANNELS];
static int nscanchannels;

void
interrupt(int sig)
{
  abortflag = 1;
}

void
GenerateScanChannels()
{
  uint32 f;
  int n;

  nscanchannels = 0;

  if(scanflag & 0x02)
    {
      // Band I (VHF), Kanal 2-4
      for(f = 48250, n = 2; f <= 62250; f += 7000, n++)
	{
	  sprintf(scanchannels[nscanchannels].name, "%02d", n);
	  scanchannels[nscanchannels++].freq = f;
	}

      // USB S4-10
      for(f = 126250, n = 4; f <= 168250; f += 7000, n++)
	{
	  sprintf(scanchannels[nscanchannels].name, "S%02d", n);
	  scanchannels[nscanchannels].freq = f;
	  nscanchannels++;
	}

      // Band III (VHF) E5-12
      for(f = 175250, n = 4; f <= 224250; f += 7000, n++)
	{
	  sprintf(scanchannels[nscanchannels].name, "E%02d", n);
	  scanchannels[nscanchannels].freq = f;
	  nscanchannels++;
	}

      // OSB S11-20
      for(f = 231250, n = 11; f <= 294250; f += 7000, n++)
	{
	  sprintf(scanchannels[nscanchannels].name, "S%02d", n);
	  scanchannels[nscanchannels].freq = f;
	  nscanchannels++;
	}

      // ESB S21-38
      for(f = 303250, n = 21; f <= 439250; f += 8000, n++)
	{
	  sprintf(scanchannels[nscanchannels].name, "S%02d", n);
	  scanchannels[nscanchannels].freq = f;
	  nscanchannels++;
	}
    }

  if(scanflag & 0x01)
    {
      // Band IV und V UHF
      for(f = 471250, n = 21; f <= 855250; f += 8000, n++)
	{
	  sprintf(scanchannels[nscanchannels].name, "CH%02d", n);
	  scanchannels[nscanchannels].freq = f;
	  nscanchannels++;
	}
    }
}

void
DumpBuffer(char *desc, unsigned char *buf, int len)
{
  int i;

  fprintf(stderr, "%s: ", desc);
  for(i = 0; i < len; i++)
    fprintf(stderr, "%02x ", buf[i]);
  fprintf(stderr, "\n");
}

//////////////////////////////////////////////////////////////////////

int
RecordVideo()
{
  int len;
  unsigned char vbuf[VBUFSIZE];
  time_t starttime, endtime;

  char filename2[256];
  char *p;
  int nfiles = 1;

  int blocks = 0;
  int oldblocks = 0;
  int totallen = 0;
  int totallen2 = 0;

  int kval;

  uint32 saa7114 = SAA7114_GetStatus(bdev);

  if((saa7114 & 0x01) == 0)
    {
      fprintf(stderr, "Video ADC not ready. Probably no input signal\n");
      fprintf(stderr, "or wrong pal/ntsc setting.\n");
      return -1;
    }

  if(debug)
    fprintf(stderr, "Recording...\n");

  time(&starttime);
  endtime = starttime + seconds;

  if(maxfilesize != 0)
    {
      if((p = strrchr(filename, '.')) != NULL)
	*p = 0;
      sprintf(filename2, "%s_%02d.%s", filename, nfiles++, "mpg");

      if(verbose)
	fprintf(stderr, "Starting file %s\n", filename2);
    }
  else
    {
      strcpy(filename2, filename);
    }

  if(strcmp(filename2, "-") == 0)
    {
      outfile = stdout;
    }
  else if((outfile = fopen(filename2, "wb")) == NULL)
    {
      perror(filename2);
      return -1;
    }

  if(setvbuf(outfile, NULL, _IOFBF, bufsize) != 0)
    fprintf(stderr, "setvbuf: %s\n", strerror(errno));

  if(DuneStartRecording(bdev) < 0)
    {
      fclose(outfile);
      return -1;
    }

  signal(SIGINT, interrupt);
#ifdef SIGPIPE
  signal(SIGPIPE, interrupt);
#endif
  
  while(!abortflag)
    {
      if(seconds)
	{
	  time_t now;

	  time(&now);
	  if(now >= endtime)
	    break;
	}

      if((len = DuneReadStream(bdev, vbuf, sizeof(vbuf))) < 0)
	{
	  fprintf(stderr, "ReadVideo failed\n");
	  break;
	}

      if(fwrite(vbuf, 1, len, outfile) != len)
	{
	  perror("fwrite");
	  break;
	}

      totallen += len;
      totallen2 += len;

      // TODO: This is not a GOP cut...
      if(maxfilesize != 0 && totallen > maxfilesize)
	{
	  fclose(outfile);

	  if((p = strrchr(filename, '.')) != NULL)
	    *p = 0;

	  sprintf(filename2, "%s_%02d.%s", filename, nfiles++, "mpg");

	  if(verbose)
	    fprintf(stderr, "\nStarting file %s\n", filename2);

	  if((outfile = fopen(filename2, "wb")) == NULL)
	    {
	      perror(filename);
	      return -1;
	    }

	  if(setvbuf(outfile, NULL, _IOFBF, bufsize) != 0)
	    fprintf(stderr, "setvbuf: %s\n", strerror(errno));

	  totallen = 0;
	}
      
      blocks = totallen / 0x4000;

      if(blocks != oldblocks)
	{
	  if(verbose)
	    fprintf(stderr, ".");
	  oldblocks = blocks;
	}
    }

  if(verbose)
    fprintf(stderr, "\n");

  DuneStopRecording1(bdev);

  // hangs
  //DuneKFirReadCmd(bdev, 0xfe, &kval);
  //fprintf(stderr, "Status: 0x%x\n", kval);

  // We *must* read all bulk data, else the device hangs
  // during the next recording, but that's not possible
  // when using ezusb.sys. Is there a status we can check?
  // Checking reg 0xfe does not help...

  //DuneKFirReadCmd(bdev, 0xfe, &kval);
  //fprintf(stderr, "Status: 0x%x\n", kval);

#if defined(LINUX) || defined(PATCHED_EZUSB)
  while((len = DuneReadStream(bdev, vbuf, sizeof(vbuf))) >= 0)
    {
      fwrite(vbuf, 1, len, outfile);
    }
#endif

  DuneStopRecording2(bdev);

  if(strcmp(filename, "-") != 0)
    fclose(outfile);

  if(verbose)
    {
      int n;
      
      time(&endtime);
      n = endtime - starttime;
      if(n == 0)
	n = 1;

      fprintf(stderr, "Recorded %u bytes in %d secs, %d bytes/sec\n",
	      totallen2, n, totallen2/n);
    }

  return 0;
}

//////////////////////////////////////////////////////////////////////

int
ScanChannels()
{
  int i;
  uint32 saa7114;
  uint32 msp3400;
  uint8 buf[2];

  if(params.videostandard != DUNE_PAL)
    {
      // NTSC has 6 MHz channels
      fprintf(stderr, "Channel scanning for NTSC not supported yet\n");
      return -1;
    }

  signal(SIGINT, interrupt);

  GenerateScanChannels();

  for(i = 0; i < nscanchannels; i++)
    {
      if(abortflag)
	break;

      if(verbose)
	fprintf(stderr, "Trying %d\n", scanchannels[i].freq);

      DuneSetFrequency(bdev, scanchannels[i].freq);
      // wait for tuner to settle...
      sleep(1);

      saa7114 = SAA7114_GetStatus(bdev);
      msp3400 = MSP3400_GetStatus(bdev, buf);

      if(saa7114 & 0x01)
	{
	  if(msp3400 & 0x02)
	    printf("# No audio\n");
	  if((msp3400 >> 6) & 0x01)
	    printf("# Stereo\n");
	  else
	    printf("# Mono\n");

	  printf("%s\t%d\n", 
		 scanchannels[i].name,
		 scanchannels[i].freq);
	}
    }

  return 0;
}

//////////////////////////////////////////////////////////////////////

#if 0
int
PlayVideo()
{
  int len;
  unsigned char vbuf[VBUFSIZE];
  time_t starttime, endtime;

  int blocks = 0;
  int oldblocks = 0;
  int totallen = 0;
  int retry = 0;

  if(debug)
    fprintf(stderr, "Playing...\n");

  time(&starttime);
  endtime = starttime + seconds;

  if(strcmp(filename, "-") == 0)
    {
      infile = stdin;
    }
  else if((infile = fopen(filename, "rb")) == NULL)
    {
      perror(filename);
      return -1;
    }

  if(setvbuf(infile, NULL, _IOFBF, bufsize) != 0)
    fprintf(stderr, "setvbuf: %s\n", strerror(errno));

  DuneStartPlaying(bdev);

  signal(SIGINT, interrupt);
  
  while(!abortflag)
    {
      if(seconds)
	{
	  time_t now;

	  time(&now);
	  if(now >= endtime)
	    break;
	}

      if((len = fread(vbuf, 1, sizeof(vbuf), infile)) <= 0)
	{
	  if(len < 0)
	    perror("fread");
	  break;
	}

#if 0
      // this is really a check of the decoder channel queue...
      if(DuneAnnouncePlayPacket(bdev) < 0)
	{
	  fprintf(stderr, "Announce failed\n");
	  break;
	}
#endif

#if 0
      if(DuneWriteStreamAsync(bdev, vbuf, len) < 0)
#else
      if(DuneWriteStream(bdev, vbuf, len) < 0)
#endif
	{
	  fprintf(stderr, "WriteVideo failed\n");
	  break;
	}

      totallen += len;
      blocks = totallen / 0x4000;

      if(blocks != oldblocks)
	{
	  if(verbose)
	    fprintf(stderr, ".");
	  oldblocks = blocks;
	}
    }

  if(verbose)
    fprintf(stderr, "\n");

#if 0
  // wait until stream played, how can we detect this?
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

  if(strcmp(filename, "-") != 0)
    fclose(infile);

  if(verbose)
    {
      int n;
      
      time(&endtime);
      n = endtime - starttime;
      if(n == 0)
	n = 1;

      fprintf(stderr, "Played %u bytes in %d secs, %d bytes/sec\n",
	      totallen, n, totallen/n);
    }

  return 0;
}
#endif

//////////////////////////////////////////////////////////////////////

#if 1
int
ShowRemoteControl()
{
  int len;
  unsigned char vbuf[64];
  unsigned char *p;
  unsigned char seq[2];
  int nseq = 0;
  int code;

  signal(SIGINT, interrupt);

  while(!abortflag)
    {
      if((len = DuneReadRemoteControl(bdev, vbuf, sizeof(vbuf))) < 0)
	{
	  if(errno == ETIMEDOUT)
	    {
	      sleep(1);
	      continue;
	    }

	  fprintf(stderr, "Read failed\n");
	  break;
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
	      fprintf(stderr, "%s\n", DuneGetIRCodeName(code));

	      nseq = 0;
	    }
	}
    }

  return 0;
}
#endif

//////////////////////////////////////////////////////////////////////

int
ShowChipInfo()
{
  DuneShowChipInfo(bdev, infoflag);
  return 0;
}

//////////////////////////////////////////////////////////////////////

void
usage()
{
  fprintf(stderr, "DuneRec Version %s\n", VERSION);
  fprintf(stderr, "Usage: %s [-d <level> -v]\n", progname);
  fprintf(stderr, " -R <outfile|-> | -S <flags> | -I <flags>\n");
  fprintf(stderr, " [-a <freq>\n");
  fprintf(stderr, " [-c <configfile>]\n");
  fprintf(stderr, " [-i <inputsource>]\n");
  fprintf(stderr, " [-s <seconds>]\n");
  fprintf(stderr, " [-m <maxfilesize>\n");
  fprintf(stderr, " [-t <dvd|dvdlong|dvd5mbit|svcd|vcd|svcd2|vcd2>]\n");
  fprintf(stderr, " [-x <pal|ntsc>]\n");
  fprintf(stderr, " [-z]\n");
  exit(1);
}

int 
main(int argc, char **argv)
{
  int c;
  
  progname = argv[0];

  DuneInit();

  while((c = getopt(argc, argv, 
		    "z:I:vhuS:T:d:i:R:P:t:s:a:k:x:c:z:y:b:r:m:C")) != -1)
    {
      switch(c)
	{
	case 'z':
	  vbiflag = optarg;
	  break;
	case 'R':
	  filename = optarg;
	  readflag = 1;
	  break;
#if 0
	case 'P':
	  filename = optarg;
	  writeflag = 1;
	  break;
#endif
	case 'S':
	  if(sscanf(optarg, "%x", &scanflag) != 1 ||
	     scanflag == 0)
	    usage();
	  break;
	case 'I':
	  infoflag = atoi(optarg);
	  break;
	case 'C':
	  remotecontrol = 1;
	  break;
	case 'd':
	  debug = atoi(optarg);
	  break;
	case 'v':
	  verbose = 1;
	  break;
	case 't':
	  rectype = optarg;
	  break;
	case 'x':
	  videostandard = optarg;
	  break;
	case 'a':
	  if(!isdigit(optarg[0]))
	    antennafreq = DuneGetChannelFreq(optarg);
	  else
	    antennafreq = atoi(optarg);
	  break;
	case 's':
	  seconds = atoi(optarg);
	  break;
	case 'i':
	  inputsource = optarg;
	  break;
	case 'c':
	  configfile = optarg;
	  break;
	case 'm':
	  maxfilesize = atol(optarg);
	  break;
	case 'h':
	  usage();
	  break;
	default:
	  fprintf(stderr, "Illegal option -%c\n", optopt);
	  usage();
	  break;
	}
    }

  if(seconds && seconds < 0)
    usage();

  DuneStartDebug(debug);
  DuneSetVerbose(verbose);

  if(readflag && writeflag)
    {
      fprintf(stderr, "Simultaneous read/write not possible.\n");
      exit(1);
    }

  if(!readflag && !writeflag && !remotecontrol && !scanflag && !infoflag)
    {
      usage();
    }

  if((bdev = DuneOpen()) == NULL)
    {
      fprintf(stderr, "DuneOpen failed\n");
      exit(1);
    }

  DuneInitParams(&params);

  if(configfile)
    {
      if(DuneReadConfig(configfile, &params) < 0)
	exit(1);
    }

  // override config file
  if(rectype)
    {
      if(stricmp(rectype, "dvd") == 0)
	params.rectype = DUNE_DVD;
      else if(stricmp(rectype, "dvdlong") == 0)
	params.rectype = DUNE_DVD_LONG;
      else if(stricmp(rectype, "dvd5mbit") == 0)
	params.rectype = DUNE_DVD_5MBIT;
      else if(stricmp(rectype, "svcd") == 0)
	params.rectype = DUNE_SVCD_SVCD;
      else if(stricmp(rectype, "svcd2") == 0)
	params.rectype = DUNE_SVCD_MPEG2;
      else if(stricmp(rectype, "vcd") == 0)
	params.rectype = DUNE_VCD_VCD;
      else if(stricmp(rectype, "vcd2") == 0)
	params.rectype = DUNE_VCD_MPEG1;
      else
	usage();
    }
  if(videostandard)
    {
      if(stricmp(videostandard, "pal") == 0)
	params.videostandard = DUNE_PAL;
      else if(stricmp(videostandard, "ntsc") == 0)
	params.videostandard = DUNE_NTSC;
      else
	usage();
    }
  if(inputsource)
    {
      if(isdigit(inputsource[0]))
	params.inputsource = atoi(inputsource);
      else if(stricmp(inputsource, "tuner") == 0)
	params.inputsource = DUNE_TUNER;
      else if(stricmp(inputsource, "svideo") == 0)
	params.inputsource = DUNE_SVIDEO;
      else if(stricmp(inputsource, "composite") == 0)
	params.inputsource = DUNE_COMPOSITE;
      else
	usage();
    }
  if(vbiflag)
    {
      params.teletext = atoi(vbiflag);
    }

  if(readflag && params.inputsource == DUNE_TUNER && antennafreq == 0)
    {
      fprintf(stderr, "WARNING: No frequency set!\n");
    }

  // load firmware etc
  if(DuneInitializeDevice(bdev, &params) < 0)
    exit(1);

  if(antennafreq)
    {
      DuneSetFrequency(bdev, antennafreq);
      sleep(1);
    }

#if 0
  DuneDumpFirmware(bdev, 0x0000, 0x1FFF, "/tmp/dune.fw1");
  // seems that code memory cannot be read...
  DuneDumpFirmware(bdev, 0x2000, 0xFFFF, "/tmp/dune.fw2");

  DuneClose(bdev);
  exit(1);
#endif

  // actions
  if(readflag)
    RecordVideo();
  else if(scanflag)
    ScanChannels();
  else if(infoflag)
    ShowChipInfo();

#if 1
  else if(remotecontrol)
    ShowRemoteControl();
#endif

#if 0
  else if(writeflag)
    PlayVideo();
#endif

  DuneClose(bdev);

  return 0;
}
