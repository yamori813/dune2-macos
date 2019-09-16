#include "dunelib.h"

int
KFir_Init_VCD2_VCD_NTSC(dunedev_t *bdev)
{
  // NOTE: No filters
  DuneKFirWriteCmd(bdev, 0x00000000, 0x0000000c);
  DuneKFirWriteCmd(bdev, 0x00000001, 0x00000004);
  DuneKFirWriteCmd(bdev, 0x00000002, 0x00000005);
  DuneKFirWriteCmd(bdev, 0x00000003, 0x00000005);
  DuneKFirWriteCmd(bdev, 0x00000004, 0x00000004);
  DuneKFirWriteCmd(bdev, 0x00000005, 0x0000003b);
  DuneKFirWriteCmd(bdev, 0x00000006, 0x0000000b);
  DuneKFirWriteCmd(bdev, 0x00000007, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000008, 0x00000014);
  DuneKFirWriteCmd(bdev, 0x00000009, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x0000000a, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x0000000b, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000000c, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000000d, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000000e, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000000f, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000010, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000011, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000012, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x00000013, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x00000014, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x00000015, 0x00000190);
  DuneKFirWriteCmd(bdev, 0x00000016, 0x0000000c);
  DuneKFirWriteCmd(bdev, 0x00000017, 0x00000004);
  DuneKFirWriteCmd(bdev, 0x00000018, 0x00000003);
  DuneKFirWriteCmd(bdev, 0x00000019, 0x0000182d);
  DuneKFirWriteCmd(bdev, 0x0000001a, 0x0000182d);
  DuneKFirWriteCmd(bdev, 0x0000001b, 0x000021d8);
  DuneKFirWriteCmd(bdev, 0x0000001c, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000001d, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000001e, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000001f, 0x00000240);
  DuneKFirWriteCmd(bdev, 0x00000020, 0x0000095e);
  DuneKFirWriteCmd(bdev, 0x00000021, 0x0000069e);
  DuneKFirWriteCmd(bdev, 0x00000022, 0x00000016);
  DuneKFirWriteCmd(bdev, 0x00000023, 0x00000ba2);
  DuneKFirWriteCmd(bdev, 0x00000024, 0x000000c6);
  DuneKFirWriteCmd(bdev, 0x00000025, 0x00000282);
  DuneKFirWriteCmd(bdev, 0x00000026, 0x00000166);
  DuneKFirWriteCmd(bdev, 0x00000027, 0x0000b6db);
  DuneKFirWriteCmd(bdev, 0x00000028, 0x00000bbb);
  DuneKFirWriteCmd(bdev, 0x00000029, 0x0000190b);
  DuneKFirWriteCmd(bdev, 0x0000002a, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x0000002b, 0x00000002);
  DuneKFirWriteCmd(bdev, 0x0000002c, 0x0000e724);
  DuneKFirWriteCmd(bdev, 0x0000002d, 0x00003215);
  DuneKFirWriteCmd(bdev, 0x0000002e, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000002f, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000030, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000031, 0x00000003);
  DuneKFirWriteCmd(bdev, 0x00000032, 0x00000005);
  DuneKFirWriteCmd(bdev, 0x00000033, 0x0000014a);
  DuneKFirWriteCmd(bdev, 0x00000034, 0x0000000f);
  DuneKFirWriteCmd(bdev, 0x00000035, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x00000036, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000037, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000038, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000039, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000003a, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000003b, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000003c, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x0000003d, 0x00000a28);
  DuneKFirWriteCmd(bdev, 0x0000003e, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x0000003f, 0x00000001);
  DuneKFirWriteCmd(bdev, 0x00000040, 0x00000002);
  DuneKFirWriteCmd(bdev, 0x00000041, 0x00000280);
  DuneKFirWriteCmd(bdev, 0x00000042, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000043, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000044, 0x00000013);
  DuneKFirWriteCmd(bdev, 0x00000045, 0x0000ffac);
  DuneKFirWriteCmd(bdev, 0x00000046, 0x000000e4);
  DuneKFirWriteCmd(bdev, 0x00000047, 0x000002ba);
  DuneKFirWriteCmd(bdev, 0x00000048, 0x000000e4);
  DuneKFirWriteCmd(bdev, 0x00000049, 0x0000ffac);
  DuneKFirWriteCmd(bdev, 0x0000004a, 0x00000013);
  DuneKFirWriteCmd(bdev, 0x0000004b, 0x0000fff5);
  DuneKFirWriteCmd(bdev, 0x0000004c, 0x0000006a);
  DuneKFirWriteCmd(bdev, 0x0000004d, 0x000000ff);
  DuneKFirWriteCmd(bdev, 0x0000004e, 0x00000144);
  DuneKFirWriteCmd(bdev, 0x0000004f, 0x000000ff);
  DuneKFirWriteCmd(bdev, 0x00000050, 0x0000006a);
  DuneKFirWriteCmd(bdev, 0x00000051, 0x0000fff5);
  DuneKFirWriteCmd(bdev, 0x00000052, 0x00000013);
  DuneKFirWriteCmd(bdev, 0x00000053, 0x0000ffac);
  DuneKFirWriteCmd(bdev, 0x00000054, 0x000000e4);
  DuneKFirWriteCmd(bdev, 0x00000055, 0x000002ba);
  DuneKFirWriteCmd(bdev, 0x00000056, 0x000000e4);
  DuneKFirWriteCmd(bdev, 0x00000057, 0x0000ffac);
  DuneKFirWriteCmd(bdev, 0x00000058, 0x00000013);
  DuneKFirWriteCmd(bdev, 0x00000059, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000005a, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000005b, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000005c, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000005d, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000005e, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000005f, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000060, 0x00000013);
  DuneKFirWriteCmd(bdev, 0x00000061, 0x0000ffac);
  DuneKFirWriteCmd(bdev, 0x00000062, 0x000000e4);
  DuneKFirWriteCmd(bdev, 0x00000063, 0x000002ba);
  DuneKFirWriteCmd(bdev, 0x00000064, 0x000000e4);
  DuneKFirWriteCmd(bdev, 0x00000065, 0x0000ffac);
  DuneKFirWriteCmd(bdev, 0x00000066, 0x00000013);
  DuneKFirWriteCmd(bdev, 0x00000067, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000068, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000069, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000006a, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000006b, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000006c, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x0000006d, 0x00000000);
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
  DuneKFirWriteCmd(bdev, 0x00000088, 0x00001870);
  DuneKFirWriteCmd(bdev, 0x00000089, 0x00000000);
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
  DuneKFirWriteCmd(bdev, 0x00000094, 0x0000ac44);
  DuneKFirWriteCmd(bdev, 0x00000095, 0x0000002a);
  DuneKFirWriteCmd(bdev, 0x00000096, 0x00000000);
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
  DuneKFirWriteCmd(bdev, 0x0000014f, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000190, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000191, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x00000192, 0x00000018);
  DuneKFirWriteCmd(bdev, 0x00000197, 0x00000064);

  return 0;
}

