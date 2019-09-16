// dunec.c - duned client
//
// Time-stamp: <22/02/03 12:05:26 keuchel@rainer-keuchel.de>

#define VERSION "dunec 1.0"

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifdef LINUX
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <stdarg.h>

#define ws_fdopen fdopen
#define ws_close  close
#define ws_fclose fclose
#define ws_fgets  fgets
#define ws_write  write
#define ws_read   read
#endif

#ifdef _WIN32
#include <winsock.h>
#include <sockio2.h>
#endif

char *fname;

#define DEFAULTPORT 2124

char *command;
char *hostname;
short port;
char *progname;

void
xfatal(const char *fmt, ...)
{
  va_list ap;
  char buf[1024];

  va_start(ap, fmt);
  vsprintf(buf, fmt, ap);
  fprintf(stderr, "ERROR: %s", buf);
  fflush(stdout);
  exit(1);
}

void
usage()
{
  fprintf(stderr, 
          "%s\n", VERSION);
  fprintf(stderr, 
          "Usage: %s -h <hostname> [-p <port>] -c <command> [-f file]\n",
	  progname);

  exit(1);
}

int
main(int argc, char **argv)
{
  char *hostname = NULL;
  short port;
  struct sockaddr_in sin;
  int s;
  struct hostent *hp;
  char c;
  int one = 1;
  int zero = 0;
  int cc;
  char buf[1024];
  int ch;
  FILE *fin;

  progname = argv[0];

  port = DEFAULTPORT;

  if(argc == 1)
    usage();

  while((ch = getopt(argc, argv, "h:p:c:f:v")) != EOF)
    {
      switch(ch)
	{
	case 'f':
	  fname = optarg;
	  break;
	case 'h':
	  hostname = optarg;
	  break;
	case 'p':
	  port = atoi(optarg);
	  break;
	case 'c':
	  command = optarg;
	  break;
        case 'v':
          fprintf(stdout, "%s\n", VERSION);
          exit(0);
	default:
	  usage();
	  break;
	}
    }

  if(hostname == NULL || command == NULL)
    usage();

#ifdef _WIN32
  wsstart();
#endif
  
  if((hp = gethostbyname(hostname)) == NULL &&
     (hp = gethostbyaddr(hostname, strlen(hostname), AF_INET)) == NULL)
    {
      perror("gethostbyname");
      exit(1);
    }

  bzero(&sin, sizeof(sin));

  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  memcpy(&sin.sin_addr.s_addr, hp->h_addr, hp->h_length);

  if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      perror("socket");
      exit(1);
    }

  if(connect(s, (struct sockaddr *) &sin, sizeof(sin)) < 0)
    {
      perror("connect");
      exit(1);
    }

  if(ws_write(s, command, strlen(command)) != strlen(command))
    {
      perror("write");
      exit(1);
    }
  if(ws_write(s, "\n", 1) != 1)
    {
      perror("write");
      exit(1);
    }

  if(strcmp(command, "getstream") == 0)
    {
      int len;
      unsigned char vbuf[1024];

#ifdef WIN32
      _setmode(1, O_BINARY);
#endif

      while(1)
	{
	  if((len = ws_read(s, vbuf, sizeof(vbuf))) <= 0)
	    {
	      perror("read");
	      break;
	    }

	  if(write(1, vbuf, len) != len)
	    {
	      perror("write");
	      break;
	    }
	}   
    }
  else if(strcmp(command, "shutdown") == 0)
    {
      printf("Shutdown started\n");
    }
  else
    {
      if((fin = ws_fdopen(s, "r")) == NULL)
	{
	  perror("fdopen");
	  exit(1);
	}

      if(ws_fgets(buf, sizeof(buf), fin) == NULL)
	{
	  perror("fgets");
	}
      fputs(buf, stdout);

      ws_fclose(fin);
    }

  ws_close(s);

  shutdown(s, 1);

  return 0;
}
