#include "dunelib.h"

// for MPEG2.02.14

int
KFir_Init_DVD1_PAL(dunedev_t *bdev)
{
  KFir_Init_X(bdev);

  DuneKFirWriteCmd(bdev, 0x0000, 0x0002);
  DuneKFirWriteCmd(bdev, 0x0001, 0x0003);
  DuneKFirWriteCmd(bdev, 0x0002, 0x0005);
  DuneKFirWriteCmd(bdev, 0x0003, 0x0005);
  DuneKFirWriteCmd(bdev, 0x0004, 0x0004);
  DuneKFirWriteCmd(bdev, 0x0005, 0x0098);
  DuneKFirWriteCmd(bdev, 0x0006, 0x003a);
  DuneKFirWriteCmd(bdev, 0x0007, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0008, 0x0070);
  DuneKFirWriteCmd(bdev, 0x0009, 0x0000);
  DuneKFirWriteCmd(bdev, 0x000a, 0x0000);
  DuneKFirWriteCmd(bdev, 0x000b, 0x0000);
  DuneKFirWriteCmd(bdev, 0x000c, 0x0001);
  DuneKFirWriteCmd(bdev, 0x000d, 0x0000);
  DuneKFirWriteCmd(bdev, 0x000e, 0x0000);
  DuneKFirWriteCmd(bdev, 0x000f, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0010, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0011, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0012, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0013, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0014, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0015, 0x0190);
  DuneKFirWriteCmd(bdev, 0x0016, 0x003f);
  DuneKFirWriteCmd(bdev, 0x0017, 0x0017);
  DuneKFirWriteCmd(bdev, 0x0018, 0x0010);
  DuneKFirWriteCmd(bdev, 0x0019, 0x9735);
  DuneKFirWriteCmd(bdev, 0x001a, 0x9735);
  DuneKFirWriteCmd(bdev, 0x001b, 0xd3b1);
  DuneKFirWriteCmd(bdev, 0x001c, 0x0000);
  DuneKFirWriteCmd(bdev, 0x001d, 0x0000);
  DuneKFirWriteCmd(bdev, 0x001e, 0x0000);
  DuneKFirWriteCmd(bdev, 0x001f, 0x0240);
  DuneKFirWriteCmd(bdev, 0x0020, 0x3a98);
  DuneKFirWriteCmd(bdev, 0x0021, 0x010f);
  DuneKFirWriteCmd(bdev, 0x0022, 0x002d);
  DuneKFirWriteCmd(bdev, 0x0023, 0x05b0);
  DuneKFirWriteCmd(bdev, 0x0024, 0x0028);
  DuneKFirWriteCmd(bdev, 0x0025, 0x007a);
  DuneKFirWriteCmd(bdev, 0x0026, 0x0166);
  DuneKFirWriteCmd(bdev, 0x0027, 0xb6db);
  DuneKFirWriteCmd(bdev, 0x0028, 0x0e10);
  DuneKFirWriteCmd(bdev, 0x0029, 0x1ae1);
  DuneKFirWriteCmd(bdev, 0x002a, 0x000b);
  DuneKFirWriteCmd(bdev, 0x002b, 0x000d);
  DuneKFirWriteCmd(bdev, 0x002c, 0xe6e0);
  DuneKFirWriteCmd(bdev, 0x002d, 0xbba0);
  DuneKFirWriteCmd(bdev, 0x002e, 0x0000);
  DuneKFirWriteCmd(bdev, 0x002f, 0x0001);
  DuneKFirWriteCmd(bdev, 0x0030, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0031, 0x0003);
  DuneKFirWriteCmd(bdev, 0x0032, 0x0005);
  DuneKFirWriteCmd(bdev, 0x0033, 0x0654);
  DuneKFirWriteCmd(bdev, 0x0034, 0x0024);
  DuneKFirWriteCmd(bdev, 0x0035, 0x0001);
  DuneKFirWriteCmd(bdev, 0x0036, 0x0001);
  DuneKFirWriteCmd(bdev, 0x0037, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0038, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0039, 0x0000);
  DuneKFirWriteCmd(bdev, 0x003a, 0x0000);
  DuneKFirWriteCmd(bdev, 0x003b, 0x0000);
  DuneKFirWriteCmd(bdev, 0x003c, 0x0001);
  DuneKFirWriteCmd(bdev, 0x003d, 0x0a28);
  DuneKFirWriteCmd(bdev, 0x003e, 0x0001);
  DuneKFirWriteCmd(bdev, 0x003f, 0x0001);
  DuneKFirWriteCmd(bdev, 0x0040, 0x0001);
  // video params, filemode never used?
  DuneKFirWriteCmd(bdev, 0x0041, 0x0397);
  DuneKFirWriteCmd(bdev, 0x0042, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0043, 0x0000);
  // filter
  DuneKFirWriteCmd(bdev, 0x0044, 0x0008);
  DuneKFirWriteCmd(bdev, 0x0045, 0xffb0);
  DuneKFirWriteCmd(bdev, 0x0046, 0x00ff);
  DuneKFirWriteCmd(bdev, 0x0047, 0x0292);
  DuneKFirWriteCmd(bdev, 0x0048, 0x00ff);
  DuneKFirWriteCmd(bdev, 0x0049, 0xffb0);
  DuneKFirWriteCmd(bdev, 0x004a, 0x0008);
  // filter
  DuneKFirWriteCmd(bdev, 0x004b, 0xfff5);
  DuneKFirWriteCmd(bdev, 0x004c, 0x006a);
  DuneKFirWriteCmd(bdev, 0x004d, 0x00ff);
  DuneKFirWriteCmd(bdev, 0x004e, 0x0144);
  DuneKFirWriteCmd(bdev, 0x004f, 0x00ff);
  DuneKFirWriteCmd(bdev, 0x0050, 0x006a);
  DuneKFirWriteCmd(bdev, 0x0051, 0xfff5);
  // filter
  DuneKFirWriteCmd(bdev, 0x0052, 0x0008);
  DuneKFirWriteCmd(bdev, 0x0053, 0xffb0);
  DuneKFirWriteCmd(bdev, 0x0054, 0x00ff);
  DuneKFirWriteCmd(bdev, 0x0055, 0x0292);
  DuneKFirWriteCmd(bdev, 0x0056, 0x00ff);
  DuneKFirWriteCmd(bdev, 0x0057, 0xffb0);
  DuneKFirWriteCmd(bdev, 0x0058, 0x0008);
  // filter
  DuneKFirWriteCmd(bdev, 0x0059, 0x0000);
  DuneKFirWriteCmd(bdev, 0x005a, 0x0000);
  DuneKFirWriteCmd(bdev, 0x005b, 0x0000);
  DuneKFirWriteCmd(bdev, 0x005c, 0x0000);
  DuneKFirWriteCmd(bdev, 0x005d, 0x0000);
  DuneKFirWriteCmd(bdev, 0x005e, 0x0000);
  DuneKFirWriteCmd(bdev, 0x005f, 0x0000);
  // filters
  DuneKFirWriteCmd(bdev, 0x0060, 0x0008);
  DuneKFirWriteCmd(bdev, 0x0061, 0xffb0);
  DuneKFirWriteCmd(bdev, 0x0062, 0x00ff);
  DuneKFirWriteCmd(bdev, 0x0063, 0x0292);
  DuneKFirWriteCmd(bdev, 0x0064, 0x00ff);
  DuneKFirWriteCmd(bdev, 0x0065, 0xffb0);
  DuneKFirWriteCmd(bdev, 0x0066, 0x0008);
  // filters
  DuneKFirWriteCmd(bdev, 0x0067, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0068, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0069, 0x0000);
  DuneKFirWriteCmd(bdev, 0x006a, 0x0000);
  DuneKFirWriteCmd(bdev, 0x006b, 0x0000);
  DuneKFirWriteCmd(bdev, 0x006c, 0x0000);
  DuneKFirWriteCmd(bdev, 0x006d, 0x0000);

  if(bdev->params.teletext)
    DuneKFirWriteCmd(bdev, 0x006e, 0x0006);
  else
    DuneKFirWriteCmd(bdev, 0x006e, 0x0000);

  DuneKFirWriteCmd(bdev, 0x006f, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0070, 0x0000);
  // vbi?
  DuneKFirWriteCmd(bdev, 0x0071, 0x00a5);
  DuneKFirWriteCmd(bdev, 0x0072, 0x005a);
  DuneKFirWriteCmd(bdev, 0x0073, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0074, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0075, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0076, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0077, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0078, 0x0001);
  DuneKFirWriteCmd(bdev, 0x0079, 0x0000);
  DuneKFirWriteCmd(bdev, 0x007a, 0x0000);
  DuneKFirWriteCmd(bdev, 0x007b, 0x00c8);
  DuneKFirWriteCmd(bdev, 0x007c, 0x000a);
  DuneKFirWriteCmd(bdev, 0x007d, 0x0000);
  DuneKFirWriteCmd(bdev, 0x007e, 0x2105);
  DuneKFirWriteCmd(bdev, 0x007f, 0x0001);
  DuneKFirWriteCmd(bdev, 0x0080, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0081, 0x0001);
  DuneKFirWriteCmd(bdev, 0x0082, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0083, 0x0041);
  DuneKFirWriteCmd(bdev, 0x0084, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0085, 0x0000);
  // audiotype mpeg
  DuneKFirWriteCmd(bdev, 0x0086, 0x0001);
  DuneKFirWriteCmd(bdev, 0x0087, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0088, 0x98da);
  DuneKFirWriteCmd(bdev, 0x0089, 0x0001);
  DuneKFirWriteCmd(bdev, 0x008a, 0x0001);
  DuneKFirWriteCmd(bdev, 0x008b, 0x00ce);
  DuneKFirWriteCmd(bdev, 0x008c, 0x026a);
  DuneKFirWriteCmd(bdev, 0x008d, 0x0338);
  DuneKFirWriteCmd(bdev, 0x008e, 0x014c);
  DuneKFirWriteCmd(bdev, 0x008f, 0x026a);
  DuneKFirWriteCmd(bdev, 0x0090, 0x0011);
  DuneKFirWriteCmd(bdev, 0x0091, 0x02bc);
  DuneKFirWriteCmd(bdev, 0x0092, 0x02bc);
  DuneKFirWriteCmd(bdev, 0x0093, 0x02bc);
  // audiofrequency
  DuneKFirWriteCmd(bdev, 0x0094, 0xbb80);
  DuneKFirWriteCmd(bdev, 0x0095, 0x002a);
  DuneKFirWriteCmd(bdev, 0x0096, 0x0001);
  DuneKFirWriteCmd(bdev, 0x0097, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0098, 0x0000);
  DuneKFirWriteCmd(bdev, 0x0099, 0x0000);
  DuneKFirWriteCmd(bdev, 0x009a, 0x0000);
  DuneKFirWriteCmd(bdev, 0x009b, 0x0000);
  DuneKFirWriteCmd(bdev, 0x009c, 0x0000);
  DuneKFirWriteCmd(bdev, 0x009d, 0x0000);
  DuneKFirWriteCmd(bdev, 0x009e, 0x0000);
  DuneKFirWriteCmd(bdev, 0x009f, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00a0, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00a1, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00a2, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00a3, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00a4, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00a5, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00a6, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00a7, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00a8, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00a9, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00aa, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00ab, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00ac, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00ad, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00ae, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00af, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00b0, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00b1, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00b2, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00b3, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00b4, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00b5, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00b6, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00b7, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00b8, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00b9, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00ba, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00bb, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00bc, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00bd, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00be, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00bf, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00c0, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00c1, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00c2, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00c3, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00c4, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00c5, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00c6, 0x0000);
  DuneKFirWriteCmd(bdev, 0x00c7, 0x0000);

  DuneKFirWriteCmd(bdev, 0x014b, 0x0000);

  if(bdev->params.teletext)
    DuneKFirWriteCmd(bdev, 0x014f, 0x0002);
  else
    DuneKFirWriteCmd(bdev, 0x014f, 0x0000);

  DuneKFirWriteCmd(bdev, 0x0190, 0xbba0);
  DuneKFirWriteCmd(bdev, 0x0191, 0x000d);

  // autopause threshold
  DuneKFirWriteCmd(bdev, 0x0192, 0x0018);
  // first gop peak bitrate?
  DuneKFirWriteCmd(bdev, 0x0197, 0x0064);

  return 0;
}

