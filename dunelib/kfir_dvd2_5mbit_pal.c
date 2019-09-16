#include "dunelib.h"

// Defaults from GraphEdit
// 720x480, 5 MBit Bitrate/6.5 MBit Peak Bitrate, 48 KHz Audio, 224 KBit Audio
// Frames per GOP 15, IP Dist 3

int
KFir_Init_DVD2_5MBIT_PAL(dunedev_t *bdev)
{
  KFir_Init_X(bdev);
  KFir_Init_Filters2(bdev);

  return 0;
}

uint8 muxtable_dvd2_5mbit_pal[0x1000] = {
};
