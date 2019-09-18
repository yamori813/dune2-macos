// dunelib.h
//
// Time-stamp: <07/01/04 01:10:20 keuchel@rainer-keuchel.de on mediadev>

#ifndef DUNELIB_H
#define DUNELIB_H 1

#ifdef __cplusplus
#define __DUNE_BEGIN extern "C" {
#define __DUNE_END   };
#else
#define __DUNE_BEGIN
#define __DUNE_END
#endif

#define DUNELIB_VERSION "1.0"

__DUNE_BEGIN

#ifdef LINUX
#include <usb.h>
#endif

#ifdef WIN32
#include <windows.h>
#endif

#ifdef LIBUSB1
#include "libusb.h"
#endif

#ifdef LINUX
#define DUNE_DIR "/usr/local/lib/dune"
#endif

#ifdef MACOS
#define DUNE_DIR "."
#endif

#ifdef WIN32
#define DUNE_DIR "c:/usr/local/lib/dune"
#define WIN32_U2SBDEVNAME "\\\\.\\EZUSB-0"
#endif

#define PCMV5_VID 0x0411
#define PCMV5_PID 0x0031

#define DUNE_VID       0x2304
#define DUNE_PID_PAL   0x061e
#define DUNE_PID_NTSC  0x061d

#define USB_VID 0x04b4
#define USB_PID 0x8613

#define DUNE_EP02 0x02
#define DUNE_EP04 0x04
#define DUNE_EP06 0x06
#define DUNE_EP81 0x81
#define DUNE_EP82 0x82
#define DUNE_EP83 0x83
#define DUNE_EP84 0x84
#define DUNE_EP86 0x86

#define DUNE_NTSC            0
#define DUNE_PAL             1
#define DUNE_NTSC_JAPAN      2

#define DUNE_DVD        1
#define DUNE_SVCD_SVCD  2
#define DUNE_VCD_VCD    3
#define DUNE_SVCD_MPEG2 4
#define DUNE_VCD_MPEG1  5
#define DUNE_DVD_LONG   6
#define DUNE_DVD_5MBIT  7

#define DUNE_TUNER     0
#define DUNE_SVIDEO    1
#define DUNE_COMPOSITE 2

#define DUNE_I2CWRITE  0xb0
#define DUNE_I2CREAD   0xb1
#define DUNE_KFIRWRITE 0xb2
#define DUNE_KFIRREAD  0xb3
#define DUNE_MICROLOAD 0xb4
#define DUNE_B5        0xb5
#define DUNE_B6        0xb6
#define DUNE_B7        0xb7
#define DUNE_B8        0xb8
#define DUNE_B9        0xb9
#define DUNE_BA        0xba
#define DUNE_BC        0xbb
#define DUNE_BD        0xbc
#define DUNE_BE        0xbd

#define BCMD_SETVBIMODE                  0x22045C

#define BCMD_IN_SETVIDEOFORMAT           0x220410
#define BCMD_IN_SETVIDEOSIZE             0x220418
#define BCMD_IN_SETAUDIOVOLUME           0x220440
#define BCMD_IN_SETAUDIOSTANDARD         0x22044C

#define BCMD_DEC_SETVIDEOFORMAT          0x220800

#define BCMD_OUT_SETVIDEOFORMAT          0x220C04

#define BCMD_ITVC_SETVIDEOSIZE           0x22003C
#define BCMD_ITVC_SETVIDEOBITRATE        0x220044
#define BCMD_ITVC_SETFRAMERATE           0x22005C
#define BCMD_ITVC_SETAUDIORATE           0x22007C

//////////////////////////////////////////////////////////////////////
// private struct from libusb

#ifdef LINUX

#include <asm/ioctl.h>

struct usb_dev_handle {
  int fd;

  struct usb_bus *bus;
  struct usb_device *device;

  int config;
  int interface;
  int altsetting;
};

// from usbdevice_fs.h

#define USB_URB_TYPE_ISO		   0
#define USB_URB_TYPE_INTERRUPT	           1
#define USB_URB_TYPE_CONTROL	           2
#define USB_URB_TYPE_BULK		   3

struct usb_iso_packet_desc {
  unsigned int length;
  unsigned int actual_length;
  unsigned int status;
};