uint8 muxtable_vcd2_vcd_ntsc[0x1000] = {
0x00,0x00,0x05,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,
0x00,0x00,0x10,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x12,0x00,0x00,0x00,0xe0,0x00,
0x00,0x00,0xc0,0x00,0x00,0x00,0x30,0x8c,0x00,0x00,0x11,0x00,0x00,0x00,0x00,0x6b,
0x00,0x00,0x03,0x00,0x00,0x00,0xe0,0x46,0x00,0x00,0x15,0x00,0x00,0x00,0xa0,0x8c,
0x00,0x00,0x00,0x00,0x00,0x00,0xa0,0x8c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,
0x00,0x00,0x00,0x08,0x00,0x00,0xf4,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,
0x00,0x00,0x1a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xbb,
0x00,0x00,0xee,0xa0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x54,
0x00,0x00,0x89,0x00,0x00,0x00,0x81,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x7c,0x06,0x00,0x00,0x35,0x01,0x00,0x00,0x42,0x00,0x00,0x00,0x01,0x00,
0x00,0x00,0x6b,0x0d,0x00,0x00,0x00,0x00,0x00,0x00,0xc2,0x0e,0x00,0x00,0x00,0x00,
0x00,0x00,0xc2,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xe1,0x00,0x00,0x04,0x01,
0x00,0x00,0x04,0xc0,0x00,0x00,0xc0,0x00,0x00,0x00,0x2e,0xe0,0x00,0x00,0xe0,0x00,
0x00,0x00,0x0f,0x00,0x00,0x00,0x40,0x54,0x00,0x00,0x89,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xb5,0x0b,0x00,0x00,0xd9,0xd0,
0x00,0x00,0xb7,0x16,0x00,0x00,0x00,0x00,0x00,0x00,0xa1,0xe6,0x00,0x00,0x24,0xbf,
0x00,0x00,0x0d,0x00,0x00,0x00,0xc4,0x00,0x00,0x00,0xbb,0x0b,0x00,0x00,0xbb,0x0b,
0x00,0x00,0x31,0x23,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x20,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xd9,0x01,
0x00,0x00,0xa4,0xa5,0x00,0x00,0x2a,0x53,0x00,0x00,0x5a,0x58,0x00,0x00,0xd4,0x00,
0x00,0x00,0x23,0xc5,0x00,0x00,0x6b,0x7c,0x00,0x00,0x00,0x01,0x00,0x00,0x04,0x00,
0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x95,0xfc,0x00,0x00,0x0d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x6e,0x6f,0x00,0x00,0x7d,0x02,
0x00,0x00,0x00,0x00,0x00,0x00,0x4e,0x9f,0x00,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x02,0x00,0x00,0x00,0xd1,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x14,0x00,
0x00,0x00,0xff,0xff,0x00,0x00,0xff,0xff,0x00,0x00,0x01,0x00,0x00,0x00,0x10,0x00,
0x00,0x00,0x16,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,
0x00,0x00,0x80,0x6c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x56,0xbb,
0x00,0x00,0x66,0xb0,0x00,0x00,0x5a,0x58,0x00,0x00,0xd4,0x00,0x00,0x00,0x6a,0x89,
0x00,0x00,0x63,0xb8,0x00,0x00,0x46,0x8a,0x00,0x00,0xd7,0xf8,0x00,0x00,0x14,0x09,
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