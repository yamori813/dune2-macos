// dunefilter.c
//
// Time-stamp: <18/03/03 08:38:19 keuchel@rainer-keuchel.de>

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include "getopt.h"

char *progname;
FILE *infile;
FILE *outfile;

int debug;
int vbi_only;

int system_header_seen;
int prev_code;
unsigned char lastpack[512];
int lastpacklen;

#define INLINE __inline

INLINE int
getbyte()
{
  unsigned int res;

  res = getc(infile);

  return res;
}

INLINE int
putbyte(int c)
{
  unsigned int res;

  res = putc(c, outfile);

  return res;
}

INLINE int
getword()
{
  unsigned int res;

  res = getc(infile) << 8;
  res |= getc(infile);

  return res;
}

INLINE int
putword(unsigned int w)
{
  unsigned int res;

  res = putc((w >> 8) & 0xFF, outfile);
  res = putc((w) & 0xFF, outfile);

  return res;
}

INLINE int
readbytes(unsigned char *buf, int len)
{
  int i;
  int c;

  for(i = 0; i < len; i++)
    {
      if((c = getbyte()) == EOF)
	return -1;

      buf[i] = c;
    }
  return 0;
}

INLINE int
writebytes(unsigned char *buf, int len)
{
  int i;

  for(i = 0; i < len; i++)
    putbyte(buf[i]);
  return 0;
}

INLINE void
add_bytes(int n)
{
  int i;

  for(i = 0; i < n; i++)
    putbyte(0xff);
}

INLINE int
copy_bytes(int len)
{
  int i;
  int c;

  for(i = 0; i < len; i++)
    {
      if((c = getc(infile)) == EOF)
	return -1;

      putc(c, outfile);
    }

  return 0;
}

INLINE int
skip_bytes(int len)
{
  int i;
  int c;

  for(i = 0; i < len; i++)
    {
      if((c = getc(infile)) == EOF)
	return -1;
    }

  return 0;
}

void
copy_video()
{
  int len;

  len = getword();

  if(debug)
    fprintf(stderr, "COPY VIDEO: Len %d\n", len);

  putbyte(0x00);
  putbyte(0x00);
  putbyte(0x01);
  putbyte(0xe0);
  putword(len);
  copy_bytes(len);
}

void
skip_video()
{
  int len;

  len = getword();

  if(debug)
    fprintf(stderr, "SKIP VIDEO: Len %d\n", len);

  skip_bytes(len);
}

void
copy_audio()
{
  int len;

  len = getword();

  if(debug)
    fprintf(stderr, "COPY AUDIO: Len %d\n", len);

  putbyte(0x00);
  putbyte(0x00);
  putbyte(0x01);
  putbyte(0xc0);
  putword(len);
  copy_bytes(len);
}

void
skip_audio()
{
  int len;

  len = getword();

  if(debug)
    fprintf(stderr, "SKIP AUDIO: Len %d\n", len);

  skip_bytes(len);
}

void
skip_vbi()
{
  int len;

  len = getword();

  if(debug)
    fprintf(stderr, "SKIP VBI: Len %d\n", len);

  skip_bytes(len);
}

void
copy_vbi()
{
  int len;

  len = getword();

  if(debug)
    fprintf(stderr, "COPY VBI: Len %d\n", len);

  // we only want the raw bits...
  copy_bytes(len);
}

void
copy_system_header()
{
  int len;

  len = getword();

  if(debug)
    fprintf(stderr, "SYSTEM_HEADER: Len %d\n", len);

  putbyte(0x00);
  putbyte(0x00);
  putbyte(0x01);
  putbyte(0xbb);
  putword(len);
  copy_bytes(len);
}

void
skip_system_header()
{
  int len;

  len = getword();

  if(debug)
    fprintf(stderr, "SYSTEM_HEADER: Len %d\n", len);

  skip_bytes(len);
}

void
copy_pack()
{
  int i, n;
  int len = 10;
  int stuff;

  putbyte(0x00);
  putbyte(0x00);
  putbyte(0x01);
  putbyte(0xba);

  if(debug)
    fprintf(stderr, "PACK\n");

  for(i = 0; i < len; i++)
    {
      lastpack[i] = getbyte();
      putbyte(lastpack[i]);
    }

  stuff = lastpack[9] & 0x7;

  if(debug)
    fprintf(stderr, "STUFFING: %d\n", stuff);

  for(n = 0; n < stuff; n++, i++)
    {
      lastpack[i] = getbyte();
      putbyte(lastpack[i]);
    }

  lastpacklen = 8 + stuff;
}

void
skip_pack()
{
  int i, n;
  int len = 8;
  int stuff;

  for(i = 0; i < len; i++)
    {
      lastpack[i] = getbyte();
    }

  stuff = lastpack[7] & 0x7;

  for(n = 0; n < stuff; n++, i++)
    {
      lastpack[i] = getbyte();
    }

  lastpacklen = 8 + stuff;
}

void
add_end()
{
  putbyte(0x00);
  putbyte(0x00);
  putbyte(0x01);
  putbyte(0xb9);
}

int
next_start_code()
{
  unsigned char buf[4];
  int n;
  int code;
  long off;
  long oldoff;

 again:
  while(1)
    {
      if(feof(infile))
	return EOF;

      if((buf[0] = getc(infile)) == 0)
	{
	  if((buf[1] = getc(infile)) == 0)
	    {
	      if((buf[2] = getc(infile)) == 1)
		{
		  buf[3] = getc(infile);
		  break;
		}
	    }
	}

      if(feof(infile))
	return EOF;
    }

  code = 0x100 | buf[3];

  off = ftell(infile) - 4;

  if(debug)
    fprintf(stderr, "%08d: %x\n", off, code);

  if(code == 0x1e0)
    {
      if(vbi_only)
	skip_video();
      else
	copy_video();
    }
  else if(code == 0x1c0)
    {
      if(vbi_only)
	skip_audio();
      else
	copy_audio();
    }
  else if(code == 0x1ba)
    {
      if(vbi_only)
	skip_pack();
      else
	copy_pack();
    }
  else if(code == 0x1bb)
    {
      if(vbi_only)
	skip_system_header();
      else
	copy_system_header();
    }
  else if(code == 0x1d1)
    {
      if(vbi_only)
	copy_vbi();
      else
	skip_vbi();
    }
  else
    {
      // on linux, stream might start with a slice...
      fprintf(stderr, "Unexpected code: 0x%x\n", code);
      //exit(1);
      goto again;
    }

  prev_code = code;

  return 0;
}

void
usage()
{
  fprintf(stderr, "Usage: %s [-d] [-v][-s]\n", progname);
  exit(1);
}

int
main(int argc, char **argv)
{
  int c;

  progname = argv[0];

  while((c = getopt(argc, argv, "dsv")) != -1)
    {
      switch(c)
	{
	case 'd':
	  debug = 1;
	  break;
	case 'v':
	  vbi_only = 1;
	  break;
	default:
	  usage();
	  break;
	}
    }

  infile = stdin;
  outfile = stdout;

#ifdef WIN32
  _setmode(0, O_BINARY);
  _setmode(1, O_BINARY);
#endif

  while(next_start_code() == 0);

  add_end();

  return 0;
}
