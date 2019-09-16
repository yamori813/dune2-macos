// Time-stamp: <23/03/03 15:44:10 keuchel@rainer-keuchel.de on mediadev>
//
// convert private stream data to teletext
//
// There is more data in the stream (66 bytes)

#ifdef WIN32
#include <io.h>
#include <fcntl.h>
#include <winsock.h>
#include "getopt.h"
#endif

#ifdef LINUX
#include <unistd.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <ctype.h>

#include "dunevtx.h"
#include "hamm.h"

int hexflag;
int showheader;
int debug;
int verbose;
int vpsflag;
int infoflag;
int preselflag;
int textflag;

double atof();

int curpage;

#define MIN(X,Y) (((X) < (Y)) ? (X) : (Y))

unsigned char *epgbuf;
unsigned char *epgp;
int epglen;

// ZVBI does not handle double height text?
// write in a format that can be used for zvbi/export
int zvbi;
double dt;
int nslice;

#define MAXSLICE 30 // ZVBI

int ale;

typedef unsigned char byte_t;

static byte_t parity_table[256];
static int init_done;

void
jul2greg(int jd, int *yp, int *mp, int *dp)
{
  int l, n, i, j;

  // modified julian date, 31.1.1982 = 45000
  // Greg date:   31.1.1982
  // Julian date: 2445001

  jd += 2400001;

  l = jd + 68569;
  n = ( 4 * l ) / 146097;
  l = l - ( 146097 * n + 3 ) / 4;
  i = ( 4000 * ( l + 1 ) ) / 1461001;
  l = l - ( 1461 * i ) / 4 + 31;
  j = ( 80 * l ) / 2447;
  *dp = l - ( 2447 * j ) / 80;
  l = j / 11;
  *mp = j + 2 - ( 12 * l );
  *yp = 100 * ( n - 49 ) + i + l;
}

static void
tools_init(void) {
  int pos, val, bit;

  /* Set up parity_table: If (parity_table[foo] & 0x80), */
  for (pos = 0; pos <= 255; pos++) {
    bit = 0; /* foo has odd number of bits set */
    val = pos;
    while (val) { /* Count number of set bits in val; see K&R, Exercise 2-9 */
      bit ^= 0x80;
      val &= val - 1;
    }
    parity_table[pos] = bit | 0x7f;
  }

  /* parity_table is also used for hamming decoding: 
   * If (parity_table[foo] & 0x40), foo has
   * more than one bit-error and can't be corrected; 
   * otherwise the correct(ed) value is
   * parity_table[foo] & 0xf
   */

  for (pos = 0; pos <= 15; pos++) {
    val = ( !(pos & 1) ^ !!(pos & 4) ^ !!(pos & 8)) << 0 | !!(pos & 1) << 1 |
          ( !(pos & 1) ^ !!(pos & 2) ^ !!(pos & 8)) << 2 | !!(pos & 2) << 3 |
          ( !(pos & 1) ^ !!(pos & 2) ^ !!(pos & 4)) << 4 | !!(pos & 4) << 5 |
          (!!(pos & 2) ^ !!(pos & 4) ^ !!(pos & 8)) << 6 | !!(pos & 8) << 7;
    for (bit = 0; bit <= 8; bit++) {
      parity_table[val ^ ((1 << bit) & 0xff)] &= (0x80 | pos);
    }
  }
  init_done = 1;
}

/* Check hamming-encoding of val, return decoded value if OK, -1 otherwise
 */
int
vtx_chkhamming(byte_t val) {
  if (!init_done)
    tools_init();
  if (parity_table[val] & 0x40) {
    return -1;
  } else {
    return parity_table[val] & 0xf;
  }
}

void
vtx_text(char *buf, int len)
{
  int i;

  // graphics is off for new row
  int graphics = 0;

  if(hexflag)
    {
      for(i = 0; i < len; i++)
	{
	  int c = buf[i] & 0x7F;
	  fprintf(stderr, "%02x ", c);
	}
      fprintf(stderr, "\n");
    }

  for(i = 0; i < len; i++)
    {
      int c = buf[i] & 0x7F;

      if(c >= 0x20 && c <= 0x7F)
	{
	  if(graphics)
	    fprintf(stderr, "%c", c);
	  else
	    fprintf(stderr, "%c", c);
	}
      else if(c < 7)
	{
	  graphics = 0;
	  //fprintf(stderr, "%x ", c);
	}
      else if (c >= 16 && c <= 23) 
	{
	  graphics = 1;
	  //fprintf(stderr, "%x ", c);
	}
      else
	{
	  //fprintf(stderr, "%x ", c);
	}
    }
  fprintf(stderr, "\n");
}

