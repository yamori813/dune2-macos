#include "dunelib.h"

// for MPEG2.02.26 

int
KFir_Init_DVD2_PAL(dunedev_t *bdev)
{
  KFir_Init_X(bdev);
  KFir_Init_Filters2(bdev);

  DuneKFirWriteCmd(bdev, 0x00000000, 0x00000002);
  DuneKFirWriteCmd(bdev, 0x00000001, 0x00000003);
  DuneKFirWriteCmd(bdev, 0x00000002, 0x00000005);
  DuneKFirWriteCmd(bdev, 0x00000003, 0x00000005);
  DuneKFirWriteCmd(bdev, 0x00000004, 0x00000004);
  DuneKFirWriteCmd(bdev, 0x00000005, 0x00000098);
  DuneKFirWriteCmd(bdev, 0x00000006, 0x0000003a);
  DuneKFirWriteCmd(bdev, 0x00000007, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000008, 0x00000070);
  DuneKFirWriteCmd(bdev, 0x00000009, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000000a, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000000b, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000000c, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x0000000d, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000000e, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000000f, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000010, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000011, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000012, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000013, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000014, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000015, 0x00000190);
  DuneKFirWriteCmd(bdev, 0x00000016, 0x0000003f);
  DuneKFirWriteCmd(bdev, 0x00000017, 0x00000017);
  DuneKFirWriteCmd(bdev, 0x00000018, 0x00000010);
  DuneKFirWriteCmd(bdev, 0x00000019, 0x00009735);
  DuneKFirWriteCmd(bdev, 0x0000001a, 0x00009735);
  DuneKFirWriteCmd(bdev, 0x0000001b, 0x0000d3b1);
  DuneKFirWriteCmd(bdev, 0x0000001c, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000001d, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000001e, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000001f, 0x00000240);
  DuneKFirWriteCmd(bdev, 0x00000020, 0x00003a98);
  DuneKFirWriteCmd(bdev, 0x00000021, 0x0000010f);
  DuneKFirWriteCmd(bdev, 0x00000022, 0x0000002d);
  DuneKFirWriteCmd(bdev, 0x00000023, 0x000005b0);
  DuneKFirWriteCmd(bdev, 0x00000024, 0x00000028);
  DuneKFirWriteCmd(bdev, 0x00000025, 0x0000007a);
  DuneKFirWriteCmd(bdev, 0x00000026, 0x00000166);
  DuneKFirWriteCmd(bdev, 0x00000027, 0x0000b6db);
  DuneKFirWriteCmd(bdev, 0x00000028, 0x00000e10);
  DuneKFirWriteCmd(bdev, 0x00000029, 0x00001ae1);
  DuneKFirWriteCmd(bdev, 0x0000002a, 0x0000000b);
  DuneKFirWriteCmd(bdev, 0x0000002b, 0x0000000d);
  DuneKFirWriteCmd(bdev, 0x0000002c, 0x0000e6e0);
  DuneKFirWriteCmd(bdev, 0x0000002d, 0x0000bba0);
  DuneKFirWriteCmd(bdev, 0x0000002e, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000002f, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x00000030, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000031, 0x00000003);
  DuneKFirWriteCmd(bdev, 0x00000032, 0x00000005);
  DuneKFirWriteCmd(bdev, 0x00000033, 0x00000654);
  DuneKFirWriteCmd(bdev, 0x00000034, 0x00000024);
  DuneKFirWriteCmd(bdev, 0x00000035, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x00000036, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x00000037, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000038, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000039, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000003a, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000003b, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000003c, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x0000003d, 0x00000a28);
  DuneKFirWriteCmd(bdev, 0x0000003e, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x0000003f, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x00000040, 0x00000001);
  // video params, filemode never used?
  DuneKFirWriteCmd(bdev, 0x00000041, 0x000003bf);
  // these are now all zero..., filters etc
  DuneKFirWriteCmd(bdev, 0x00000042, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000043, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000044, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000045, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000046, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000047, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000048, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000049, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000004a, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000004b, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000004c, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000004d, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000004e, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000004f, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000050, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000051, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000052, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000053, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000054, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000055, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000056, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000057, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000058, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000059, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000005a, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000005b, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000005c, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000005d, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000005e, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000005f, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000060, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000061, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000062, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000063, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000064, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000065, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000066, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000067, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000068, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000069, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000006a, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000006b, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000006c, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000006d, 0x00000000);
  if(bdev->params.teletext)
    DuneKFirWriteCmd(bdev, 0x0000006e, 0x00000006);
  else
    DuneKFirWriteCmd(bdev, 0x0000006e, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000006f, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000070, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000071, 0x000000a5);
  DuneKFirWriteCmd(bdev, 0x00000072, 0x0000005a);
  DuneKFirWriteCmd(bdev, 0x00000073, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000074, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000075, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000076, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000077, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000078, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x00000079, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000007a, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000007b, 0x000000c8);
  DuneKFirWriteCmd(bdev, 0x0000007c, 0x0000000a);
  DuneKFirWriteCmd(bdev, 0x0000007d, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000007e, 0x00002105);
  DuneKFirWriteCmd(bdev, 0x0000007f, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x00000080, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000081, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x00000082, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000083, 0x00000041);
  DuneKFirWriteCmd(bdev, 0x00000084, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000085, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000086, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x00000087, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000088, 0x000098da);
  DuneKFirWriteCmd(bdev, 0x00000089, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x0000008a, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x0000008b, 0x000000ce);
  DuneKFirWriteCmd(bdev, 0x0000008c, 0x0000026a);
  DuneKFirWriteCmd(bdev, 0x0000008d, 0x00000338);
  DuneKFirWriteCmd(bdev, 0x0000008e, 0x0000014c);
  DuneKFirWriteCmd(bdev, 0x0000008f, 0x0000026a);
  DuneKFirWriteCmd(bdev, 0x00000090, 0x00000011);
  DuneKFirWriteCmd(bdev, 0x00000091, 0x000002bc);
  DuneKFirWriteCmd(bdev, 0x00000092, 0x000002bc);
  DuneKFirWriteCmd(bdev, 0x00000093, 0x000002bc);
  // audiofrequency
  DuneKFirWriteCmd(bdev, 0x00000094, 0x0000bb80);
  DuneKFirWriteCmd(bdev, 0x00000095, 0x0000002a);
  DuneKFirWriteCmd(bdev, 0x00000096, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x00000097, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000098, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000099, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000009a, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000009b, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000009c, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000009d, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000009e, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000009f, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000a0, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000a1, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000a2, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000a3, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000a4, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000a5, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000a6, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000a7, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000a8, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000a9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000aa, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000ab, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000ac, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000ad, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000ae, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000af, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000b0, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000b1, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000b2, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000b3, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000b4, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000b5, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000b6, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000b7, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000b8, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000b9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000ba, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000bb, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000bc, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000bd, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000be, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000bf, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c0, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c1, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c2, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c3, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c4, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c5, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c6, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c7, 0x00000000);

  DuneKFirWriteCmd(bdev, 0x0000014b, 0x00000000);

  if(bdev->params.teletext)
    DuneKFirWriteCmd(bdev, 0x0000014f, 0x00000002);
  else
    DuneKFirWriteCmd(bdev, 0x0000014f, 0x00000000);

  // 9000000, bitrate related
  DuneKFirWriteCmd(bdev, 0x00000190, 0x0000bba0);
  DuneKFirWriteCmd(bdev, 0x00000191, 0x0000000d);

  DuneKFirWriteCmd(bdev, 0x00000192, 0x00000018);
  DuneKFirWriteCmd(bdev, 0x00000197, 0x00000064);

  return 0;
}

uint8 muxtable_dvd2_pal[0x1000] = {
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
0x00,0x00,0xc8,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,0xdc,0x42,0x00,0x00,0x00,0x00,
0x00,0x00,0xdc,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe1,0x00,0x00,0x04,0x01,
0x00,0x00,0x04,0xc0,0x00,0x00,0xc0,0x00,0x00,0x00,0xe2,0xe0,0x00,0x00,0xe0,0x00,
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
0x00,0x00,0x95,0xfc,0x00,0x00,0x0d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
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