struct usb_urb {
  unsigned char type;
  unsigned char endpoint;
  int status;
  unsigned int flags;
  void *buffer;
  int buffer_length;
  // out params, filled on completion
  int actual_length;
  int start_frame;
  int number_of_packets;
  int error_count;
  unsigned int signr;
  // internal
  void *usercontext;
  struct usb_iso_packet_desc iso_frame_desc[0];
};

#define USB_SUBMITURB         _IOR('U', 10, struct usb_urb)
#define USB_DISCARDURB        _IO('U', 11)
#define USB_REAPURB           _IOW('U', 12, void *)
#define USB_REAPURBNDELAY     _IOW('U', 13, void *)
#endif

//////////////////////////////////////////////////////////////////////

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
#ifdef LINUX
typedef unsigned long long uint64;
#endif
#ifdef WIN32
typedef unsigned __int64 uint64;
#endif

typedef struct _srecord {
  uint32  len;
  uint32  addr;
  uint32  type;
  uint8   bytes[16];
} srecord_t;

typedef struct _vendor_cmd {
  int num;
  int len;
  unsigned int request;
  unsigned int value;
  unsigned char bytes[64];
} vendor_cmd_t;

typedef struct _duneparams {
  uint32 rectype;
  uint32 inputsource;
  uint32 videostandard;
  uint32 brightness;
  uint32 teletext;
  char *regfile;
  uint32 kfir_filter;
} duneparams_t;

typedef struct _dunedev {
#ifdef LINUX
  usb_dev_handle *usb;
#endif
#ifdef WIN32
  HANDLE usb;
#endif
#ifdef LIBUSB1
  libusb_device_handle *usb;
  libusb_context *ctx;
#endif
  int timeout;
  int fw1loaded;
  int fw2loaded;
  uint32 pid;

  uint32 saaid;

  duneparams_t params;
} dunedev_t;

typedef struct _usb_cmd {
  int num;
  int type;
  unsigned int request;
  unsigned int value;
  unsigned int endpoint;
  int len;
  int driveroffset;
  unsigned char bytes[64];
} usb_cmd_t;

typedef struct _dune_eeprom {
  uint8 b0;
  uint8 vid1;
  uint8 vid2;
  uint8 pid1;
  uint8 pid2;
  uint8 b5;
  uint8 b6;
  uint8 b7;
  uint8 b8;
  uint8 b9;
} dune_eprom_t;

typedef struct _dune_channel {
  char name[64];
  uint32 freq;
} dune_channel_t;

//////////////////////////////////////////////////////////////////////

int DuneInit();
int DuneStartDebug(int level);
int DuneSetVerbose(int n);
int DuneDebug(const char *fmt, ...);

dunedev_t *DuneOpen();
int DuneClose(dunedev_t *dev);

int DuneCommand(dunedev_t *dev, uint32 cmd, uint8 *req, int reqlen, 
		  uint8 *reply);

int DuneSetParams0(dunedev_t *bdev, uint32 code);
int DuneSetParams1(dunedev_t *bdev, uint32 code, uint32 dwValue);
int DuneSetParams2(dunedev_t *bdev, uint32 code, uint32 dwValue1, 
		     uint32 dwValue2);
int DuneSetParams3(dunedev_t *bdev, uint32 code, uint32 dwValue1, 
		     uint32 dwValue2, uint32 dwValue3);
int DuneSetParams4(dunedev_t *bdev, uint32 code, uint32 dwValue1, 
		     uint32 dwValue2, uint32 dwValue3, uint32 dwValue4);
int DuneSetParams5(dunedev_t *bdev, uint32 code, uint32 dwValue1, 
		     uint32 dwValue2, uint32 dwValue3, uint32 dwValue4, 
		     uint32 dwValue5);

int DuneI2CRead(dunedev_t *bdev, uint32 addr, uint32 subaddr, 
		  int usesub, int nbytes, uint8 *buf);
int DuneI2CWrite(dunedev_t *bdev, uint32 addr, uint32 subaddr, 
		   int usesub, int nbytes, uint8 *buf);

int DuneGetInfo1(dunedev_t *bdev);
int DuneGetInfo2(dunedev_t *bdev);
int DuneGetInfo3(dunedev_t *bdev);
int DuneGetInfo4(dunedev_t *bdev);
int DuneGetInfo5(dunedev_t *bdev);