//////////////////////////////////////////////////////////////////////

unsigned long
getbits_hamm8(unsigned char **pp, int n, int reset)
{
  static int bitsleft = 0;
  static unsigned char c;

  int i;
  unsigned long lres = 0;

  if(reset)
    bitsleft = 0;

  for(i = 0; i < n; i++)
    {
      if(bitsleft == 0)
	{
	  unsigned char *p = *pp;

	  c = vtx_chkhamming(*p++);
	  bitsleft = 4;
	  *pp = p;
	}

      lres |= ((c & 0x01) << i);
      c >>= 1;

      bitsleft--;
    }

  return lres;
}

//////////////////////////////////////////////////////////////////////

void
dump_buffer(const char *fname, unsigned char *buf, int len)
{
  FILE *f;

  if((f = fopen(fname, "ab")))
    {
      fwrite(buf, 1, len, f);
      fclose(f);
    }
}

void
show_epg(unsigned char *pepg, int len)
{
  unsigned char *p = pepg;

  int appid = getbits_hamm8(&p, 5, 1);
  int bs = getbits_hamm8(&p, 11, 0);
  int cs = getbits_hamm8(&p, 8, 0);
  int cbs = getbits_hamm8(&p, 10, 0);
  int type = getbits_hamm8(&p, 6, 0);

  fprintf(stderr, 
	  "AppId: %d BlockSize: %d CheckSum: %x ControlSize: %d Type: %d\n",
	  appid, bs, cs, cbs, type);
}


void
handle_epg(unsigned char *buf)
{
  int mag;
  int pn;
  int bp;
  int bs;
  int off;
  int n;
  int i;

  unsigned char *p = buf;

  mag = getbits_hamm8(&p, 3, 1);
  pn = getbits_hamm8(&p, 5, 0);
  bp = getbits_hamm8(&p, 4, 0);

  off = 2+bp*3+1;

  // ignore these, they also have different mag...
  if(pn > 23)
    {
#if 0
      // 25: Empty
      // 31: EPG Teletext System

      for(i = 0; i < 42; i++)
	buf[i] = buf[i] & 0x7F;

      dump_buffer("epgx.dat", buf, 42);
#endif

      return;
    }

  fprintf(stderr, "EPG: mag %d pk %d bp 0x%x\n", mag, pn, bp);

  if(bp >= 0 && bp <= 0xC)
    {
      if(epglen)
	{
	  unsigned char *p = buf + 3;
	  n = MIN(epglen, buf + 42 - p);

	  fprintf(stderr, "Copying %d epg bytes\n", n);

	  for(i = 0; i < n; i++)
	    {
	      *epgp++ = *p;
	      epglen--;
	    }
	}

      if(epglen == 0 && epgbuf)
	{
	  show_epg(epgbuf, epgp - epgbuf);
	  free(epgbuf);
	  epgbuf = NULL;
	}

      p = buf + off;

      // seperator
      while(*p == 0x5E)
	p++;

      if(*p != 0xA1)
	{
	  fprintf(stderr, "Wrong bs\n");
	}
      else
	{
	  int bt, bl;
	  int cs, cbs, type;
	  unsigned char *psh;
	  int n;

	  p += 1;
	  psh = p;
	  bt = getbits_hamm8(&psh, 5, 1);
	  bl = getbits_hamm8(&psh, 11, 0);

	  fprintf(stderr, "EPG: bt 0x%x bl %d\n", bt, bl);

	  epglen = bl;
	  epgbuf = epgp = malloc(epglen+4);

	  for(i = 0; i < 4; i++)
	    {
	      *epgp++ = *p++;
	    }

	  n = MIN(epglen, buf + 42 - p);
	  fprintf(stderr, "Copying %d epg bytes\n", n);

	  for(i = 0; i < n; i++)
	    {
	      *epgp++ = *p;
	      epglen--;
	    }
	}
    }
  else if(bp == 0xD)
    {
      if(epglen)
	{
	  unsigned char *p = buf + 3;
	  int n = MIN(epglen, buf + 42 - p);
	  
	  fprintf(stderr, "Copying %d epg bytes\n", n);

	  for(i = 0; i < n; i++)
	    {
	      *epgp++ = *p;
	      epglen--;
	    }
	}

      if(epglen == 0 && epgbuf)
	{
	  show_epg(epgbuf, epgp - epgbuf);
	  free(epgbuf);
	  epgbuf = NULL;
	}
    }

#if 0
  for(i = 0; i < 42; i++)
    buf[i] = buf[i] & 0x7F;

  dump_buffer("epg.dat", buf, 42);
#endif
}

