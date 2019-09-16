// server.c
//
// Time-stamp: <25/02/03 16:37:09 keuchel@rainer-keuchel.de on mediadev>

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#ifndef SCO_CC
#include <getopt.h>
#endif
#include <netdb.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/signal.h>
#include <sys/socket.h>

#ifdef SCO
#undef MIN
#undef MAX
#endif

#include <sys/time.h>
#include <sys/resource.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

#ifdef SCO_CC
#define __SCO_WAIT3__
#endif

#ifndef _WIN32
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#endif

#include "duned.h"
#include "server.h"
#include "version.h"

#ifdef _WIN32
#include <sockio2.h>
#undef TEXT
#undef DELETE
#else
#define ws_read     read
#define ws_write    write
#define ws_fdopen   fdopen
#define ws_fclose   fclose
#define ws_fprintf  fprintf
#define ws_vfprintf vfprintf
#define ws_fflush   fflush
#define ws_close    close
#define ws_setvbuf  setvbuf
#define ws_fgets    fgets
#endif

#ifndef _WIN32
#include <sys/shm.h>
#endif

#ifdef SCO
#define WAIT_ANY (-1)
#endif

FILE *netin;
FILE *netout;
int netsock;
int listensock;
int debug;
char *progname;
pid_t serverpid;

extern char *dunedir;

/* cleanup handler for exit() */
static void
cleanup(void)
{
#ifndef _WIN32
  if(getpid() == serverpid)
    {
      remove(PIDNAME);
    }
#endif
}

char *
server_getline(char *buf, size_t size)
{
  char *p;

  if(ws_fgets(buf, size, netin) == NULL)
    {
      syslog(LOG_ERR, "fgets: %m");
      return NULL;
    }

  if((p = strrchr(buf, '\n')) != NULL)
    *p = 0;

  if(debug)
    server_dprintf("getline: %s\n", buf);

  return buf;
}

#ifndef _WIN32
void
sigpipehandler(int sig)
{
  syslog(LOG_ERR, "SIGPIPE");
}

void
sighuphandler(int sig)
{
  syslog(LOG_ERR, "SIGHUP");
}

void
sigtermhandler(int sig)
{
  syslog(LOG_ERR, "Server shutting down on SIGTERM...");
  exit(0);
}

#endif /* !WIN32 */

void
server_dprintf(const char *fmt, ...)
{
  FILE *f;
  va_list ap;

  if(!debug)
    return;

  va_start(ap, fmt);
  if((f = fopen(LOGNAME, "a")) != NULL)
    {
      vfprintf(f, fmt, ap);
      fclose(f);
    }
  va_end(ap);
}

int
server_reply(const char *fmt, ...)
{
  va_list ap;
  int len;

  va_start(ap, fmt);
  len = ws_vfprintf(netout, fmt, ap);
  va_end(ap);

  ws_fflush(netout); 

  return len;
}

#ifndef _WIN32
int
daemon_init()
{
  pid_t pid;

  if((pid = fork()) < 0)
    {
      syslog(LOG_ERR, "fork(): %m");
      return -1;
    }
  else if(pid != 0)
    {
      syslog(LOG_NOTICE, "parent pid %d exiting after fork", getpid());
      exit(0);
    } 

  /* become session leader */  
  if(setsid() < 0)
    syslog(LOG_ERR, "setsid: %m");

  umask(0);

  return 0;
}
#endif

void
server_handle_command(char *buf)
{
  if(strcmp(buf, "shutdown") == 0)
    {
      duned_shutdown();
      exit(0);
    }

  duned_handle_command(buf);
}

int
server_handle_connection()
{
  char buf[MAXCMDLEN];

  if((netin = ws_fdopen(netsock, "r")) == NULL)
    {
      syslog(LOG_ERR, "fdopen: %m");
      exit(1);
    }

  if((netout = ws_fdopen(netsock, "w")) == NULL)
    {
      syslog(LOG_ERR, "fdopen: %m");
      exit(1);
    }

#if 0
  if(ws_setvbuf(netout, NULL, _IOLBF, 1024) != 0)
    {
      syslog(LOG_ERR, "setvbuf: %m");
    }
#endif

#if 0
  if(duned_initsession() < 0)
    exit(1);
#endif

  if(server_getline(buf, sizeof(buf)))
    duned_handle_command(buf);

#if 0
  duned_cleanupsession();
#endif

  ws_fclose(netin);
  ws_fclose(netout);

  return 0;
}

void
usage()
{
  fprintf(stderr, "Usage: %s [-d <level> -p <port> -V]\n", progname);
  exit(1);
}

#ifdef _SERVICE 
#define main service_main
#endif