int DuneSetInputSource(dunedev_t *bdev, uint32 source);
int DuneSetFrequency(dunedev_t *bdev, uint32 freq);

int DuneSetAudioVolumeIn(dunedev_t *bdev, uint32 left, uint32 right);
int DuneSetAudioStandard(dunedev_t *bdev, uint32 matrixmode, 
			 uint32 audiostandard);

int DuneSetInputVideoFormat(dunedev_t *bdev, uint32 format);
int DuneSetMPEGDecoderOutputVideoFormat(dunedev_t *bdev, uint32 format);
int DuneSetEncoderOutputVideoFormat(dunedev_t *bdev, uint32 format);
int DuneSetVideoSize(dunedev_t *bdev, uint32 w, uint32 h);
int DuneSetVBIMode(dunedev_t *bdev, uint32 mode);

int DuneSetupColors(dunedev_t *bdev, char *config);

int DuneStartRecording(dunedev_t *bdev);
int DuneStopRecording(dunedev_t *bdev);

int DuneStartPlaying(dunedev_t *bdev);
int DuneStopPlaying(dunedev_t *bdev);

int DuneGetMPEGDecoderStatus(dunedev_t *bdev, uint32 *pvid, uint32 *paud);

//////////////////////////////////////////////////////////////////////

int DuneReadStream(dunedev_t *bdev, unsigned char *buf, int len);
int DuneWriteStream(dunedev_t *bdev, unsigned char *buf, int len);
int DuneWriteStreamAsync(dunedev_t *bdev, unsigned char *buf, int len);
int DuneReadRemoteControl(dunedev_t *bdev, unsigned char *buf, int len);

//////////////////////////////////////////////////////////////////////
// Chip Functions

int SAA7114_Reset(dunedev_t *bdev);
uint32 SAA7114_GetStatus(dunedev_t *bdev);
uint32 SAA7114_SetColorStandard(dunedev_t *bdev, uint32 n);

int MSP3400_Init(dunedev_t *bdev);
uint32 MSP3400_GetStatus(dunedev_t *bdev, uint8 *buf);
uint32 MSP3400_GetStatusStandardResult(dunedev_t *bdev, uint8 *buf);

int Tuner_Init(dunedev_t *bdev);
int Tuner_GetStatus(dunedev_t *bdev, uint8 *buf);

int TDA9887_Init(dunedev_t *bdev);
int TDA9887_GetStatus(dunedev_t *bdev, uint8 *buf);

//////////////////////////////////////////////////////////////////////

int DuneLoadChannels();
int DuneLoadIRCodes();
int DuneGetChannelFreq(const char *name);
char *DuneGetIRCodeName(uint32 code);

//////////////////////////////////////////////////////////////////////

int DuneSetAudioRate(dunedev_t *bdev, uint32 ratecode);

//////////////////////////////////////////////////////////////////////
// New dune commands...

int DuneDumpFirmware(dunedev_t *bdev, int start, int end, char *fname);

int DuneKFirWriteCmd(dunedev_t *ddev, int reg, int val);
int DuneKFirReadCmd(dunedev_t *ddev, int reg, int *pval);

int DuneStartRecCmd(dunedev_t *ddev);
int DuneStopRecCmd(dunedev_t *ddev);

int DuneI2CReadCmd(dunedev_t *bdev, uint32 addr, uint32 subaddr, 
		   int usesub, int nbytes, uint8 *buf);
int DuneI2CWriteCmd(dunedev_t *bdev, uint32 addr, uint32 subaddr, 
		    int usesub, int nbytes, uint8 *buf);

int DuneRunCommandTable(dunedev_t *bdev, vendor_cmd_t *cmdtab);

int DuneSendMuxTable(dunedev_t *ddev, uint8 *data);

int DuneEraseMicrocodeCmd(dunedev_t *ddev);

int DuneSendBC(dunedev_t *ddev);

int DuneReadEPROM(dunedev_t *ddev, uint8 *data, int len);

int DuneSetVideoBitrate(dunedev_t *bdev, uint32 rate);

int DuneInitParams(duneparams_t *params);
int DuneReadConfig(const char *fname, duneparams_t *params);

int DuneShowChipInfo(dunedev_t *bdev, int infoflag);

__DUNE_END

#endif
