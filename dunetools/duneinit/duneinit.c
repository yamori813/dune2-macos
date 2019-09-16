// duneinit.c - load firmware
//
// Time-stamp: <07/01/04 01:30:09 keuchel@rainer-keuchel.de on mediadev>
// 

#ifdef LINUX
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

#define VERSION "1.1"

#define stricmp strcasecmp

extern char *dunedir;

char *progname;
int debug;
int verbose;

dunedev_t *bdev;
duneparams_t params;

//////////////////////////////////////////////////////////////////////

void
usage()
{
  fprintf(stderr, "DuneInit for Linux Version %s\n", VERSION);
  fprintf(stderr, "Usage: %s [-d <level> -v]\n", progname);
  exit(1);
}

int 
main(int argc, char **argv)
{
  int c;
  char *p;
  progname = argv[0];

  DuneInit();

  while((c = getopt(argc, argv, "dv")) != -1)
    {
      switch(c)
	{
	case 'd':
	  debug = atoi(optarg);
	  break;
	case 'v':
	  verbose = 1;
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

  DuneInit();
  DuneStartDebug(debug);

  if((bdev = DuneOpen()) == NULL)
    {
      fprintf(stderr, "DuneOpen failed\n");
      exit(1);
    }

  DuneInitParams(&params);

  params.rectype = DUNE_DVD;
  params.videostandard = DUNE_PAL;
  params.inputsource = DUNE_COMPOSITE;

  if(DuneInitializeDevice(bdev, &params) < 0)
    {
      fprintf(stderr, "Cannot init dune\n");
      exit(1);
    }

  DuneClose(bdev);

  return 0;
}