int
main(int argc, char **argv)
{
  int i;
  unsigned char buf[1024];
  FILE *fin = stdin;
  int plen = 48;
  int c;
  int cstart;

  int magazine, packet_number, page_number;
  int charset;

  while((c = getopt(argc, argv, "xhzvistdpa")) != EOF)
    {
      switch(c)
	{
	case 't':
	  textflag = 1;
	  break;
	case 's':
	  preselflag = 1;
	  break;
	case 'i':
	  infoflag = 1;
	  break;
	case 'p':
	  vpsflag = 1;
	  break;
	case 'd':
	  debug = 1;
	  break;
	case 'v':
	  verbose = 1;
	  break;
	case 'x':
	  hexflag = 1;
	  break;
	case 'h':
	  showheader = 1;
	  break;
	case 'z':
	  zvbi = 1;
	  break;
	case 'a':
	  ale = 1;
	  break;
	default:
	  break;
	}
    }

#ifdef WIN32
  _setmode(0, O_BINARY);
  _setmode(1, O_BINARY);
#endif

  // Videotext starts with 0x4b 0x6a
  // There are lots of lines with 0xa0.

  while(!feof(stdin))
    {
      if((c = getc(stdin)) != 0xff)
	continue;
      if((c = getc(stdin)) != 0xff)
	continue;
      if((c = getc(stdin)) != 0xff)
	continue;

      // ??
      cstart = getc(stdin);

      if(cstart == 0xFF)
	continue;

      fread(buf, 1, plen, stdin);

      if(ale)
	{
	  fwrite(buf, 1, 42, stdout);
	}

      if(1)
	{
#if 0
	  if(hexflag)
	    {
	      for(i = 0; i < plen; i++)
		{
		  int c = buf[i];
		  fprintf(stderr, "%02x ", c);
		}
	      fprintf(stderr, "\n");
	    }
#endif

	  if(vtx_chkhamming(buf[0]) == -1)
	    {
	      fprintf(stderr, "Hamming error1\n");
	      continue;
	    }
	  if(vtx_chkhamming(buf[1]) == -1)
	    {
	      fprintf(stderr, "Hamming error2\n");
	      continue;
	    }

	  magazine = (buf[0] >> 1 & 0x01) << 0;
	  magazine |= (buf[0] >> 3 & 0x01) << 1;
	  magazine |= (buf[0] >> 5 & 0x01) << 2;

	  // IDL Service, ETSI 300 708
	  if(vtx_chkhamming(buf[1]) == 0xF &&
	     vtx_chkhamming(buf[0]) != 0x0 && // Normal teletext
	     vtx_chkhamming(buf[0]) != 0xF // Packet 7/31
	     )
	    {
	      // FAB Teletext System
	      fprintf(stderr, "IDL: Channel %d\n", vtx_chkhamming(buf[0]));
	      dump_buffer("idl.dat", buf, 42);
	    }

	  packet_number = vtx_chkhamming(buf[1]) << 1;
	  packet_number |= buf[0] >> 7;

	  if(verbose || debug)
	    {
	      fprintf(stderr, "%x Packet: %02d: ", 
		      curpage, packet_number);
	    }

	  // EPG, there are strange X25 and X31 packets here
	  // Seems that this is only transmitted on KABEL1,
	  // but with blocktype 1, maybe for a special decoder?!

	  if(curpage == 0x1df && packet_number > 0)
	    {
	      handle_epg(buf);
	    }
	  else
	    {
	      // preselection data, only info
	      // ZDF sends these *before* the teletext data...
	      if(preselflag && packet_number == 26)
		{
		  // triplets are 24/18 encoded and have "functions"
		  // see page 81
		  // we ignore the "enhancement data" here and use
		  // only presel info
		  int i;
		  int designation = vtx_chkhamming(buf[2]);

		  fprintf(stderr, "MAG %d/%d Des: 0x%x\n", 
			  magazine, packet_number, designation);

		  for(i = 0; i < 39; i += 3)
		    {
		      int n;
		      int err = 0;
		      int addr, mode, data;

		      n = hamm24(&buf[i+3], &err);

		      // these seem to be unused entries,
		      // can we detect them in another way?
		      if(err)
			break;

		      addr = (n >> 0) & 0x3F;
		      mode = (n >> 6) & 0x1F;
		      data = (n >> 11) & 0x7F;

		      fprintf(stderr, "addr %d mode %x data %x\n",
			      addr,
			      mode,
			      data
			      );

		      // column address group
		      if(addr >= 0 && addr <= 39)
			{
			  int col, min;

			  switch(mode)
			    {
			    case 0x06:
			      col = addr;
			      min = data;
			      fprintf(stderr, "PDC: Col %d Min: %02x\n",
				      col, min);
			      break;
			    default:
			      break;
			    }
			}
		      // row address group
		      else if(addr >= 40 && addr <= 63)
			{
			  int row, hour, month, day;

			  switch(mode)
			    {
			    case 0x08:
			      fprintf(stderr, "PDC: Origin\n");
			      break;
			    case 0x09:
			      month = addr & 0xF;
			      day = data;
			      // month not decimal!
			      fprintf(stderr, "PDC: Month %d Day %x\n",
				      month, day);
			      break;
			    case 0x0A:
			      row = addr - 40;
			      hour = data;
			      fprintf(stderr, "PDC: Start Row %d Hour: %x\n",
				      row, hour);
			      break;
			    case 0x0B:
			      row = addr - 40;
			      hour = data;
			      fprintf(stderr, "PDC: End Row %d Hour: %x\n",
				      row, hour);
			      break;
			    case 0x0C:
			      fprintf(stderr, "PDC: Row & Offset\n");
			      break;
			    case 0x0D:
			      fprintf(stderr, "PDC: Series\n");
			      break;
			    default:
			      break;
			    }
			}
		    }
		}
	      else if(packet_number == 30)
		{
		  int designation = vtx_chkhamming(buf[2]);
		  unsigned char *p = buf + 2;

		  // why is 0 = 8 for packet 30????
		  if(magazine == 0)
		    magazine = 8;

		  if(magazine == 0x08)
		    {
		      if(infoflag && 
			 (designation == 0x00 || designation == 0x01))
			{
			  unsigned int utc = 
			    p[13]*65536+p[14]*256+p[15]-0x111111;
			  unsigned int jul = 
			    p[10]%16*65536+p[11]*256+p[12] - 0x11111;
			  double toff;

			  int y, m, d;
			  int hour, min, sec;

			  char dbuf[10];
			  char tbuf[10];

			  sprintf(tbuf, "%c%d.%d\n",
				(p[9]&0x40)?'-':'+', p[9]/4%16, (p[9]&2)?5:0);
			  toff = atof(tbuf);

			  // decimal!
			  sprintf(dbuf, "%06x", jul);
			  jul = atol(dbuf);

			  jul2greg(jul, &y, &m, &d);

			  hour = (utc >> 16) & 0xFF;
			  sprintf(tbuf, "%02x", hour);
			  hour = atol(tbuf);

			  min = (utc >> 8) & 0xFF;
			  sprintf(tbuf, "%02x", min);
			  min = atol(tbuf);

			  sec = (utc >> 0) & 0xFF;
			  sprintf(tbuf, "%02x", sec);
			  sec = atol(tbuf);

			  fprintf(stderr, "date: %02d.%02d.%04d ",
				  d, m, y);
#if 0
			  fprintf(stderr, "utc: %02d:%02d:%02d ",
				  hour, min, sec);
			  fprintf(stderr, "timeoff: %.2f ", toff);
#endif
			  fprintf(stderr, "time: %02d:%02d:%02d ",
				  hour+(int)toff, min, sec);
			  fprintf(stderr, "info: ");
			  vtx_text(p+20, 20);
			}

		      if(vpsflag && 
			 (designation == 0x02 || designation == 0x03))
			{
			  fprintf(stderr, "vpsinfo: ");
			  vtx_text(p+20, 20);
			}
		    }
		}
	    }

	  if(packet_number == 0)
	    {
	      char page[10];
	      char *p;

	      if(vtx_chkhamming(buf[2]) == -1)
		fprintf(stderr, "Hamming error3\n");
	      if(vtx_chkhamming(buf[3]) == -1)
		fprintf(stderr, "Hamming error4\n");

	      sprintf(page, "%x%x%x", 
		      magazine,
		      vtx_chkhamming(buf[3]),
		      vtx_chkhamming(buf[2]));
	      sscanf(page, "%x", &curpage);

	      if(debug || verbose || textflag)
		{
		  fprintf(stderr, "Page %s\n", page);
		}

	      for(p = page; *p; p++)
		{
		  if(!isdigit(*p))
		    {
		      // 09b: Technisat Programm Liste
		      // 1df: EPG
		      // 1e7: <ttx://0DC2/110/0001>(n:Nachrichten im Z
		      // 1ee: Only X26 and X28?
		      // 1f0: TOP Index page
		      // 1f1:
		      // 1f2:
		      // 1f3: TOP Index page
		      // 7a0:

		      fprintf(stderr, "Page %s\n", page);
		      break;
		    }
		}

	      if(showheader)
		{
		  fprintf(stderr, "Page Header\n");
		  // complete page number consist of mag, units, tens
		  fprintf(stderr, "Mag        %x\n", magazine);
		  fprintf(stderr, "Page Tens  %x\n", vtx_chkhamming(buf[3]));
		  fprintf(stderr, "Page Units %x\n", vtx_chkhamming(buf[2]));

		  fprintf(stderr, "S1         %x\n", vtx_chkhamming(buf[4]));
		  fprintf(stderr, "S2         %x\n", vtx_chkhamming(buf[5]));
		  fprintf(stderr, "S3         %x\n", vtx_chkhamming(buf[6]));
		  fprintf(stderr, "S4         %x\n", vtx_chkhamming(buf[7]));

		  fprintf(stderr, "C4         %x\n", buf[5]>>7 & 0x01);
		  fprintf(stderr, "C5         %x\n", buf[7]>>5 & 0x01);
		  fprintf(stderr, "C6         %x\n", buf[7]>>7 & 0x01);
		  fprintf(stderr, "C7         %x\n", buf[8]>>1 & 0x01);
		  fprintf(stderr, "C8         %x\n", buf[8]>>3 & 0x01);
		  fprintf(stderr, "C9         %x\n", buf[8]>>5 & 0x01);
		  fprintf(stderr, "C10        %x\n", buf[8]>>7 & 0x01);
		  fprintf(stderr, "C11        %x\n", buf[9]>>1 & 0x01);

		  charset = buf[9] >> 3 & 0x01;
		  charset |= (buf[9] >> 5 & 0x01) << 1;
		  charset |= (buf[9] >> 7 & 0x01) << 2;

		  fprintf(stderr, "Charset: %d\n", charset);
		}

	      if(debug || verbose || textflag)
		vtx_text(buf + 10, 32);
	    }
	  else
	    {
	      if(debug || verbose || textflag)
		vtx_text(buf+2, 40);
	    }

	  if(zvbi)
	    {
	      if(nslice == 0)
		{
		  printf("%f\n", dt);
		  dt += 0.25;
		  printf("%c", MAXSLICE);
		}

	      // index
	      printf("%c", 0);
	      // line
	      printf("%c", 0);
	      printf("%c", 0);
	      // data
	      fwrite(buf, 1, 42, stdout);

	      nslice++;

	      if(nslice == MAXSLICE)
		nslice = 0;
	    }
	}
    }

  return 0;
}