int
main(int argc, char **argv)
{
  char *p;
  struct sockaddr_in sin;
  char c;
  int one = 1;
  int zero = 0;
  int cc;
  int ch;
  int i;
  struct sockaddr_in from;
  int fromlen;
  int status;
  FILE *f;
  int fd;
  int port = DEFAULTPORT;

#ifndef _WIN32
  struct sigaction act, oact;
#endif

#ifdef _WIN32
  char path[256];
#endif

  progname = argv[0];

#ifdef _WIN32
  wsstart();

  /* Services are started in the %SYSTEM32% directory... */
  GetModuleFileName(NULL, path, sizeof(path));

  if((p = strrchr(path, '\\')) != NULL)
    *p = 0;

  if(SetCurrentDirectory(path) == FALSE)
    {
      syslog(LOG_ERR, "Cannot change directory to %s", path);
      return 1;
    }

#if 0
  GetCurrentDirectory(sizeof(path), path);
  syslog(LOG_NOTICE, "Server started in %s", path);
#endif
#endif // WIN32

  atexit(cleanup);

  while((ch = getopt(argc, argv, "fd:p:V")) != EOF)
    {
      switch(ch)
	{
	case 'f':
	  /* foreground flag */
	  break;
	case 'V':
	  printf("%s\n", VERSION_STRING);
	  exit(0);
	case 'd':
	  debug = atoi(optarg);
	  break;
	case 'p':
	  port = atoi(optarg);
	  break;
	default:
	  usage();
	}
    }

  openlog("duned", LOG_PID, LOG_DAEMON);

  if(duned_init() < 0)
    {
      exit(1);
    }

#ifndef _WIN32
  if(daemon_init() < 0)
    {
      fprintf(stderr, "%s\n", "daemon_init");
      exit(1);
    }

  if((f = fopen(PIDNAME, "w")) != NULL)
    {
      serverpid = getpid();

      fprintf(f, "%d\n", serverpid);
      fclose(f);
    }
#endif

#ifdef _WIN32
  fprintf(stderr, "%s started\n", VERSION_STRING);
#endif

#ifdef LINUX
  syslog(LOG_NOTICE, "%s started\n", VERSION_STRING);
#endif

#ifndef _WIN32
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
#ifdef SA_RESTART
  act.sa_flags |= SA_RESTART;
#endif

  act.sa_handler = sighuphandler; 
  sigaction(SIGHUP, &act, &oact);
  act.sa_handler = sigpipehandler; 
  sigaction(SIGPIPE, &act, &oact);
  act.sa_handler = sigtermhandler; 
  sigaction(SIGTERM, &act, &oact);

  if(!debug)
    {
      if(chdir("/") < 0)
	perror("chdir");
    }
#endif // !WIN32

  if((listensock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      fprintf(stderr, "socket: %s\n", strerror(errno));
      exit(1);
    }

  bzero(&sin, sizeof(sin));

  sin.sin_family = AF_INET;
  sin.sin_port = htons((short) port);
  sin.sin_addr.s_addr = htonl(INADDR_ANY);

  /* SCO takes a long time to remove listensockets when the
     process has crashed */
  if(bind(listensock, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
#ifdef _WIN32
      fprintf(stderr, "bind: %s\n", wsstrerror(WSAGetLastError()));
      syslog(LOG_ERR, "bind: %s\n", wsstrerror(WSAGetLastError()));
#else
      fprintf(stderr, "bind: %s\n", strerror(errno));
      syslog(LOG_ERR, "bind: %s\n", strerror(errno));
#endif
      exit(1);
    }

  close(0);

#ifdef _WIN32
#define NULLDEV "nul"
#else
#define NULLDEV "/dev/null"
#endif

  if((fd = open(NULLDEV, O_WRONLY)) < 0)
    {
      syslog(LOG_ERR, "open(%s): %m", NULLDEV);
      exit(1);
    }
  if(dup2(fd, 1) < 0)
    {
      syslog(LOG_ERR, "dup2: %m");
      exit(1);
    }
  if(dup2(fd, 2) < 0)
    {
      syslog(LOG_ERR, "dup2: %m");
      exit(1);
    }

  syslog(LOG_NOTICE, "Server started");

  if(listen(listensock, 5) < 0)
    {
      syslog(LOG_ERR, "listen: %m");
      exit(1);
    }

  while(1)
    {
      pid_t pid;
      char *p;

      fromlen = sizeof(from);

      if((netsock = accept(listensock, (struct sockaddr *)&from, 
			   &fromlen)) < 0)
	{
	  if(errno == EINTR)
	    continue;

	  syslog(LOG_ERR, "accept: %m");
	  exit(1);
	}

      syslog(LOG_NOTICE, "Connection from %s", 
	     inet_ntoa(from.sin_addr));

      server_handle_connection(); 

      ws_close(netsock);
    }

  /* never reached! */
  syslog(LOG_ERR, "Exiting");
  exit(1);
}

#ifdef _WIN32
#ifdef _SERVICE
void 
main_thread(void *arg)
{
  service_main(__argc, __argv);
}
#endif
#endif