uint8 muxtable_dvd1_pal[0x1000] = {
0x00,0x00,0x02,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,
0x00,0x00,0x10,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x12,0x00,0x00,0x00,0xe0,0x00,
0x00,0x00,0xc0,0x00,0x00,0x00,0x80,0x8d,0x00,0x00,0x5b,0x00,0x00,0x00,0x00,0x6b,
0x00,0x00,0x03,0x00,0x00,0x00,0x40,0x54,0x00,0x00,0x89,0x00,0x00,0x00,0xe4,0x57,
0x00,0x00,0x00,0x00,0x00,0x00,0xe4,0x57,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,
0x00,0x00,0x00,0x08,0x00,0x00,0xf4,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,
0x00,0x00,0x1a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xbb,
0x00,0x00,0xee,0xa0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x54,
0x00,0x00,0x89,0x00,0x00,0x00,0x81,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x47,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0xc8,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,0x2c,0x5b,0x00,0x00,0x00,0x00,
0x00,0x00,0x2c,0x5b,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe1,0x00,0x00,0x04,0x01,
0x00,0x00,0x50,0xc0,0x00,0x00,0xc0,0x00,0x00,0x00,0x50,0xe0,0x00,0x00,0xe0,0x00,
0x00,0x00,0x0f,0x00,0x00,0x00,0x40,0x54,0x00,0x00,0x89,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc4,0x09,0x00,0x00,0x00,0x00,
0x00,0x00,0xc8,0xaf,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0x7a,
0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x0e,0x00,0x00,0x10,0x0e,
0x00,0x00,0x30,0x2a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x20,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x49,0x00,
0x00,0x00,0xc4,0x23,0x00,0x00,0xc5,0x54,0x00,0x00,0x5a,0x58,0x00,0x00,0xd4,0x00,
0x00,0x00,0x23,0xc5,0x00,0x00,0x6b,0x7c,0x00,0x00,0x00,0x01,0x00,0x00,0x04,0x00,
0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x0d,0x7a,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x62,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x4e,0x9f,0x00,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xd1,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x14,0x00,
0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,0x01,0x00,0x00,0x00,0x10,0x00,
0x00,0x00,0x16,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,
0x00,0x00,0x80,0x6c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x56,0xbb,
0x00,0x00,0x66,0xb0,0x00,0x00,0x5a,0x58,0x00,0x00,0xd4,0x00,0x00,0x00,0x6a,0x89,
0x00,0x00,0x63,0xb8,0x00,0x00,0x46,0x8a,0x00,0x00,0xd7,0xf8,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0xb0,0x00,
0x00,0x00,0x00,0x0d,0x00,0x00,0xc3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,
0x00,0x00,0x10,0xe0,0x00,0x00,0xfe,0xb3,0x00,0x00,0x39,0x48,0x00,0x00,0x01,0x00,
0x00,0x00,0x10,0x00,0x00,0x00,0x1a,0x00,0x00,0x00,0xb0,0x02,0x00,0x00,0x00,0x17,
0x00,0x00,0xc3,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0xe0,0x00,0x00,0x00,0xf0,
0x00,0x00,0xe0,0x03,0x00,0x00,0xf0,0x12,0x00,0x00,0x02,0x00,0x00,0x00,0x11,0xe0,
0x00,0x00,0x00,0xf0,0x00,0x00,0x95,0x20,0x00,0x00,0x65,0x18,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};