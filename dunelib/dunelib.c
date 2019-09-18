// dunelib.c
//
// Time-stamp: <07/01/04 01:39:48 keuchel@rainer-keuchel.de on mediadev>

#ifdef LINUX
#include <unistd.h>
#endif

#ifdef WIN32
#include <windows.h>
#include <io.h>
#include <malloc.h>
#endif

#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "dunelib.h"
#include "kfir.h"

#ifdef EZUSB
#include "ezusb.h"
#include "ezusbsys.h"
#endif

//#define UCODEVERSION "MPEG2.02.26"
#define UCODEVERSION "MPEG2.02.27b"

#define stricmp strcasecmp

#if defined(LINUX) || defined(MACOS)
#define O_BINARY 0
#endif
#if defined(MACOS)
typedef unsigned long long uint64;

int
vendorrequest_write(dunedev_t *bdev, int req, int val,
                    unsigned char *buf, int size);
int
vendorrequest_write2(dunedev_t *bdev, int req, int val, int idx,
                     unsigned char *buf, int size);
int
vendorrequest_read(dunedev_t *bdev, int req, int val,
                   unsigned char *buf, int size);
#endif

#define TUNERI2CADDR	0xc0
#define TDAI2CADDR	0x86
#define SAAI2CADDR	0x42
#define MSPI2CADDR	0x80

#define MAXCHANNELS 126
#define MAXIRCODES  126

char *dunedir = DUNE_DIR;

static int debug;
static int verbose;

struct ircode {
  char *name;
  uint32 single;
  uint32 multi;
};

dune_channel_t dune_channels[MAXCHANNELS];
static int nchannels;

static struct ircode ircodes[MAXCHANNELS];
static int nircodes;

static int dune_ezusb2_version;
static char *dune_ucode_version;

//////////////////////////////////////////////////////////////////////
// these are from the usblib test program...

#ifdef LINUX
static void 
print_endpoint(struct usb_endpoint_descriptor *endpoint)
{
  fprintf(stderr,"bEndpointAddress: %02xh\n", endpoint->bEndpointAddress);
  fprintf(stderr,"bmAttributes:     %02xh\n", endpoint->bmAttributes);
  fprintf(stderr,"wMaxPacketSize:   %d\n", endpoint->wMaxPacketSize);
  fprintf(stderr,"bInterval:        %d\n", endpoint->bInterval);
  fprintf(stderr,"bRefresh:         %d\n", endpoint->bRefresh);
  fprintf(stderr,"bSynchAddress:    %d\n", endpoint->bSynchAddress);
}

static void 
print_altsetting(struct usb_interface_descriptor *interface)
{
  int i;

  fprintf(stderr,"bInterfaceNumber:   %d\n", interface->bInterfaceNumber);
  fprintf(stderr,"bAlternateSetting:  %d\n", interface->bAlternateSetting);
  fprintf(stderr,"bNumEndpoints:      %d\n", interface->bNumEndpoints);
  fprintf(stderr,"bInterfaceClass:    %d\n", interface->bInterfaceClass);
  fprintf(stderr,"bInterfaceSubClass: %d\n", interface->bInterfaceSubClass);
  fprintf(stderr,"bInterfaceProtocol: %d\n", interface->bInterfaceProtocol);
  fprintf(stderr,"iInterface:         %d\n", interface->iInterface);
  
  for (i = 0; i < interface->bNumEndpoints; i++)
    {
      fprintf(stderr, "*** Endpoint %d ***\n", i);
      print_endpoint(&interface->endpoint[i]);
    }
}

static void 
print_interface(struct usb_interface *interface)
{
  int i;

  for (i = 0; i < interface->num_altsetting; i++)
    {
      fprintf(stderr, "*** AltSetting %d ***\n", i);
      print_altsetting(&interface->altsetting[i]);
    }
}

static void 
print_configuration(struct usb_config_descriptor *config)
{
  int i;

  fprintf(stderr,"wTotalLength:         %d\n", config->wTotalLength);
  fprintf(stderr,"bNumInterfaces:       %d\n", config->bNumInterfaces);
  fprintf(stderr,"bConfigurationValue:  %d\n", config->bConfigurationValue);
  fprintf(stderr,"iConfiguration:       %d\n", config->iConfiguration);
  fprintf(stderr,"bmAttributes:         %02xh\n", config->bmAttributes);
  fprintf(stderr,"MaxPower:             %d\n", config->MaxPower);
  
  for (i = 0; i < config->bNumInterfaces; i++)
    {
      fprintf(stderr, "*** Interface %d ***\n", i);
      print_interface(&config->interface[i]);
    }
}

#endif

#if defined(LINUX) || defined(EZUSB)
static int
vendorrequest_write(dunedev_t *bdev, int req, int val, 
		    unsigned char *buf, int size)
{
  int reqtype = USB_TYPE_VENDOR | USB_ENDPOINT_OUT;
  int timeout = 5000;

  if(debug > 1 && size < 1024)
    {
      int i;

      fprintf(stderr, "VendorWrite: %x, %x\n", req, val);
      for(i = 0; i < size; i++)
	fprintf(stderr, "%02x ", buf[i]);
      fprintf(stderr, "\n");
    }

  if(usb_control_msg(bdev->usb, reqtype, req, val, 0, buf, size, 
		     timeout) < 0)
    {
      fprintf(stderr, "Vendorrequest_Write(): %s\n", usb_strerror());
      return -1;
    }

  //usleep(10);

  return 0;
}

// with index, for microcode and msp status...
static int
vendorrequest_write2(dunedev_t *bdev, int req, int val, int idx,
		     unsigned char *buf, int size)
{
  int reqtype = USB_TYPE_VENDOR | USB_ENDPOINT_OUT;
  int timeout = 5000;

  if(debug > 2 && size < 1024)
    {
      int i;

      fprintf(stderr, "VendorWrite: %x, %x\n", req, val);
      for(i = 0; i < size; i++)
	fprintf(stderr, "%02x ", buf[i]);
      fprintf(stderr, "\n");
    }

  if(usb_control_msg(bdev->usb, reqtype, req, val, idx, buf, size, 
		     timeout) < 0)
    {
      fprintf(stderr, "Vendorrequest_Write(): %s\n", usb_strerror());
      return -1;
    }

  //usleep(10);

  return 0;
}

static int
vendorrequest_read(dunedev_t *bdev, int req, int val, 
		   unsigned char *buf, int size)
{
  int reqtype = USB_TYPE_VENDOR | USB_ENDPOINT_IN;
  int timeout = 5000;

  if(debug > 1)
    fprintf(stderr, "VendorRead: %x, %x\n", req, val);

  if(usb_control_msg(bdev->usb, reqtype, req, val, 0, buf, size, 
		     timeout) < 0)
    {
      fprintf(stderr, "Vendorrequest_Read(): %s\n", usb_strerror());
      return -1;
    }

  return 0;
}

#endif // LINUX or EZUSB

//////////////////////////////////////////////////////////////////////

int
DuneInit()
{
  char *p;

  if((p = getenv("DUNE_DIR")) != NULL)
    dunedir = p;

  DuneLoadChannels();
  DuneLoadIRCodes();

  return 0;
}

int
DuneStartDebug(int n)
{
  debug = n;
  return 0;
}

int
DuneSetVerbose(int n)
{
  verbose = n;
  return 0;
}

int 
DuneDebug(const char *fmt, ...)
{
  static FILE *f = NULL;
  va_list ap;

  if(f == NULL)
    {
      char path[256];

      sprintf(path, "%s/%s", dunedir, "debug.log");
      if((f = fopen(path, "w")) == NULL)
	return -1;
    }

  va_start(ap, fmt);
  vfprintf(f, fmt, ap);
  fprintf(f, "\n");
  //fflush(f);

  return 0;
}

static int
DuneInitDev(dunedev_t *bdev)
{
#ifdef LINUX
  if(usb_claim_interface(bdev->usb, 0) < 0)
    {
      fprintf(stderr, "Cannot claim interface: %s\n", usb_strerror());
      return -1;
    }
#endif

  bdev->timeout = 3000;
  bdev->fw1loaded = 0;
  bdev->fw2loaded = 0;

  bdev->saaid = 0;

  return 0;
}

#ifdef LINUX
dunedev_t *
DuneOpen()
{
  struct usb_bus *bus;
  struct usb_device *dev;
  dunedev_t *bdev;
  int i;
  int retry = 0;
  int epcount = 0;

  if(geteuid() != 0)
    {
      fprintf(stderr, "DuneOpen() might fail\n");
      fprintf(stderr, "You must run this program as root\n");
    }

  usb_init();
  usb_find_busses();
  usb_find_devices();

  for (bus = usb_busses; bus; bus = bus->next) {
    for (dev = bus->devices; dev; dev = dev->next) {
      if(debug > 2) {
	fprintf(stderr, "VID %x PID %x\n",
		dev->descriptor.idVendor, dev->descriptor.idProduct);
      }

      // this is the vid/pid of the usb chip vendor...
      if(dev->descriptor.idVendor == USB_VID &&
	 dev->descriptor.idProduct == USB_PID)
	{
	  fprintf(stderr, "USB Chip VID %x PID %x found.\n",
		  dev->descriptor.idVendor, dev->descriptor.idProduct);
	  fprintf(stderr, "The device did not change it's VID/PID.\n");
	  fprintf(stderr, "You must reset the device by replugging power.\n");
	  return NULL;
	}
	
      if(dev->descriptor.idVendor == DUNE_VID &&
	 (dev->descriptor.idProduct == DUNE_PID_PAL ||
	  dev->descriptor.idProduct == DUNE_PID_NTSC))
	{
	  if(debug)
	    {
	      fprintf(stderr, "Dune at %s/%s VID %04X PID %04X\n", 
		      bus->dirname, dev->filename,
		      dev->descriptor.idVendor, dev->descriptor.idProduct);
	    }

	  if (dev->config == NULL) {
	    fprintf(stderr, "Cannot retrieve descriptors...\n");
	    fprintf(stderr, "Replug power cable for reset...\n");
	    return NULL;
	  }

	  epcount = dev->config[0].interface[0].altsetting[0].bNumEndpoints;

	  if (debug > 1) 
	    {
	      for (i = 0; i < dev->descriptor.bNumConfigurations; i++)
		{
		  fprintf(stderr, "*** Configuration %d ***\n", i);
		  print_configuration(&dev->config[i]);
		}
	    }

	  bdev = malloc(sizeof(dunedev_t));

	  if((bdev->usb = usb_open(dev)) <= 0)
	    {
	      fprintf(stderr, "Cannot open usb: %s\n", usb_strerror());
	      return NULL;
	    }

	  bdev->pid = dev->descriptor.idProduct;

	  if(DuneInitDev(bdev) < 0)
	    {
	      return NULL;
	    }

	  if(epcount == 2)
	    {
	      // check only
	      DuneLoadFirmwareEZUSB2(bdev, NULL, 0);
	    }
	  else 
	    {
	      fprintf(stderr, "Loading ezusb2 firmware...\n");
	      DuneLoadFirmwareEZUSB2(bdev, NULL, 1);
	      fprintf(stderr, "You have to restart the program!\n");
	      DuneClose(bdev);
	      exit(1);
	    }

	  return bdev;
	}
    }
  }

  fprintf(stderr, "Cannot find dune!\n");
  return NULL;
}
#endif // LINUX

#ifdef WIN32
dunedev_t *
DuneOpen()
{
  dunedev_t *bdev;
  char *devname = WIN32_U2SBDEVNAME;
  int epcount = 0;
  int kval = 0;

  bdev = malloc(sizeof(dunedev_t));

  if((bdev->usb = CreateFile(devname, GENERIC_READ|GENERIC_WRITE, 0, NULL,
			     OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE)
    {
      uint32 dwError = GetLastError();

      if(dwError == ERROR_FILE_NOT_FOUND)
	fprintf(stderr, "Device not found. Check USB cable connection!\n");
      else
	fprintf(stderr, "Cannot open device: %d\n", GetLastError());
      return NULL;
    }

  if(DuneInitDev(bdev) < 0)
    {
      return NULL;
    }

  epcount = ezusb_getnumpipes(bdev->usb);

  if(epcount < 0)
    {
      fprintf(stderr, "Cannot determine pipe count: %s\n", usb_strerror());
      exit(1);
    }
  else if(epcount == 2)
    {
      DuneKFirReadCmd(bdev, 0xfe, &kval);
      fprintf(stderr, "Status: 0x%x\n", kval);
    }

  if(epcount == 0 || kval == 0x04)
    {
      if(kval == 0x04 || kval == 0x00)
	fprintf(stderr, "Unsane device state 0x%x\n", kval);

      fprintf(stderr, "Loading ezusb2 firmware...\n");
      if(DuneLoadFirmwareEZUSB2(bdev, NULL, 1) < 0)
	exit(1);

      fprintf(stderr, "Loading ezusb2 firmware...done\n");

      // even needed on win32?!
      fprintf(stderr, "You have to restart the program!\n");
      DuneClose(bdev);
      exit(2);
    }
  else
    {
      if(DuneLoadFirmwareEZUSB2(bdev, NULL, 0) < 0)
	exit(1);
    }

  return bdev;
}
#endif // WIN32

#ifdef MACOS
dunedev_t *
DuneOpen()
{
  dunedev_t *bdev;

  int r, cnt;
  libusb_device **devs;
  libusb_device_handle *dev_handle;
  int vid, pid;
  struct libusb_device_descriptor desc;
  libusb_context *ctx = NULL;
  int i;
  int sn = 0;

  r = libusb_init(&ctx);

  cnt = libusb_get_device_list(ctx, &devs);

  vid = -1;
  for (i = 0; i < cnt; ++i) {
    r = libusb_get_device_descriptor(devs[i], &desc);
    if (desc.idVendor == PCMV5_VID && desc.idProduct == PCMV5_VID) {
      sn = desc.iSerialNumber;
      vid = desc.idVendor;
      pid = desc.idProduct;
      break;
    }
  }

  if (vid == -1) {
    fprintf(stderr, "Cannot find dune!\n");
    return NULL;
  }

  bdev = malloc(sizeof(dunedev_t));

  bdev->ctx = ctx;
  bdev->usb = libusb_open_device_with_vid_pid(ctx, vid, pid);

  if(sn != 0)
    {
      // check only
      DuneLoadFirmwareEZUSB2(bdev, NULL, 0);
    }
  else 
    {
      fprintf(stderr, "Loading ezusb2 firmware...\n");
      DuneLoadFirmwareEZUSB2(bdev, NULL, 1);
      fprintf(stderr, "You have to restart the program!\n");
      DuneClose(bdev);
      exit(1);
    }
  libusb_claim_interface(bdev->usb, 0);

  return bdev;
}
#endif // MACOS

#ifdef LINUX
int
DuneClose(dunedev_t *bdev)
{
  if(bdev == NULL || bdev->usb == 0)
    {
      errno = EBADF;
      return -1;
    }

#ifdef LINUX
  usb_release_interface(bdev->usb, 0);
#endif

  usb_close(bdev->usb);
  free(bdev);

  return 0;
}
#endif // LINUX

#ifdef WIN32
int
DuneClose(dunedev_t *dev)
{
  CloseHandle(dev->usb);
  free(dev);

  return 0;
}
#endif // LINUX

#ifdef MACOS
int
DuneClose(dunedev_t *dev)
{

  libusb_close(dev->usb);
  libusb_exit(dev->ctx);
  return 0;
}
#endif // MACOS

int
MSP3400_WriteReg(dunedev_t *bdev, int subaddr, int reg, int val)
{
  int res;
  uint16 buf[2];

  buf[0] = htons(reg);
  buf[1] = htons(val);

  res = DuneI2CWriteCmd(bdev, MSPI2CADDR, subaddr, 1, 4, (uint8 *) buf);

  return res;
}

int
MSP3400_Reset(dunedev_t *bdev)
{
  int res;
  uint16 val;

  val = htons(0x8000);
  res = DuneI2CWriteCmd(bdev, MSPI2CADDR, 0x00, 1, 2, (uint8 *) &val);
  val = htons(0x0000);
  res = DuneI2CWriteCmd(bdev, MSPI2CADDR, 0x00, 1, 2, (uint8 *) &val);

  return res;
}

int
MSP3400_Init(dunedev_t *bdev)
{
  MSP3400_Reset(bdev);

  // ACB, input
  MSP3400_WriteReg(bdev, 0x12, 0x13, 0x0C00);

  if(bdev->params.inputsource == DUNE_TUNER)
    {
      // speaker source/channel matrix
      MSP3400_WriteReg(bdev, 0x12, 0x08, 0x0020);
      // scart1 source/channel matrix
      MSP3400_WriteReg(bdev, 0x12, 0x0A, 0x0020);
    }
  else
    {
      // speaker source/channel matrix
      MSP3400_WriteReg(bdev, 0x12, 0x08, 0x0220);
      // scart1 source/channel matrix
      MSP3400_WriteReg(bdev, 0x12, 0x0A, 0x0220);
    }

  // prescale fm/fm matrix
  MSP3400_WriteReg(bdev, 0x12, 0x0E, 0x2400);
  // prescale nicam
  MSP3400_WriteReg(bdev, 0x12, 0x10, 0x2000);
  // prescale scart input
  MSP3400_WriteReg(bdev, 0x12, 0x0D, 0x1D00);
  // mode tone control (bass/treble)
  MSP3400_WriteReg(bdev, 0x12, 0x20, 0x0001);
  // volume loadspeaker channel
  MSP3400_WriteReg(bdev, 0x12, 0x00, 0x7300);
  // volume scart1 output channel
  MSP3400_WriteReg(bdev, 0x12, 0x07, 0x7301);

  // modus: automatic sound select
  if(bdev->params.videostandard == DUNE_NTSC)
    {
      // prefer BTSC for 4.5 MHz carrier (USA)
      // 3451G probably does not support ntsc sound, check tuner type!
      MSP3400_WriteReg(bdev, 0x10, 0x30, 0x2001);
    }
  else
    {
      MSP3400_WriteReg(bdev, 0x10, 0x30, 0x7001);
    }

  // standard select automatic on
  MSP3400_WriteReg(bdev, 0x10, 0x20, 0x0001);

  //usleep(1000);

  return 0;
}

uint32
MSP3400_GetStandardResult(dunedev_t *bdev, uint8 *buf)
{
  int res;
  uint16 reg;

  reg = htons(0x007e);

  res = DuneI2CWriteCmd(bdev, MSPI2CADDR, 0x11, 1, 2, (uint8 *) &reg);
  res = DuneI2CReadCmd(bdev, MSPI2CADDR, 0xff, 1, 2, buf);

  reg = *(uint16 *) buf;
  reg = ntohs(reg);
  *(uint16 *) buf = reg;

  return buf[0];
}

uint32
MSP3400_GetStatus(dunedev_t *bdev, uint8 *buf)
{
  int res;
  uint16 reg;

  reg = htons(0x0200);

  res = DuneI2CWriteCmd(bdev, MSPI2CADDR, 0x11, 1, 2, (uint8 *) &reg);
  res = DuneI2CReadCmd(bdev, MSPI2CADDR, 0xff, 1, 2, buf);

  reg = *(uint16 *) buf;
  reg = ntohs(reg);
  *(uint16 *) buf = reg;
  
  return buf[0];
}

uint32
MSP3400_GetVersion(dunedev_t *bdev, uint8 *buf)
{
  int res;
  uint16 reg;

  reg = htons(0x001F);

  res = DuneI2CWriteCmd(bdev, MSPI2CADDR, 0x13, 1, 2, (uint8 *) &reg);
  res = DuneI2CReadCmd(bdev, MSPI2CADDR, 0xff, 1, 2, buf);

  reg = *(uint16 *) buf;
  reg = ntohs(reg);
  *(uint16 *) buf = reg;
  
  return buf[0];
}

//////////////////////////////////////////////////////////////////////

int
Tuner_Init(dunedev_t *bdev)
{
  int res;
  uint8 fm1636mk3[] = {0x0e,0x20,0x8e,0x02,0xa0};
 
  res = DuneI2CWriteCmd(bdev, TUNERI2CADDR, 0, 0, sizeof(fm1636mk3), fm1636mk3);

  return res;
}

int
Tuner_GetStatus(dunedev_t *bdev, uint8 *buf)
{
  int res;

  res = DuneI2CRead(bdev, TUNERI2CADDR, 0x00, 0x00, 0x01, buf);

  return res;
}

//////////////////////////////////////////////////////////////////////
// TDA9887 demodulator

// first byte is subaddr

// negative fm tv, qss carrier mode, automute, 0 dB takeover point
static uint8 pll_pal_bg[] = { 0x00, 0x16, 0x70, 0x49 }; // 38.90 MHz FM 5.5
static uint8 pll_ntsc[] =   { 0x00, 0x16, 0x70, 0x44 }; // 45.75 MHz FM 4.5
static uint8 pll_pal_i[] =  { 0x00, 0x16, 0x70, 0x4a }; // 38.90 MHz FM 6.0
static uint8 pll_pal_dk[] = { 0x00, 0x16, 0x70, 0x4b }; // 38.90 MHz FM 6.5
// ???
//static uint8 pll_pal_l[] =  { 0x00, 0x06, 0x50, 0x4b };

int
TDA9887_Init(dunedev_t *bdev)
{
  int res;

  // i finally found out what this is...
  //uint8 unknown[]   = {0x00,0x16,0x70,0x49};
  //res = DuneI2CWriteCmd(bdev, 0x86, 0, 0, sizeof(unknown), unknown);

  if(bdev->params.videostandard == DUNE_PAL)
    res = DuneI2CWriteCmd(bdev, TDAI2CADDR, 0, 0, 4, pll_pal_bg);
  else if(bdev->params.videostandard == DUNE_NTSC)
    res = DuneI2CWriteCmd(bdev, TDAI2CADDR, 0, 0, 4, pll_ntsc);

  return res;
}

int
TDA9887_GetStatus(dunedev_t *bdev, uint8 *buf)
{
  int res;

  res = DuneI2CRead(bdev, TDAI2CADDR, 0x00, 0x00, 0x01, buf);

  return res;
}

//////////////////////////////////////////////////////////////////////

static uint8 saa7114regs[0xFF];

int
SAA7114_WriteReg(dunedev_t *bdev, uint8 sub, uint8 val)
{
  int res;

  res = DuneI2CWriteCmd(bdev, SAAI2CADDR, sub, 1, 1, &val);

  return res;
}

int
SAA7114_WriteBlock(dunedev_t *bdev, int startsub, int endsub)
{
  int res;
  int len = endsub - startsub + 1;

  res = DuneI2CWriteCmd(bdev, SAAI2CADDR, startsub, 1, len, saa7114regs + startsub);

  return res;
}

int
SAA7114_Reset(dunedev_t *bdev)
{
  uint8 i2c;
  int i;

  bdev->saaid = 0;

  // This strange sequence is also in dunepal, not documented in SAA docs.
  // Also resets the input selection led to composite..

  // Documented in SAA7115 docs...

/*
  for(i = 0; i < 1000; i++)
    {
      i2c = 0;

      DuneI2CWrite(bdev, SAAI2CADDR, 0x00, 0x01, 0x01, &i2c);
      DuneI2CRead(bdev, SAAI2CADDR, 0x00, 0x01, 0x01, &i2c);

      if(i2c == 1)
	break;
    }
*/

  for(i = 0; i < 7; i++)
    {
      i2c = i;

      DuneI2CWrite(bdev, SAAI2CADDR, 0x00, 0x01, 0x01, &i2c);
      DuneI2CRead(bdev, SAAI2CADDR, 0x00, 0x01, 0x01, &i2c);
      bdev->saaid |= i2c;
      bdev->saaid <<= 4;
    }

  return 0;
}

uint32
SAA7114_GetStatus(dunedev_t *bdev)
{
  uint8 i2c = 0;

  DuneI2CReadCmd(bdev, SAAI2CADDR, 0x1F, 1, 0x01, &i2c);

  return i2c;
}

// From old driver

int
SAA7114_Init_PAL(dunedev_t *bdev, duneparams_t *params)
{
  memset(saa7114regs, 0, sizeof(saa7114regs));

  saa7114regs[0x00] = 0x00;
  saa7114regs[0x01] = 0x08;

  if(params->inputsource == DUNE_COMPOSITE)
    saa7114regs[0x02] = 0xc0;
  else if(params->inputsource == DUNE_SVIDEO)
    saa7114regs[0x02] = 0xc7;
  else if(params->inputsource == DUNE_TUNER)
    saa7114regs[0x02] = 0xc5;

  if(params->inputsource == DUNE_TUNER)
    saa7114regs[0x03] = 0x20;
  else
    saa7114regs[0x03] = 0x24;

  saa7114regs[0x04] = 0x00;
  saa7114regs[0x05] = 0x00;
  saa7114regs[0x06] = 0xeb;
  saa7114regs[0x07] = 0xe0;

  // sync, 50 Hz, 625 lines
  // use tvmode only for poor quality tv?!
  if(params->inputsource == DUNE_TUNER)
    saa7114regs[0x08] = 0x20; // tvmode, odd/even toggle
  else
    saa7114regs[0x08] = 0x38; // fast locking, odd/even toggle

  // luminance control
  if(params->inputsource == DUNE_COMPOSITE)
    saa7114regs[0x09] = 0x40; // comb active
  else if(params->inputsource == DUNE_SVIDEO)
    saa7114regs[0x09] = 0xC0; // bypass trap/comb filter
  else if(params->inputsource == DUNE_TUNER)
    saa7114regs[0x09] = 0x45; // comb, 5 = resolution enhancement 3.0dB

  saa7114regs[0x0a] = 0x80;
  saa7114regs[0x0b] = 0x40;
  saa7114regs[0x0c] = 0x40;
  saa7114regs[0x0d] = 0x00;

  // chrominance control/colour standard
  saa7114regs[0x0e] = 0x81; // adaptive chrominance filter

  saa7114regs[0x0f] = 0x00;
  saa7114regs[0x10] = 0x06;
  saa7114regs[0x11] = 0x41;
  saa7114regs[0x12] = 0x00;
  saa7114regs[0x13] = 0x00;

  if(params->inputsource == DUNE_COMPOSITE)
    saa7114regs[0x14] = 0x10;
  else if(params->inputsource == DUNE_SVIDEO)
    saa7114regs[0x14] = 0x20;
  else if(params->inputsource == DUNE_TUNER)
    saa7114regs[0x14] = 0x20;

  saa7114regs[0x15] = 0x11;
  saa7114regs[0x16] = 0xfe;
  saa7114regs[0x17] = 0x40;
  saa7114regs[0x18] = 0x40;
  saa7114regs[0x19] = 0x80;

  if(params->rectype == DUNE_VCD_VCD || params->rectype == DUNE_SVCD_SVCD ||
     params->rectype == DUNE_VCD_MPEG1 || params->rectype == DUNE_SVCD_MPEG2)
    {
      // Audio master clock, cylces per field
      // For 44.1KHz audio and 50 Hz field frequency
      // See page 58 for samples...
      saa7114regs[0x30] = 0x00;
      saa7114regs[0x31] = 0x72;
      saa7114regs[0x32] = 0x03;
      saa7114regs[0x33] = 0x00;
      
      saa7114regs[0x34] = 0xf2;
      saa7114regs[0x35] = 0x00;
      saa7114regs[0x36] = 0x2d;
      saa7114regs[0x37] = 0x00;
    }
  else
    {
      // Audio master clock, cylces per field
      // For 48KHz audio and 50 Hz field frequency
      // See page 58 for samples...
      saa7114regs[0x30] = 0x00;
      saa7114regs[0x31] = 0xc0;
      saa7114regs[0x32] = 0x03;
      saa7114regs[0x33] = 0x00;
      
      saa7114regs[0x34] = 0xce;
      saa7114regs[0x35] = 0xfb;
      saa7114regs[0x36] = 0x30;
      saa7114regs[0x37] = 0x00;
    }

  saa7114regs[0x38] = 0x03;
  saa7114regs[0x39] = 0x10;

  // audio clock control
  saa7114regs[0x3a] = 0x01; //field locked
  //saa7114regs[0x3a] = 0x09; //free-running - old driver?

  // vbi slicer
  saa7114regs[0x40] = 0x40;
  // line control lines2-4
  saa7114regs[0x41] = 0xff; // active video
  saa7114regs[0x42] = 0xff;
  saa7114regs[0x43] = 0xff;
  saa7114regs[0x44] = 0xff;
  saa7114regs[0x45] = 0x00; // WST
  saa7114regs[0x46] = 0x00;
  saa7114regs[0x47] = 0x00;
  saa7114regs[0x48] = 0x00;
  saa7114regs[0x49] = 0x00;
  saa7114regs[0x4a] = 0x00;
  saa7114regs[0x4b] = 0x00;
  saa7114regs[0x4c] = 0x00;
  saa7114regs[0x4d] = 0x00;
  saa7114regs[0x4e] = 0x00;
  saa7114regs[0x4f] = 0x00;
  saa7114regs[0x50] = 0x00;
  saa7114regs[0x51] = 0x00;
  saa7114regs[0x52] = 0x00;
  saa7114regs[0x53] = 0x00;
  saa7114regs[0x54] = 0x00;
  saa7114regs[0x55] = 0x00;
  saa7114regs[0x56] = 0xff; // active video
  saa7114regs[0x57] = 0xff;

  saa7114regs[0x58] = 0x01;
  saa7114regs[0x59] = 0x47;

  // slicer offsets
  saa7114regs[0x5a] = 0x03;
  saa7114regs[0x5b] = 0x43;

  saa7114regs[0x5c] = 0x00;
  saa7114regs[0x5d] = 0xbe;
  saa7114regs[0x5e] = 0x2a;
  saa7114regs[0x5f] = 0x00;

  // global control, task, slicer defines timings for scaler output
  saa7114regs[0x80] = 0x50;
  saa7114regs[0x81] = 0x00;
  saa7114regs[0x82] = 0x00;
  saa7114regs[0x83] = 0x01;
  saa7114regs[0x84] = 0xa0;
  saa7114regs[0x85] = 0x10;

  // iport fifo control, video and data
  saa7114regs[0x86] = 0xc5;
  saa7114regs[0x87] = 0x01;
  saa7114regs[0x88] = 0xf0;

  // task a
  saa7114regs[0x90] = 0x00;
  saa7114regs[0x91] = 0x08;
  saa7114regs[0x92] = 0x10;
  saa7114regs[0x93] = 0x80;

  // horizontal input offset
  saa7114regs[0x94] = 0x02;
  saa7114regs[0x95] = 0x00;

  // horizontal length, 720
  saa7114regs[0x96] = 0xd0;
  saa7114regs[0x97] = 0x02;

  // vertical input offset
  saa7114regs[0x98] = 0x14;
  saa7114regs[0x99] = 0x00;

  // vertical input window length, 300
  saa7114regs[0x9a] = 0x2c;
  saa7114regs[0x9b] = 0x01;

  // horizontal output window length, 720
  saa7114regs[0x9c] = 0xd0;
  saa7114regs[0x9d] = 0x02;

  // vertical output window length, 300
  saa7114regs[0x9e] = 0x2c;
  saa7114regs[0x9f] = 0x01;

  saa7114regs[0xa0] = 0x01;
  saa7114regs[0xa1] = 0x00;
  saa7114regs[0xa2] = 0x00;
  saa7114regs[0xa3] = 0x00;
  saa7114regs[0xa4] = 0x80;
  saa7114regs[0xa5] = 0x40;
  saa7114regs[0xa6] = 0x40;
  saa7114regs[0xa7] = 0x00;
  saa7114regs[0xa8] = 0x00;
  saa7114regs[0xa9] = 0x04;
  saa7114regs[0xaa] = 0x00;
  saa7114regs[0xab] = 0x00;
  saa7114regs[0xac] = 0x00;
  saa7114regs[0xad] = 0x02;
  saa7114regs[0xae] = 0x00;
  saa7114regs[0xaf] = 0x00;
  saa7114regs[0xb0] = 0x00;
  saa7114regs[0xb1] = 0x04;
  saa7114regs[0xb2] = 0x00;
  saa7114regs[0xb3] = 0x04;
  saa7114regs[0xb4] = 0x01;
  saa7114regs[0xb5] = 0x00;
  saa7114regs[0xb6] = 0x00;
  saa7114regs[0xb7] = 0x00;
  saa7114regs[0xb8] = 0x00;
  saa7114regs[0xb9] = 0x00;
  saa7114regs[0xba] = 0x00;
  saa7114regs[0xbb] = 0x00;
  saa7114regs[0xbc] = 0x00;
  saa7114regs[0xbd] = 0x00;
  saa7114regs[0xbe] = 0x00;
  saa7114regs[0xbf] = 0x00;

  // dont know why, but this order/dup seems important...
  SAA7114_WriteBlock(bdev, 0x90, 0xbf);
  SAA7114_WriteBlock(bdev, 0x80, 0x88);

  SAA7114_WriteBlock(bdev, 0x00, 0x19);
  SAA7114_WriteBlock(bdev, 0x30, 0x3a);
  SAA7114_WriteBlock(bdev, 0x40, 0x5f);
  SAA7114_WriteBlock(bdev, 0x80, 0x88);
  SAA7114_WriteBlock(bdev, 0x90, 0xbf);

  return 0;
}

// From new driver

int
SAA7114_Init_NTSC(dunedev_t *bdev, duneparams_t *params)
{
  memset(saa7114regs, 0, sizeof(saa7114regs));

  saa7114regs[0x0] = 0x0;
  saa7114regs[0x1] = 0x8;

  if(params->inputsource == DUNE_COMPOSITE)
    saa7114regs[0x02] = 0xc0;
  else if(params->inputsource == DUNE_SVIDEO)
    saa7114regs[0x02] = 0xc7;
  else if(params->inputsource == DUNE_TUNER)
    saa7114regs[0x02] = 0xc5;

  // gain
  saa7114regs[0x3] = 0x0;
  saa7114regs[0x4] = 0x90;
  saa7114regs[0x5] = 0x90;

  saa7114regs[0x6] = 0xeb;
  saa7114regs[0x7] = 0xe0;

  // sync, 60 Hz, 525 lines
  if(params->inputsource == DUNE_TUNER)
    saa7114regs[0x8] = 0x60; // tvmode, odd/even toggle
  else
    saa7114regs[0x8] = 0x78; // fast locking, odd/even toggle also 0x38?!

  // luminance control
  if(params->inputsource == DUNE_COMPOSITE)
    saa7114regs[0x09] = 0x40; // comb active
  else if(params->inputsource == DUNE_SVIDEO)
    saa7114regs[0x09] = 0xC0; // bypass trap/comb filter
  else if(params->inputsource == DUNE_TUNER)
    saa7114regs[0x09] = 0x45; // comb, 5 = resolution enhancement 3.0dB

  saa7114regs[0xa] = 0x80;
  saa7114regs[0xb] = 0x44;
  saa7114regs[0xc] = 0x40;
  saa7114regs[0xd] = 0x0;

  // chrominance control/colour standard
  saa7114regs[0xe] = 0x89; // adaptive chrominance filter, ntsc

  saa7114regs[0xf] = 0x0;

  saa7114regs[0x10] = 0x0E;
  saa7114regs[0x11] = 0x41;

  // rt signal control
  saa7114regs[0x12] = 0x00;
  // rt/xport out control
  saa7114regs[0x13] = 0x00;
  // aout connection
  saa7114regs[0x14] = 0x00; // 0x10?
  // vgate
  saa7114regs[0x15] = 0x11;

  saa7114regs[0x16] = 0xfe;
  saa7114regs[0x17] = 0x40;
  saa7114regs[0x18] = 0x40;
  saa7114regs[0x19] = 0x80;

  saa7114regs[0x1a] = 0x0;
  saa7114regs[0x1b] = 0x0;
  saa7114regs[0x1c] = 0x0;
  saa7114regs[0x1d] = 0x0;
  saa7114regs[0x1e] = 0x0;

  saa7114regs[0x1f] = 0x0;
  saa7114regs[0x20] = 0x0;
  saa7114regs[0x21] = 0x0;
  saa7114regs[0x22] = 0x0;
  saa7114regs[0x23] = 0x0;
  saa7114regs[0x24] = 0x0;
  saa7114regs[0x25] = 0x0;
  saa7114regs[0x26] = 0x0;
  saa7114regs[0x27] = 0x0;
  saa7114regs[0x28] = 0x0;
  saa7114regs[0x29] = 0x0;
  saa7114regs[0x2a] = 0x0;
  saa7114regs[0x2b] = 0x0;
  saa7114regs[0x2c] = 0x0;
  saa7114regs[0x2d] = 0x0;
  saa7114regs[0x2e] = 0x0;
  saa7114regs[0x2f] = 0x0;

  if(params->rectype == DUNE_VCD_VCD || params->rectype == DUNE_SVCD_SVCD ||
     params->rectype == DUNE_VCD_MPEG1 || params->rectype == DUNE_SVCD_MPEG2)
    {
      // Audio master clock, cylces per field
      // For 44.1KHz audio and 59.94 Hz field frequency
      saa7114regs[0x30] = 0xbc;
      saa7114regs[0x31] = 0xdf;
      saa7114regs[0x32] = 0x02;
      saa7114regs[0x33] = 0x00;

      saa7114regs[0x34] = 0xf2;
      saa7114regs[0x35] = 0x00;
      saa7114regs[0x36] = 0x2d;
      saa7114regs[0x37] = 0x00;
    }
  else
    {
      // Audio master clock, cylces per field
      // For 48KHz audio and 59.94 Hz field frequency
      // See page 58 for samples...
      saa7114regs[0x30] = 0xcd;
      saa7114regs[0x31] = 0x20;
      saa7114regs[0x32] = 0x03;
      saa7114regs[0x33] = 0x0;

      saa7114regs[0x34] = 0xce;
      saa7114regs[0x35] = 0xfb;
      saa7114regs[0x36] = 0x30;
      saa7114regs[0x37] = 0x0;
    }

  // clock ratio
  saa7114regs[0x38] = 0x3;
  // clock ratio
  saa7114regs[0x39] = 0x10;

  // audio clock control
  saa7114regs[0x3a] = 0x1; // field-locked
  //saa7114regs[0x3a] = 0x9; // free-running

  saa7114regs[0x3b] = 0x0;
  saa7114regs[0x3c] = 0x0;
  saa7114regs[0x3d] = 0x0;
  saa7114regs[0x3e] = 0x0;
  saa7114regs[0x3f] = 0x0;

  // vbi slicer
  saa7114regs[0x40] = 0x40;
  // line control lines 2-24
  saa7114regs[0x41] = 0xff;
  saa7114regs[0x42] = 0xff;
  saa7114regs[0x43] = 0xff;
  saa7114regs[0x44] = 0xff;
  saa7114regs[0x45] = 0xff;
  saa7114regs[0x46] = 0xff;
  saa7114regs[0x47] = 0xff;
  saa7114regs[0x48] = 0xff;
  saa7114regs[0x49] = 0xff;
  saa7114regs[0x4a] = 0xff;
  saa7114regs[0x4b] = 0xff;
  saa7114regs[0x4c] = 0xff;
  saa7114regs[0x4d] = 0xff;
  saa7114regs[0x4e] = 0xff;
  saa7114regs[0x4f] = 0xff;
  saa7114regs[0x50] = 0xff;
  saa7114regs[0x51] = 0xff;
  saa7114regs[0x52] = 0xff;
  saa7114regs[0x53] = 0xff;
  saa7114regs[0x54] = 0xff;
  saa7114regs[0x55] = 0xff;
  saa7114regs[0x56] = 0xff;
  saa7114regs[0x57] = 0xff;

  saa7114regs[0x58] = 0x1;
  saa7114regs[0x59] = 0x47;

  // slicer offsets
  saa7114regs[0x5a] = 0x03;
  saa7114regs[0x5b] = 0x03;

  saa7114regs[0x5c] = 0x0;
  saa7114regs[0x5d] = 0xbe;
  saa7114regs[0x5e] = 0x2a;
  saa7114regs[0x5f] = 0x0;
  saa7114regs[0x60] = 0x0;
  saa7114regs[0x61] = 0x0;
  saa7114regs[0x62] = 0x0;
  saa7114regs[0x63] = 0x0;
  saa7114regs[0x64] = 0x0;
  saa7114regs[0x65] = 0x0;
  saa7114regs[0x66] = 0x0;
  saa7114regs[0x67] = 0x0;
  saa7114regs[0x68] = 0x0;
  saa7114regs[0x69] = 0x0;
  saa7114regs[0x6a] = 0x0;
  saa7114regs[0x6b] = 0x0;
  saa7114regs[0x6c] = 0x0;
  saa7114regs[0x6d] = 0x0;
  saa7114regs[0x6e] = 0x0;
  saa7114regs[0x6f] = 0x0;
  saa7114regs[0x70] = 0x0;
  saa7114regs[0x71] = 0x0;
  saa7114regs[0x72] = 0x0;
  saa7114regs[0x73] = 0x0;
  saa7114regs[0x74] = 0x0;
  saa7114regs[0x75] = 0x0;
  saa7114regs[0x76] = 0x0;
  saa7114regs[0x77] = 0x0;
  saa7114regs[0x78] = 0x0;
  saa7114regs[0x79] = 0x0;
  saa7114regs[0x7a] = 0x0;
  saa7114regs[0x7b] = 0x0;
  saa7114regs[0x7c] = 0x0;
  saa7114regs[0x7d] = 0x0;
  saa7114regs[0x7e] = 0x0;
  saa7114regs[0x7f] = 0x0;

  // global control, task a
  if(params->teletext)
    saa7114regs[0x80] = 0x50; // slicer defines timings for scaler output
  else
    saa7114regs[0x80] = 0x10; // window defines timings for scaler output

  saa7114regs[0x81] = 0x0;
  saa7114regs[0x82] = 0x0;
  saa7114regs[0x83] = 0x1;
  saa7114regs[0x84] = 0xa0;
  saa7114regs[0x85] = 0x10;

  // iport fifo control
  if(params->teletext)
    saa7114regs[0x86] = 0xc5; // video and data
  else
    saa7114regs[0x86] = 0x45; // video only

  saa7114regs[0x87] = 0x1;
  saa7114regs[0x88] = 0xf0;
  saa7114regs[0x89] = 0x0;
  saa7114regs[0x8a] = 0x0;
  saa7114regs[0x8b] = 0x0;
  saa7114regs[0x8c] = 0x0;
  saa7114regs[0x8d] = 0x0;
  saa7114regs[0x8e] = 0x0;
  saa7114regs[0x8f] = 0x0;

  // task a
  saa7114regs[0x90] = 0x0;
  saa7114regs[0x91] = 0x8;
  saa7114regs[0x92] = 0x10;
  saa7114regs[0x93] = 0x80;

  // horizontal input offset
  saa7114regs[0x94] = 0x2;
  saa7114regs[0x95] = 0x0;

  // horizontal length, 720
  saa7114regs[0x96] = 0xd0;
  saa7114regs[0x97] = 0x2;

  // vertical input offset
  saa7114regs[0x98] = 0x10;
  saa7114regs[0x99] = 0x00;

  // vertical input window length, 256
  saa7114regs[0x9a] = 0xff;
  saa7114regs[0x9b] = 0x00;

  // horizontal output window length, 720
  saa7114regs[0x9c] = 0xd0;
  saa7114regs[0x9d] = 0x2;

  // vertical output window length, 256
  saa7114regs[0x9e] = 0xff;
  saa7114regs[0x9f] = 0x00;

  saa7114regs[0xa0] = 0x1;
  saa7114regs[0xa1] = 0x0;
  saa7114regs[0xa2] = 0x0;
  saa7114regs[0xa3] = 0x0;
  saa7114regs[0xa4] = 0x80;
  saa7114regs[0xa5] = 0x40;
  saa7114regs[0xa6] = 0x40;
  saa7114regs[0xa7] = 0x0;
  saa7114regs[0xa8] = 0x0;
  saa7114regs[0xa9] = 0x4;
  saa7114regs[0xaa] = 0x0;
  saa7114regs[0xab] = 0x0;
  saa7114regs[0xac] = 0x0;
  saa7114regs[0xad] = 0x2;
  saa7114regs[0xae] = 0x0;
  saa7114regs[0xaf] = 0x0;
  saa7114regs[0xb0] = 0x0;
  saa7114regs[0xb1] = 0x4;
  saa7114regs[0xb2] = 0x0;
  saa7114regs[0xb3] = 0x4;
  saa7114regs[0xb4] = 0x1;
  saa7114regs[0xb5] = 0x0;
  saa7114regs[0xb6] = 0x0;
  saa7114regs[0xb7] = 0x0;
  saa7114regs[0xb8] = 0x0;
  saa7114regs[0xb9] = 0x0;
  saa7114regs[0xba] = 0x0;
  saa7114regs[0xbb] = 0x0;
  saa7114regs[0xbc] = 0x0;
  saa7114regs[0xbd] = 0x0;
  saa7114regs[0xbe] = 0x0;
  saa7114regs[0xbf] = 0x0;
  saa7114regs[0xc0] = 0x0;
  saa7114regs[0xc1] = 0x0;
  saa7114regs[0xc2] = 0x0;
  saa7114regs[0xc3] = 0x0;
  saa7114regs[0xc4] = 0x0;
  saa7114regs[0xc5] = 0x0;
  saa7114regs[0xc6] = 0x0;
  saa7114regs[0xc7] = 0x0;
  saa7114regs[0xc8] = 0x0;
  saa7114regs[0xc9] = 0x0;
  saa7114regs[0xca] = 0x0;
  saa7114regs[0xcb] = 0x0;
  saa7114regs[0xcc] = 0x0;
  saa7114regs[0xcd] = 0x0;
  saa7114regs[0xce] = 0x0;
  saa7114regs[0xcf] = 0x0;
  saa7114regs[0xd0] = 0x0;
  saa7114regs[0xd1] = 0x0;
  saa7114regs[0xd2] = 0x0;
  saa7114regs[0xd3] = 0x0;
  saa7114regs[0xd4] = 0x0;
  saa7114regs[0xd5] = 0x0;
  saa7114regs[0xd6] = 0x0;
  saa7114regs[0xd7] = 0x0;
  saa7114regs[0xd8] = 0x0;
  saa7114regs[0xd9] = 0x0;
  saa7114regs[0xda] = 0x0;
  saa7114regs[0xdb] = 0x0;
  saa7114regs[0xdc] = 0x0;
  saa7114regs[0xdd] = 0x0;
  saa7114regs[0xde] = 0x0;
  saa7114regs[0xdf] = 0x0;
  saa7114regs[0xe0] = 0x0;
  saa7114regs[0xe1] = 0x0;
  saa7114regs[0xe2] = 0x0;
  saa7114regs[0xe3] = 0x0;
  saa7114regs[0xe4] = 0x0;
  saa7114regs[0xe5] = 0x0;
  saa7114regs[0xe6] = 0x0;
  saa7114regs[0xe7] = 0x0;
  saa7114regs[0xe8] = 0x0;
  saa7114regs[0xe9] = 0x0;
  saa7114regs[0xea] = 0x0;
  saa7114regs[0xeb] = 0x0;
  saa7114regs[0xec] = 0x0;
  saa7114regs[0xed] = 0x0;
  saa7114regs[0xee] = 0x0;
  saa7114regs[0xef] = 0x0;
  saa7114regs[0xf0] = 0x0;
  saa7114regs[0xf1] = 0x0;
  saa7114regs[0xf2] = 0x0;
  saa7114regs[0xf3] = 0x0;
  saa7114regs[0xf4] = 0x0;
  saa7114regs[0xf5] = 0x0;
  saa7114regs[0xf6] = 0x0;
  saa7114regs[0xf7] = 0x0;
  saa7114regs[0xf8] = 0x0;
  saa7114regs[0xf9] = 0x0;
  saa7114regs[0xfa] = 0x0;
  saa7114regs[0xfb] = 0x0;
  saa7114regs[0xfc] = 0x0;
  saa7114regs[0xfd] = 0x0;
  saa7114regs[0xfe] = 0x0;

  // dont know why, but this order/dup seems important...
  SAA7114_WriteBlock(bdev, 0x90, 0xbf);
  SAA7114_WriteBlock(bdev, 0x80, 0x88);

  SAA7114_WriteBlock(bdev, 0x00, 0x19);
  SAA7114_WriteBlock(bdev, 0x30, 0x3a);
  SAA7114_WriteBlock(bdev, 0x40, 0x5f);
  SAA7114_WriteBlock(bdev, 0x80, 0x88);
  SAA7114_WriteBlock(bdev, 0x90, 0xbf);

  return 0;
}

// Read values from a registry file, experimental, needs fixes

#if 0
int
SAA7114_Init_File(dunedev_t *bdev, duneparams_t *params, const char *fname)
{
  char buf[512];
  uint32 reg, val;
  FILE *f;
  int res = -1;
  int saaflag = 0;
  char *p;

  memset(saa7114regs, 0, sizeof(saa7114regs));

  if((f = fopen(fname, "r")) == NULL)
    {
      perror(fname);
      return -1;
    }

  fgets(buf, sizeof(buf), f);
  if(strncmp(buf, "REGEDIT4", 8) != 0)
    {
      fprintf(stderr, "%s: Not a regedit file\n", fname);
      goto done;
    }

  while(fgets(buf, sizeof(buf), f))
    {
      if((p = strrchr(buf, '\r')) || (p = strrchr(buf, '\n')))
	*p = 0;

      if(strstr(buf, "HKEY_LOCAL_MACHINE"))
	{
	  if(strstr(buf, "SAA7114\\AudioClockGeneration"))
	    saaflag = 1;
	  else if(strstr(buf, "SAA7114\\DataSlicer"))
	    saaflag = 1;
	  else if(strstr(buf, "SAA7114\\FrontEndAndDecoder"))
	    saaflag = 1;
	  else if(strstr(buf, "SAA7114\\GlobalSettings"))
	    saaflag = 1;
	  else if(strstr(buf, "SAA7114\\ScalerA"))
	    saaflag = 1;
	  else
	    saaflag = 0;
	  continue;
	}

      if(saaflag)
	{
	  if(sscanf(buf, "\"Reg%x\"=dword:%x\n", &reg, &val) == 2)
	    {
	      saa7114regs[reg] = val;
	    }
	}
    }

  // global control, task, slicer defines timings for scaler output
  saa7114regs[0x80] = 0x50;

  // dont know why, but this order/dup seems important...
  SAA7114_WriteBlock(bdev, 0x90, 0xbf);
  SAA7114_WriteBlock(bdev, 0x80, 0x88);

  SAA7114_WriteBlock(bdev, 0x00, 0x19);
  SAA7114_WriteBlock(bdev, 0x30, 0x3a);
  SAA7114_WriteBlock(bdev, 0x40, 0x5f);
  SAA7114_WriteBlock(bdev, 0x80, 0x88);
  SAA7114_WriteBlock(bdev, 0x90, 0xbf);

  res = 0;

 done:
  fclose(f);

  return res;
}
#endif

int
SAA7114_SetBrightness(dunedev_t *bdev, uint32 n)
{
  saa7114regs[0x0A] = n;

  SAA7114_WriteBlock(bdev, 0x0A, 0x0A);

  return 0;
}

//////////////////////////////////////////////////////////////////////

int
DuneStartRecording(dunedev_t *bdev)
{
  int val = 0;

  if(debug || verbose)
    fprintf(stderr, "Start recording...\n");

  if(DuneKFirReadCmd(bdev, KFIR_ADDR_EREADY, &val) < 0)
    return -1;

  if(debug)
    fprintf(stderr, "Ready  = 0x%x\n", val);

  if((val & 0x01) != 0x01)
    {
      fprintf(stderr, "KFIR not ready (0x%x)\n", val);
      return -1;
    }

  if(DuneKFirReadCmd(bdev, KFIR_ADDR_RECORD, &val) < 0)
    return -1;

#if 0
  if(debug)
    fprintf(stderr, "Record = 0x%x\n", val);
#endif

  //////////////////////////////////////////////////////////////////////

  if(DuneKFirWriteCmd(bdev, KFIR_ADDR_RECORD, 0x01) < 0)
    return -1;

  if(DuneStartRecCmd(bdev) < 0)
    return -1;

  //////////////////////////////////////////////////////////////////////

#if 0
  // seems that there is no reaction until record stopped...
  if(DuneKFirReadCmd(bdev, KFIR_ADDR_EREADY, &val) < 0)
    return -1;

  if(debug)
    fprintf(stderr, "Ready  = 0x%x\n", val);

  if(DuneKFirReadCmd(bdev, KFIR_ADDR_RECORD, &val) < 0)
    return -1;

  if(debug)
    fprintf(stderr, "Record = 0x%x\n", val);
#endif

  return 0;
}

int
DuneStopRecording1(dunedev_t *bdev)
{
  if(debug || verbose)
    fprintf(stderr, "Stop recording1...\n");

  if(DuneStopRecBACmd(bdev) < 0)
    return -1;

  return 0;
}

int
DuneStopRecording2(dunedev_t *bdev)
{
  if(debug || verbose)
    fprintf(stderr, "Stop recording2...\n");

  if(DuneStopRecB9Cmd(bdev) < 0)
    return -1;

  // not used with new firmware!?
  if(DuneKFirWriteCmd(bdev, KFIR_ADDR_RECORD, 0x00) < 0)
    return -1;

  return 0;
}

//////////////////////////////////////////////////////////////////////
// left from bungee, dune will probably never have a decoder..

int
DuneStartPlaying(dunedev_t *bdev)
{
  if(debug || verbose)
    fprintf(stderr, "Start playing...\n");

  return -1;
}

int
DuneStopPlaying(dunedev_t *bdev)
{
  if(debug || verbose)
    fprintf(stderr, "Stop playing...\n");

  return -1;
}

int
DuneGetMPEGDecoderStatus(dunedev_t *bdev, uint32 *pvid, uint32 *paud)
{
  int i;
  uint32 buf[16];

  if(debug)
    fprintf(stderr, "GetMPEGDecoderStatus()...\n");

  return -1;
}

int
DuneSetAudioVolumeIn(dunedev_t *bdev, uint32 left, uint32 right)
{
  if(debug)
    fprintf(stderr, "SetAudioVolumeIn(%x, %x)\n", left, right);

  return -1;
}

int
DuneSetAudioStandard(dunedev_t *bdev, uint32 matrixmode, 
		       uint32 audiostandard)
{
  return -1;
}

int
DuneSetInputVideoFormat(dunedev_t *bdev, uint32 format)
{
  return -1;
}

int
DuneSetMPEGDecoderOutputVideoFormat(dunedev_t *bdev, uint32 format)
{
  return -1;
}

int
DuneSetEncoderOutputVideoFormat(dunedev_t *bdev, uint32 format)
{
  return -1;
}

int
DuneSetVideoSize(dunedev_t *bdev, uint32 w, uint32 h)
{
  return -1;
}

int
DuneSetupColors(dunedev_t *bdev, char *config)
{
  uint32 brightness, saturation, sharpness, contrast;

  if(sscanf(config, "%d,%d,%d,%d",
	    &brightness, &saturation, &sharpness, &contrast) != 4)
    {
      fprintf(stderr, "Invalid color config. Skipped.\n");
      return -1;
    }

  return -1;
}

//////////////////////////////////////////////////////////////////////

#if defined(LINUX) || defined(EZUSB)
int
DuneReadStream(dunedev_t *bdev, unsigned char *buf, int len)
{
  int timeout = 1000;
  int i;

  if((len = usb_bulk_read(bdev->usb, DUNE_EP82, buf, len, timeout)) < 0)
    {
      //fprintf(stderr, "ReadVideo: %s\n", usb_strerror());
      return -1;
    }

  if(debug > 2)
    fprintf(stderr, "Read %d bytes\n", len);

  return len;
}
#endif

#if defined(MACOS)
int
DuneReadStream(dunedev_t *bdev, unsigned char *buf, int len)
{
  int timeout = 1000;
  int readlen;
  int ret;

  ret = libusb_bulk_transfer(bdev->usb, DUNE_EP82, buf, len, &readlen, timeout);
  if(ret != 0) {
    printf("libusb_bulk_transfer error(%d)\n", ret);
    return 0;
  }
  
  if(debug > 2)
    fprintf(stderr, "Read %d bytes\n", readlen);

  return readlen;
}
#endif

#if defined(LINUX) || defined(EZUSB)
int
DuneWriteStream(dunedev_t *bdev, unsigned char *buf, int len)
{
  int timeout = 1000;

  if((len = usb_bulk_write(bdev->usb, DUNE_EP04, buf, len, timeout)) < 0)
    {
      fprintf(stderr, "WriteVideo: %s\n", usb_strerror());
      return -1;
    }

  return len;
}
#endif

#ifdef LINUX
int
DuneWriteStreamAsync(dunedev_t *bdev, unsigned char *buf, int len)
{
  int ret;

  if((ret = usb_bulk_write_async(bdev->usb, DUNE_EP04, buf, len)) != 0)
    {
      fprintf(stderr, "async_write failed: %d\n", ret);
      return -1;
    }
  if((ret = usb_bulk_wait_async(bdev->usb)) < 0)
    {
      fprintf(stderr, "async wait failed: %d\n", ret);
      return -1;
    }

  return len;
}

int
DuneWriteStreamAsyncNoWait(dunedev_t *bdev, unsigned char *buf, int len)
{
  int ret;

  if((ret = usb_bulk_write_async(bdev->usb, DUNE_EP04, buf, len)) != 0)
    {
      fprintf(stderr, "async_write failed: %d\n", ret);
      return ret;
    }

  return len;
}

int
DuneWriteStreamAsyncWaitForCompletion(dunedev_t *bdev)
{
  int ret;

  if((ret = usb_bulk_wait_async(bdev->usb)) < 0)
    {
      fprintf(stderr, "async wait failed: %d\n", ret);
      return ret;
    }

  return 0;
}

int
DuneWriteStreamAsyncWaitForCompletionNonBlock(dunedev_t *bdev)
{
  int ret;

  if((ret = usb_bulk_wait_async_nonblock(bdev->usb)) < 0)
    {
      return ret;
    }

  return 0;
}

#endif

#ifdef LINUX
int
DuneReadRemoteControl(dunedev_t *bdev, unsigned char *buf, int len)
{
  int i;
  int timeout = 10;

  // Works with first firmware, but strange codes.

  // The builtin ir seems to use interrupt transfers in new firmware,
  // we cannot do this with libusb.

  // Pinnacle seems to sell the box now with an external receiver
  // for a com port..

  if((len = usb_bulk_read(bdev->usb, DUNE_EP81, buf, len, timeout)) < 0)
    {
      // errno can be ETIMEDOUT
      return -1;
    }

  if(debug > 2)
    fprintf(stderr, "Read %d bytes\n", len);

  if(debug > 3)
    {
      if(len)
	{
	  for(i = 0; i < len; i++)
	    fprintf(stderr, "%02x ", buf[i]);
	  fprintf(stderr, "\n");
	}
    }

  return len;
}
#endif

#ifdef MACOS
int
DuneReadRemoteControl(dunedev_t *bdev, unsigned char *buf, int len)
{

  return 0;
}
#endif

//////////////////////////////////////////////////////////////////////

int
DuneLoadChannels()
{
  char path[256];
  FILE *f;
  char buf[256];
  char *p;

  nchannels = 0;

  sprintf(path, "%s/channels.txt", dunedir);

  if((f = fopen(path, "r")) == NULL)
    {
      return -1;
    }
  while((fgets(buf, sizeof(buf), f)) != NULL)
    {
      if(nchannels == MAXCHANNELS)
	break;

      if(buf[0] == '#')
	continue;
      if((p = strrchr(buf, '\n')) != NULL)
	*p = 0;
      if((p = strtok(buf, "\t ")) == NULL)
	continue;
      strcpy(dune_channels[nchannels].name, p);
      if((p = strtok(NULL, "\t ")) == NULL)
	continue;
      dune_channels[nchannels].freq = atol(p);
      nchannels++;
    }
  fclose(f);
  return 0;
}

int
DuneLoadIRCodes()
{
  char path[256];
  FILE *f;
  char buf[256];
  char *p;

  nircodes = 0;

  sprintf(path, "%s/ircodes.txt", dunedir);

  if((f = fopen(path, "r")) == NULL)
    {
      return -1;
    }
  while((fgets(buf, sizeof(buf), f)) != NULL)
    {
      if(nircodes == MAXIRCODES)
	break;

      if(buf[0] == '#')
	continue;
      if((p = strrchr(buf, '\n')) != NULL)
	*p = 0;

      if((p = strtok(buf, "\t ")) == NULL)
	continue;
      ircodes[nircodes].name = strdup(p);
      if((p = strtok(NULL, "\t ")) == NULL)
	goto done;
      sscanf(p, "%x", &ircodes[nircodes].single);
      if((p = strtok(NULL, "\t ")) == NULL)
	goto done;

      sscanf(p, "%x", &ircodes[nircodes].multi);

    done:
      nircodes++;
    }
  fclose(f);
  return 0;
}

int
DuneGetChannelFreq(const char *name)
{
  int i;

  for(i = 0; i < nchannels; i++)
    {
      if(stricmp(name, dune_channels[i].name) == 0)
	return dune_channels[i].freq;
    }

  fprintf(stderr, "Channel %s not found\n", name);
  return 0;
}

char *
DuneGetIRCodeName(uint32 code)
{
  int i;
  static char buf[126];

  for(i = 0; i < nircodes; i++)
    {
      if(ircodes[i].single == code ||
	 ircodes[i].multi == code)
	{
	  return ircodes[i].name;
	}

    }

  sprintf(buf, "Code %04x", code);

  return buf;
}

//////////////////////////////////////////////////////////////////////
// Code originally by Andreas Gal

// usb vendor requests for loading firmware

static int
ezusb_write(dunedev_t *bdev, int req, int val, uint8 *data, int len)
{
  int i;

#if 0
  fprintf(stderr, "req 0x%x val 0x%x len %d\n", req, val, len);
  for(i = 0; i < len; i++)
    {
      fprintf(stderr, "%02x ", data[i]);
    }
  fprintf(stderr, "\n");
#endif

  return vendorrequest_write(bdev, req, val, data, len);
}

static int
ezusb_read(dunedev_t *bdev, int req, int val, uint8 *data, int len)
{
  return vendorrequest_read(bdev, req, val, data, len);
}

static int 
ezusb_write_onchip(dunedev_t *bdev, unsigned address, uint8 *data, int len)
{
  return ezusb_write(bdev, 0xa0, address, data, len);
}

// not used with fx2 chip...

static int 
ezusb_write_extern(dunedev_t *bdev, unsigned address, uint8 *data, int len)
{
  return ezusb_write(bdev, 0xa3, address, data, len);
}

// set EZUSB reset (halts CPU and enables hardware loader)

static int 
ezusb2_reset(dunedev_t *bdev, uint8 reset)
{ 
  return ezusb_write(bdev, 0xa0, 0xe600, &reset, 1);
}

// send firmware between address min and max to device (directly
// reading Pinnacle's U2SB.sys format)

static int 
ezusb_send_firmware(dunedev_t *bdev, uint8* data, int count, 
		    unsigned min, unsigned max)
{
  while (count > 0) {
    unsigned size = (((unsigned)(data[1])) << 8) | data[0];
    unsigned addr = (((unsigned)(data[3])) << 8) | data[2];

    if ((addr >= min) && (addr <= max)) {
      if(ezusb_write_onchip(bdev, addr, data + 5, size) < 0)
	return -1;
    }

    count -= 22;
    data += 22;
  }

  return 0;
}

// send a patch in srecord format

int
send_patch(dunedev_t *bdev, srecord_t *srec)
{
  int i;

  for(i = 0; srec[i].type != 1; i++)
    {
      ezusb_write_extern(bdev, srec[i].addr, srec[i].bytes, srec[i].len);
    }

  return 0;
}

//////////////////////////////////////////////////////////////////////
// New firmware loader, based on Andreas Gal's version.
// We scan for the address of the first part because
// the original version did not work with beta2 fw
// as there is more padding.

int
DuneLoadFirmwareEZUSB2(dunedev_t *bdev, char *u2sb, int load)
{
  uint8* fw;
  uint8* fw1;
  int nfw1;

  uint8* p;
  int size, len;
  int count;
  struct stat st;
  int fd;
  char path[256];

  if(u2sb == NULL)
    u2sb = "dune.sys";

  sprintf(path, "%s/%s", dunedir, u2sb);
  
  if(stat(path, &st) < 0)
    {
      fprintf(stderr, "%s: %s\n", path, strerror(errno));
      return -1;
    }

  count = st.st_size;
  fw = malloc(count);

  if((fd = open(path, O_RDONLY|O_BINARY)) < 0)
    {
      fprintf(stderr, "%s: %s\n", strerror(errno));
      return -1;
    }

  read(fd, fw, count);

  close(fd);

  //////////////////////////////////////////////////////////////////////
  // scan for start...

  p = fw;
  size = count;

  while (size > 0)
    {
      // 0.0.3.48
      if(*(uint32 *) p == 0x17b7000a)
	{
	  dune_ezusb2_version = 1;
	  break;
	}

      // 1.0.3.105
      if(*(uint32 *) p == 0x1972000a)
	{
	  dune_ezusb2_version = 2;
	  break;
	}
      // PcMv5U2.sys Ver.1.31
      if(*(uint32 *) p == 0x1d30000b)
	{
	  dune_ezusb2_version = 2;
	  break;
	}

      size -= 4;
      p += 4;
    }

  if(size <= 0)
    {
      fprintf(stderr, "Start of FW not found.\n");
      return -1;
    }

  fw1 = p;

  for (len = 0; (size > 0) && (!p[4]); len += 22) 
    {
      size -= 22;
      p += 22;
    }

  nfw1 = len;
        
  if (size <= 0 || nfw1 == 0) 
    {
      fprintf(stderr, "FW not found in %s\n", path);
      free(fw);
      return -1;
    }

  if(debug)
    {
      fprintf(stderr, "%s: FWOff: %d FWLen: %d\n",
	      u2sb, fw1 - fw, nfw1);
    }

  if(load)
    {
      if(ezusb2_reset(bdev, 1) < 0)
	return -1;
      if(ezusb_send_firmware(bdev, fw1, nfw1, 0x0000, 0x1fff) < 0)
	return -1;
      if(ezusb2_reset(bdev, 0) < 0)
	return -1;
    }

  free(fw);

  return 0;
}

//////////////////////////////////////////////////////////////////////
// more speed when writing data to the mpeg decoder, needed
// to play dvd with lpcm audio on my 333 mhz vcr...

#ifdef LINUX
static struct usb_urb urb;

int 
usb_bulk_write_async(usb_dev_handle *dev, int ep, char *bytes, int length)
{
  // must not be on stack, the completion routine copies data to it...
  int ret;

  if (length > 0x4000) // 16384
    return -EINVAL;

  ep &= ~USB_ENDPOINT_IN;

  memset(&urb, 0, sizeof(urb));
  urb.type = USB_URB_TYPE_BULK;
  urb.endpoint = ep;
  // any flags for more speed?
  //urb.flags = 0;
  urb.buffer = (unsigned char *) bytes;
  urb.buffer_length = length;
  //urb.signr = 0;

  // filled in on completion
  //urb.status = 0;
  //urb.error_count = 0;
  //urb.actual_length = 0;

  if((ret = ioctl(dev->fd, USB_SUBMITURB, &urb)) < 0)
    return ret;

  return 0;
}

int 
usb_bulk_wait_async(usb_dev_handle *dev)
{
  int ret;
  // this byte is copied to the first byte of a finished urb on
  // successfull completion...
  unsigned int arg = 0;

  if((ret = ioctl(dev->fd, USB_REAPURB, &arg)) < 0)
    return ret;

  if(urb.actual_length != urb.buffer_length)
    return -EIO;

  return 0;
}

int 
usb_bulk_wait_async_nonblock(usb_dev_handle *dev)
{
  int ret;
  // this byte is copied to the first byte of a finished urb on
  // successfull completion...
  unsigned int arg = 0;

  // -EPERM?
  if((ret = ioctl(dev->fd, USB_REAPURBNDELAY, &arg)) < 0)
    return ret;

  if(urb.actual_length != urb.buffer_length)
    return -EIO;

  return 0;
}
#endif

//////////////////////////////////////////////////////////////////////

int
DuneI2CRead(dunedev_t *bdev, uint32 addr, uint32 subaddr, 
	    int usesub, int nbytes, uint8 *buf)
{
  DuneI2CReadCmd(bdev, addr, subaddr, usesub, nbytes, buf);
  return 1;
}

int
DuneI2CWrite(dunedev_t *bdev, uint32 addr, uint32 subaddr, 
	     int usesub, int nbytes, uint8 *buf)
{
  return DuneI2CWriteCmd(bdev, addr, subaddr, usesub, nbytes, buf);
}

//////////////////////////////////////////////////////////////////////

int
DuneDumpFirmware(dunedev_t *bdev, int start, int end, char *fname)
{
  int addr;
  unsigned char buf[0x10];
  int fd;

  memset(buf, 0, sizeof(buf));

  if((fd = open(fname, O_BINARY|O_CREAT|O_TRUNC|O_WRONLY, 0666)) < 0)
    return -1;

  for(addr = start; addr < end; addr += 0x10)
    {
      if(ezusb_read(bdev, 0xA0, addr, buf, 0x10) < 0)
	break;

      write(fd, buf, 0x10);
    }

  close(fd);

  return 0;
}

int
DuneKFirWriteCmd(dunedev_t *ddev, int reg, int val)
{
  int res;
  unsigned short sval = ntohs(val);

  res = vendorrequest_write(ddev, 0xb2, reg, (uint8 *) &sval, 2);
  //usleep(10);

  return res;
}

int
DuneKFirReadCmd(dunedev_t *ddev, int reg, int *pval)
{
  int res;
  unsigned short sval = 0;

  res = vendorrequest_read(ddev, 0xb3, reg, (uint8 *) &sval, 2);
  *pval = htons(sval);

  return res;
}

int
DuneStartRecCmd(dunedev_t *ddev)
{
  int res;
  uint8 dummy[1];

  res = vendorrequest_write(ddev, 0xb8, 0, dummy, 0);

  return res;
}

int
DuneStopRecB9Cmd(dunedev_t *ddev)
{
  int res;
  uint8 dummy[1];

  res = vendorrequest_write(ddev, 0xb9, 0, dummy, 0);

  return res;
}

int
DuneStopRecBACmd(dunedev_t *ddev)
{
  int res;
  uint8 dummy[1];

  res = vendorrequest_write(ddev, 0xba, 0, dummy, 0);

  return res;
}

int
DuneI2CWriteCmd(dunedev_t *bdev, uint32 addr, uint32 subaddr, 
		int usesub, int nbytes, uint8 *buf)
{
  int res;
  uint8 *buf2 = alloca(nbytes+1);

  if(addr == MSPI2CADDR)
    addr |= 0x200;
  else if(addr == SAAI2CADDR || addr == TDAI2CADDR)
    addr |= 0x100;

  if(usesub)
    {
      buf2[0] = subaddr;
      memcpy(buf2+1, buf, nbytes);
      nbytes++;
    }
  else
    {
      memcpy(buf2, buf, nbytes);
    }

  // msp read needs index = 1 -> no stop condition
  if(addr == MSPI2CADDR && (buf2[0] == 0x11 || buf2[0] == 0x13))
    res = vendorrequest_write2(bdev, 0xb0, addr, 1, buf2, nbytes);
  else
    res = vendorrequest_write2(bdev, 0xb0, addr, 0, buf2, nbytes);

  return res;
}

int
DuneI2CReadCmd(dunedev_t *bdev, uint32 addr, uint32 subaddr, 
	       int usesub, int nbytes, uint8 *buf)
{
  int res;

//  if(usesub)
//    addr |= subaddr << 8;
  if(addr == SAAI2CADDR)
    addr |= 0x100;

  res = vendorrequest_read(bdev, 0xb1, addr, buf, nbytes);

  return res;
}

// vendor request value/index pairs for ucode upload

struct {
  int value;
  int index;
} microparams[] = {
  // first is probably dev, second is inst
{0x00008007,0x00000003},
{0x00000007,0x00000003},
{0x00000007,0x00000003},
{0x00000007,0x00000003},
{0x00008004,0x00000003},
{0x00008004,0x00000005},
{0x00008004,0x00000007},
{0x00008005,0x00000003},
{0x00008008,0x00000003},
{0x00000008,0x00000003},
{0x00000008,0x00000003},
{0x00008009,0x00000003},
{0x00000009,0x00000003},
{0x00000009,0x00000003},
{0x00000009,0x00000003},
{0x00008009,0x00000005},
{0x00000009,0x00000005},
{0x00000009,0x00000005},
{0x00000009,0x00000005},
{0x00000009,0x00000005},
{0x00000009,0x00000005},
{0x00008005,0x00000005},
{0x00008004,0x00000009}
};
  
int
DuneSendFirmwareChunk(dunedev_t *bdev, int n, unsigned char *buf, int len)
{
  int res;
  int timeout = 3000;

  int val = microparams[n].value;
  int ix  = microparams[n].index;

  if(debug > 2)
    {
      int i, j;

      fprintf(stderr, "Chunk %d, val 0x%x, idx 0x%x, len %d\n", 
	      n, val, ix, len);

      for(i = 0; i < len; i += 16)
	{
	  fprintf(stderr, "    %08x: ", i);
	  for(j = 0; j < 16; j++)
	    fprintf(stderr, "%02x ", buf[i+j]);
	  fprintf(stderr, "\n");
	}
    }

  res = vendorrequest_write2(bdev, 0xb4, val, ix, buf, len);

  return res;
}

static int 
numhexdigits(const char *s)
{
  int n;
  
  for(n=0;*s;s++)
    {
      if(isdigit(*s) || (tolower(*s) >= 'a' && tolower(*s) <= 'f'))
	n++;
    }
  return n;
}

int
DuneLoadMicrocode(dunedev_t *bdev, const char *fware)
{
  struct stat st;
  unsigned char *buf, *p;
  int len;
  int cc;
  char fname[256];
  int nfirmware = 0x02;
  FILE *f;
  char line[126];
  int nchunk = 0;
  int kval;
  int i;
  int total = 0;
  int marker_seen = 0;
  uint32 uversion = 0;

  if(fware != NULL)
    {
      sprintf(fname, "%s/%s", dunedir, fware);
    }
  else
    {
      sprintf(fname, "%s/k2.v", dunedir);

      if((p = getenv("DUNE_FIRMWARE")) != NULL)
	strcpy(fname, p);
    }

  if(debug || verbose)
    fprintf(stderr, "Uploading KFIR microcode from %s...\n", fname);


  if(stat(fname, &st) < 0)
    {
      perror(fname);
      return -1;
    }

  if((buf = malloc(4096)) == NULL)
    return -1;

  if((f = fopen(fname, "r")) == NULL)
    return -1;

  DuneEraseMicrocodeCmd(bdev);

  DuneKFirWriteCmd(bdev, 0xc8, 0x0706);
  DuneKFirReadCmd(bdev, 0xc9, &kval);
  DuneKFirWriteCmd(bdev, 0xc8, 0x0705);
  DuneKFirReadCmd(bdev, 0xc9, &kval);

  p = buf;
  len = 0;

  while(fgets(line, sizeof(line), f))
    {
      char *s;
      uint32 ui32;
      uint64 ui64;

      if((s = strchr(line, '\n')) != NULL)
	*s = 0;
      if((s = strchr(line, '\r')) != NULL)
	*s = 0;

      if(line[0] == '*')
	{
	  char *s;

	  if((s = strstr(line, "Version: ")) != NULL)
	    {
	      dune_ucode_version = strdup(s + 9);
	      if((s = strchr(dune_ucode_version, ' ')))
		*s = 0;
	    }
	}

      // some empty lines have spaces...
      if(line[0] == '*' || line[0] == 0 || line[0] == ' ')
	{
	  continue;
	}

      if(line[0] == '#')
	marker_seen = 1;

      // there might be lines without comment char at the top...
      if(!marker_seen)
	continue;

      if(line[0] == '#')
	{
	  if(len)
	    {
	      DuneSendFirmwareChunk(bdev, nchunk++, buf, len);
	      total += len;
	    }

	  p = buf;
	  len = 0;
	  continue;
	}

      // strange encoding - and this decoder is stupid...but works

      // the last chunk...
      if(numhexdigits(line) == 17)
	{
	  sscanf(line, "%1x", &ui32);

	  ui32 = htonl(ui32);

	  *(uint16 *) p = htons(ui32 & 0xFFFF);
	  p += 2;
	  *(uint16 *) p = htons((ui32 >> 16) & 0xFFFF);
	  p += 2;

	  sscanf(line+1, "%8x", &ui32);

	  ui32 = htonl(ui32);

	  *(uint16 *) p = htons(ui32 & 0xFFFF);
	  p += 2;
	  *(uint16 *) p = htons((ui32 >> 16) & 0xFFFF);
	  p += 2;

	  sscanf(line+9, "%8x", &ui32);

	  ui32 = htonl(ui32);

	  *(uint16 *) p = htons(ui32 & 0xFFFF);
	  p += 2;
	  *(uint16 *) p = htons((ui32 >> 16) & 0xFFFF);
	  p += 2;

	  len += 12;
	}
      else
	{
	  sscanf(line, "%x", &ui32);

	  ui32 = htonl(ui32);

	  *(uint16 *) p = htons(ui32 & 0xFFFF);
	  p += 2;
	  *(uint16 *) p = htons((ui32 >> 16) & 0xFFFF);
	  p += 2;

	  len += 4;
	}

      if(len >= 4096)
	{
	  DuneSendFirmwareChunk(bdev, nchunk++, buf, len);
	  total += len;
	  p = buf;
	  len = 0;
	}
    }

  if(len)
    {
      DuneSendFirmwareChunk(bdev, nchunk++, buf, len);
      total += len;
    }

  fclose(f);

  free(buf);

  //fprintf(stderr, "Loaded %d bytes ucode\n", total);

  DuneKFirWriteCmd(bdev, 0xc8, 0x0700);
  DuneKFirWriteCmd(bdev, 0xc9, 0x0000);
  DuneKFirWriteCmd(bdev, 0xc8, 0x0705);
  DuneKFirReadCmd(bdev, 0xc9, &kval);
  
  for(i = 0; i < 10; i++)
    {
      // when there was an error, 0xfe is returned
      DuneKFirReadCmd(bdev, 0xfe, &kval);
      if(kval == 0x01)
	break;
    }

  if(kval == 0xfe)
    {
      fprintf(stderr, "Error loading microcode (0x%x)\n", kval);
      return -1;
    }

  if(kval != 0x01)
    {
      fprintf(stderr, "Not ready after microcode upload (%x)\n", kval);
    }
  else if(verbose)
    {
      fprintf(stderr, "Microcode upload ok\n");
    }

  if(debug || verbose)
    fprintf(stderr, "Uploading KFIR microcode...done\n");

  if(debug || verbose)
    {
      // new driver, ucode version!?
      DuneKFirWriteCmd(bdev, 0xc8, 0x08be);
      DuneKFirReadCmd(bdev, 0xc9, &kval);
      uversion = kval;

      DuneKFirWriteCmd(bdev, 0xc8, 0x08bf);
      DuneKFirReadCmd(bdev, 0xc9, &kval);
      uversion |= kval << 16;

      fprintf(stderr, "Chip reports ucode version %d.%d\n", 
	      uversion >> 16, uversion & 0xFFFF);
    }

  return 0;
}

int
DuneSetFrequency(dunedev_t *bdev, uint32 freq)
{
  uint32 freq1 = 0;
  uint32 freq2 = 0;
  uint8 i2c[5];
  uint32 dwParams[1];
  uint8 band = 0;

  freq1 = freq;

  if(bdev->params.videostandard == DUNE_PAL)
    {
      freq2 = 389000; // picture carrier pal
    }
  else if(bdev->params.videostandard == DUNE_NTSC)
    {
      freq2 = 457500; // picture carrier ntsc
    }
  else if(bdev->params.videostandard == DUNE_NTSC_JAPAN)
    {
      freq2 = 587500; // picture carrier ntsc_japan
    }
  else
    {
      fprintf(stderr, "Video standard not set\n");
      return -1;
    }

  freq2 /= 10;
  freq2 += freq1; // KHz
  freq2 *= 10;
  freq2 /= 625;

  if(debug > 1)
    fprintf(stderr, "SetFrequency(%d)\n", freq1);

  if(bdev->params.videostandard == DUNE_PAL)
    {
      if(freq1 < 170000)
	band = 0x01;
      else if(freq1 > 170000 && freq1 < 450000)
	band = 0x02;
      else
	band = 0x04;
    }
  else 
    {
      if(freq1 < 160000)
	band = 0x01;
      else if(freq1 > 160000 && freq1 < 442000)
	band = 0x02;
      else
	band = 0x04;
    }

  if(debug > 1)
    fprintf(stderr, "SetFrequency()\n");

  i2c[0] = (freq2 >> 8) & 0xFF;
  i2c[1] = (freq2 >> 0) & 0xFF;
  i2c[2] = 0x8e;
  i2c[3] = band;
  // The TDA6500 has this byte, but it *replaces* the band
  // switch byte when test bits are 011.
  i2c[4] = 0xA0; // unused?! explicitly set in new driver!

  DuneI2CWriteCmd(bdev, TUNERI2CADDR, 0, 0, 5, i2c);

  // needed?
  usleep(1000);

  MSP3400_Init(bdev);

  return 0;
}

#if 0
int 
DuneRunCommandTable(dunedev_t *bdev, vendor_cmd_t *cmdtab)
{
  int i;

  for(i = 0; cmdtab[i].num != -1; i++)
    {
      switch(cmdtab[i].request)
	{
	case DUNE_I2CWRITE:
	case DUNE_KFIRWRITE:
	case DUNE_B7:
	case DUNE_B8:
	case DUNE_B9:
	case DUNE_BA:
	case DUNE_BC:
	  vendorrequest_write(bdev, cmdtab[i].request, cmdtab[i].value,
			      cmdtab[i].bytes, cmdtab[i].len);
	  break;
	case DUNE_I2CREAD:
	case DUNE_KFIRREAD:
	  vendorrequest_read(bdev, cmdtab[i].request, cmdtab[i].value,
			     cmdtab[i].bytes, cmdtab[i].len);
	  break;
	default:
	  fprintf(stderr, "Unhandled vendor command: %x\n",
		  cmdtab[i].request);
	  break;
	}
    }

  return 0;
}
#endif

int
DuneSendMuxTable(dunedev_t *ddev, uint8 *data)
{
  int res;

  res = vendorrequest_write(ddev, 0xb5, 0, data, 0x1000);

  return res;
}

int
DuneEraseMicrocodeCmd(dunedev_t *ddev)
{
  int res;
  uint8 dummy[1];

  res = vendorrequest_write(ddev, 0xb7, 0, dummy, 0);

  return res;
}

// sent before usb chip fw upload, not needed?!

int
DuneSendBC(dunedev_t *ddev)
{
  int res;
  uint8 dummy[1];

  res = vendorrequest_write(ddev, 0xbc, 0, dummy, 0);

  return res;
}

// Only in new driver, not needed

int
DuneSendBD(dunedev_t *ddev)
{
  int res;
  uint8 dummy[1];

  res = vendorrequest_write(ddev, 0xbc, 0, dummy, 0);

  return res;
}

int
DuneReadEPROM(dunedev_t *ddev, uint8 *data, int len)
{
  int res;

  if(len != 512)
    return -1;

  res = vendorrequest_read(ddev, 0xb1, 0xa0, data, len);

  return res;
}

int
DuneShowKFirRegs(dunedev_t *bdev)
{
  int kv = 0;
  int bitrate = 0;
  long long ll;
  int frames_per_1000;
  int intra_pict_dist = 0;
  int m, n;

  DuneKFirReadCmd(bdev, 0, &kv);
  fprintf(stderr, "AspectRatio:       %d", kv);
  if(kv == 1)
    fprintf(stderr, " square\n");
  else if(kv == 2)
    fprintf(stderr, " 4:3\n");
  else if(kv == 3)
    fprintf(stderr, " 16:9\n");
  else
    fprintf(stderr, "\n");

  DuneKFirReadCmd(bdev, 1, &kv);
  fprintf(stderr, "FrameRateCode:     %d", kv);
  if(kv == 3)
    fprintf(stderr, " 25.00\n"), frames_per_1000 = 25000;
  else if(kv == 4)
    fprintf(stderr, " 29.97\n"), frames_per_1000 = 29970;
  else if(kv == 5)
    fprintf(stderr, " 30.00\n"), frames_per_1000 = 30000;
  else
    fprintf(stderr, "\n");

  bitrate = 0;
  DuneKFirReadCmd(bdev, 5, &kv);
  bitrate |= kv;
  DuneKFirReadCmd(bdev, 6, &kv);
  bitrate |= kv << 8;
  DuneKFirReadCmd(bdev, 7, &kv);
  bitrate |= kv << 16;
  fprintf(stderr, "Bitrate:           %d\n", bitrate*400);

  DuneKFirReadCmd(bdev, 0x31, &m);
  DuneKFirReadCmd(bdev, 0x32, &n);

  intra_pict_dist = m * n;

  ll = 0;
  DuneKFirReadCmd(bdev, 0x190, &kv);
  ll |= kv;
  DuneKFirReadCmd(bdev, 0x191, &kv);
  ll |= kv << 16;

  ll = (ll * 4 * frames_per_1000) / (intra_pict_dist * 1000);
  fprintf(stderr, "PeakBitrate:       %d\n", (int) ll);

  //DuneKFirReadCmd(bdev, 9, &kv);
  //fprintf(stderr, "ProgressiveSeq:    %d\n", kv);
  //DuneKFirReadCmd(bdev, 10, &kv);
  //fprintf(stderr, "ProgressiveFrame:  %d\n", kv);

  DuneKFirReadCmd(bdev, 34, &kv);
  fprintf(stderr, "MacroBlockWidth:   %d -> %d\n", kv, kv*16);
  DuneKFirReadCmd(bdev, 52, &kv);
  fprintf(stderr, "MacroBlockLines:   %d -> %d\n", kv, kv*16);

  DuneKFirReadCmd(bdev, 47, &kv);
  fprintf(stderr, "VideoFormat:       %d", kv);
  if(kv == 0)
    fprintf(stderr, " ntsc\n");
  else
    fprintf(stderr, " pal\n");

  DuneKFirReadCmd(bdev, 48, &kv);
  fprintf(stderr, "Interlace/Progr:   %d", kv);
  if(kv == 0)
    fprintf(stderr, " interlace\n");
  else
    fprintf(stderr, " progressive\n");

  DuneKFirReadCmd(bdev, 54, &kv);
  fprintf(stderr, "VBR:               %d", kv);
  if(kv == 0)
    fprintf(stderr, " constant\n");
  else
    fprintf(stderr, " variable\n");

  DuneKFirReadCmd(bdev, 55, &kv);
  fprintf(stderr, "FirstField:        %d", kv);
  if(kv == 0)
    fprintf(stderr, " bottom field first\n");
  else
    fprintf(stderr, " top field first\n");

  DuneKFirReadCmd(bdev, 59, &kv);
  fprintf(stderr, "3/2 PulldownDet:   %d\n", kv);

  DuneKFirReadCmd(bdev, 63, &kv);
  fprintf(stderr, "DecimationPatt:    %d\n", kv);
  DuneKFirReadCmd(bdev, 64, &kv);
  fprintf(stderr, "DecimationSize:    %d\n", kv);

  DuneKFirReadCmd(bdev, 65, &kv);
  fprintf(stderr, "VideoParams:       %x", kv);
  if(kv & 0x40)
    fprintf(stderr, " filemode");
  else
    fprintf(stderr, " livemode");
  if(kv & 0x100)
    fprintf(stderr, " pal");
  else
    fprintf(stderr, " ntsc");
  fprintf(stderr, "\n");

  DuneKFirReadCmd(bdev, 66, &kv);
  fprintf(stderr, "HOffset:           %x\n", kv);

  DuneKFirReadCmd(bdev, 67, &kv);
  fprintf(stderr, "VOffset:           %x\n", kv);

  return 0;
}

int
DuneDumpKFirRegs(dunedev_t *bdev)
{
  int i;
  int val;

  fprintf(stderr, "KFir2Regs:\n");
  for(i = 0; i < 256; i++)
    {
      DuneKFirReadCmd(bdev, i, &val);
      fprintf(stderr, "kfir[0x%02x] = 0x%04x;", i, val);
      fprintf(stderr, "\n");
    }
  return 0;
} 

int
DuneShowMuxTable(dunedev_t *bdev, uint8 *data)
{
  unsigned long *pl = (unsigned long *) data;
  unsigned long n1, n2;

  n1 = ntohs(ntohl(pl[MUX_AUDIO_ADR_STREAM_TYPE]));
  fprintf(stderr, "StreamType:        %x\n", n1);
  n1 = ntohs(ntohl(pl[MUX_AUDIO_ADR_AUDIO_STREAM_TYPE]));
  fprintf(stderr, "AudioStreamType:   %x\n", n1);

  n1 = ntohs(ntohl(pl[MUX_AUDIO_ADR_VIDEO_BITRATE_LOW]));
  n2 = ntohs(ntohl(pl[MUX_AUDIO_ADR_VIDEO_BITRATE_HIGH]));
  fprintf(stderr, "VideoBitRate:      %d\n", n1 | n2 << 16);

  n1 = ntohs(ntohl(pl[MUX_AUDIO_ADR_AUDIO_BITRATE_LOW]));
  n2 = ntohs(ntohl(pl[MUX_AUDIO_ADR_AUDIO_BITRATE_HIGH]));
  fprintf(stderr, "AudioBitRate:      %d\n", n1 | n2 << 16);

  n1 = ntohs(ntohl(pl[MUX_AUDIO_ADR_ES_RATE_LOW]));
  n2 = ntohs(ntohl(pl[MUX_AUDIO_ADR_ES_RATE_HIGH]));
  fprintf(stderr, "ESRate:            %d\n", (n1 | n2 << 16) * 400);

  n1 = ntohs(ntohl(pl[MUX_AUDIO_ADR_PROGRAM_MUX_RATE_LOW]));
  n2 = ntohs(ntohl(pl[MUX_AUDIO_ADR_PROGRAM_MUX_RATE_HIGH]));
  fprintf(stderr, "ProgMuxRate:       %d\n", (n1 | n2 << 16) * 400);

  n1 = ntohs(ntohl(pl[MUX_AUDIO_ADR_FRAMES_PER_100_SEC]));
  fprintf(stderr, "Frames/100s:       %d\n", n1);

  n1 = ntohs(ntohl(pl[MUX_AUDIO_ADR_SECTORSIZE]));
  fprintf(stderr, "SectorSize:        %d\n", n1);

  n1 = ntohs(ntohl(pl[MUX_AUDIO_ADR_VARIABLE_BITRATE]));
  fprintf(stderr, "VarBitrateX:       %d\n", n1);

  return 0;
}

int
DuneDumpMuxTable(dunedev_t *bdev, uint8 *data)
{
  unsigned long *pl = (unsigned long *) data;
  unsigned long n1;
  int i;

  fprintf(stderr, "Mux/AudioTable:\n");
  for(i = 0; i < 250; i++)
    {
      n1 = ntohs(ntohl(pl[i]));
      fprintf(stderr, "mux[%d] = 0x%08x\n", i, n1);
    }
  return 0;
}

int
DuneDumpSAA7114Regs(dunedev_t *bdev)
{
  int i;
  uint8 i2c[1];

  fprintf(stderr, "SAA7114Regs:\n");
  for(i = 0; i < 0xEF; i++)
    {
      DuneI2CReadCmd(bdev, SAAI2CADDR, i, 1, 1, i2c);
      fprintf(stderr, "saa7114[0x%02x] = 0x%02x;\n", i, i2c[0]);
    }
  return 0;
}

int
DuneResetKFirRegs(dunedev_t *bdev)
{
  int i;
  int val;

  for(i = 0; i < 256; i++)
    {
      DuneKFirWriteCmd(bdev, i, 0x0000);
    }
  return 0;
}

int
DuneKFirPreLoadParams(dunedev_t *bdev)
{
  int kval;
  int i;

  DuneKFirWriteCmd(bdev, 0xc8, 0x0700);
  DuneKFirWriteCmd(bdev, 0xc9, 0x0000);
  DuneKFirWriteCmd(bdev, 0xc8, 0x0705);
  DuneKFirReadCmd(bdev, 0xc9, &kval);

  for(i = 0; i < 10; i++)
    {
      DuneKFirReadCmd(bdev, 0xfe, &kval);
      if(kval == 0x01 || kval == 0xfe)
	break;
    }

  if(kval != 0x01)
    {
      fprintf(stderr, "Not ready for params (0x%x)\n", kval);
    }

  return 0;
}

int
KFir_Init_X(dunedev_t *bdev)
{
  // i don't know what difference this makes... vbi?!
  if(bdev->params.teletext)
    {
      DuneKFirWriteCmd(bdev, 0x006e, 0x0006);
      DuneKFirWriteCmd(bdev, 0x014f, 0x0002);
    }
  else
    {
      DuneKFirWriteCmd(bdev, 0x006e, 0x0000);
      DuneKFirWriteCmd(bdev, 0x014f, 0x0000);
    }

  // video vertical offset
  DuneKFirWriteCmd(bdev, 0x0043, 0x0000);

  return 0;
}

// for MPEG2.02.26 

int
KFir_Init_Filters2_None(dunedev_t *bdev)
{
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000108);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000109);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000010a);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000010b);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x000003ff);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000010c);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000010d);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000010e);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000010f);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000110);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000111);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000112);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x000003ff);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000113);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000114);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000115);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000116);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000117);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000118);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000119);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x000003ff);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000011a);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000011b);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000011c);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000011d);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000011e);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000011f);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000120);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x000003ff);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000121);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000122);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000123);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000124);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000125);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000126);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000127);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x000003ff);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000128);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000129);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000012a);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000012b);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000012c);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000012d);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000012e);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x000003ff);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000012f);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000130);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000131);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);

  return 0; 
}

// 4 MHz horizontal chroma
// 7.5 MHz horizontal and vertical luma

int
KFir_Init_Filters2_75(dunedev_t *bdev)
{
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000108);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000008);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000109);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x0000ffb0);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000010a);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x000000ff);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000010b);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000292);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000010c);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x000000ff);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000010d);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x0000ffb0);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000010e);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000008);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000010f);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x0000fff5);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000110);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x0000006a);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000111);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x000000ff);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000112);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000144);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000113);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x000000ff);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000114);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x0000006a);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000115);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x0000fff5);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000116);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000008);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000117);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x0000ffb0);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000118);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x000000ff);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000119);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000292);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000011a);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x000000ff);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000011b);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x0000ffb0);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000011c);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000008);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000011d);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000011e);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000011f);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000120);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x000003ff);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000121);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000122);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000123);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000124);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000008);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000125);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x0000ffb0);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000126);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x000000ff);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000127);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000292);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000128);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x000000ff);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000129);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x0000ffb0);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000012a);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000008);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000012b);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000012c);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000012d);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000012e);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x000003ff);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x0000012f);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000130);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);
  DuneKFirWriteCmd(bdev, 0x000000c8, 0x00000131);
  DuneKFirWriteCmd(bdev, 0x000000c9, 0x00000000);

  return 0;
}

int
KFir_Init_Filters2(dunedev_t *bdev)
{
  if(bdev->params.kfir_filter == 0)
    KFir_Init_Filters2_None(bdev);
  else if(bdev->params.kfir_filter == 75)
    KFir_Init_Filters2_75(bdev);
  else
    fprintf(stderr, "WARNING: Unsupported value for kfir_filter\n");

  return 0;
}

#if 0
// unused, do complete init...
int
DuneSetVideoBitrate(dunedev_t *bdev, uint32 rate)
{
  rate /= 400;

  DuneKFirWriteCmd(bdev, KFIR_ADR_BIT_RATE_0, (rate >> 0) & 0xFF);
  DuneKFirWriteCmd(bdev, KFIR_ADR_BIT_RATE_1, (rate >> 8) & 0xFF);
  DuneKFirWriteCmd(bdev, KFIR_ADR_BIT_RATE_2, (rate >> 16) & 0xFF);

  return 0;
}
#endif

#if 0
// unused, do complete init...
int
DuneSetAudioSampleFreq(dunedev_t *bdev, uint32 freq)
{
  // would have to change saa audio clock regs also,
  // and mux tables...
  return DuneKFirWriteCmd(bdev, KFIR_ADR_AUDIO_SAMPLEFREQ, freq);
}
#endif

#if 0
// unused, do complete init...
int
DuneSetInputSource(dunedev_t *bdev, uint32 inputsource)
{
  if(inputsource == DUNE_COMPOSITE)
    SAA7114_WriteReg(bdev, 0x02, 0xc0);
  else if(inputsource == DUNE_SVIDEO)
    SAA7114_WriteReg(bdev, 0x02, 0xc7);
  else if(inputsource == DUNE_TUNER)
    SAA7114_WriteReg(bdev, 0x02, 0xc5);

  if(inputsource == DUNE_TUNER)
    SAA7114_WriteReg(bdev, 0x03, 0x20);
  else
    SAA7114_WriteReg(bdev, 0x03, 0x24);

  if(inputsource == DUNE_TUNER)
    SAA7114_WriteReg(bdev, 0x08, 0x20);
  else
    SAA7114_WriteReg(bdev, 0x08, 0x38);
  
  // input mode select
  if(inputsource == DUNE_COMPOSITE)
    SAA7114_WriteReg(bdev, 0x09, 0x40);
  else if(inputsource == DUNE_SVIDEO)
    SAA7114_WriteReg(bdev, 0x09, 0xC0);
  else if(inputsource == DUNE_TUNER)
    SAA7114_WriteReg(bdev, 0x09, 0x45);

  return 0;
}
#endif

#if 0
// unused, do complete init...
int
DuneSetVBIMode(dunedev_t *bdev, uint32 mode)
{
  int res;

  if(mode == 1)
    {
      saa7114regs[0x41] = 0xff; // active video
      saa7114regs[0x42] = 0xff;
      saa7114regs[0x43] = 0xff;
      saa7114regs[0x44] = 0xff;
      saa7114regs[0x45] = 0x00; // WST
      saa7114regs[0x46] = 0x00;
      saa7114regs[0x47] = 0x00;
      saa7114regs[0x48] = 0x00;
      saa7114regs[0x49] = 0x00;
      saa7114regs[0x4a] = 0x00;
      saa7114regs[0x4b] = 0x00;
      saa7114regs[0x4c] = 0x00;
      saa7114regs[0x4d] = 0x00;
      saa7114regs[0x4e] = 0x00;
      saa7114regs[0x4f] = 0x00;
      saa7114regs[0x50] = 0x00;
      saa7114regs[0x51] = 0x00;
      saa7114regs[0x52] = 0x00;
      saa7114regs[0x53] = 0x00;
      saa7114regs[0x54] = 0x00;
      saa7114regs[0x55] = 0x00;
      saa7114regs[0x56] = 0xff; // active video
      saa7114regs[0x57] = 0xff;
    }
  else if(mode == 0)
    {
      saa7114regs[0x41] = 0xff; // active video
      saa7114regs[0x42] = 0xff;
      saa7114regs[0x43] = 0xff;
      saa7114regs[0x44] = 0xff;
      saa7114regs[0x45] = 0xff; 
      saa7114regs[0x46] = 0xff;
      saa7114regs[0x47] = 0xff;
      saa7114regs[0x48] = 0xff;
      saa7114regs[0x49] = 0xff;
      saa7114regs[0x4a] = 0xff;
      saa7114regs[0x4b] = 0xff;
      saa7114regs[0x4c] = 0xff;
      saa7114regs[0x4d] = 0xff;
      saa7114regs[0x4e] = 0xff;
      saa7114regs[0x4f] = 0xff;
      saa7114regs[0x50] = 0xff;
      saa7114regs[0x51] = 0xff;
      saa7114regs[0x52] = 0xff;
      saa7114regs[0x53] = 0xff;
      saa7114regs[0x54] = 0xff;
      saa7114regs[0x55] = 0xff;
      saa7114regs[0x56] = 0xff; 
      saa7114regs[0x57] = 0xff;
    }
  else
    {
      errno = EINVAL;
      return -1;
    }

  res = SAA7114_WriteBlock(bdev, 0x41, 0x57);

  // TODO: also turn off text stream and clock dependency!

  return res;
}
#endif

extern uint8 muxtable_dvd1_ntsc[];
extern uint8 muxtable_dvd1_pal[];
extern uint8 muxtable_dvd2_ntsc[];
extern uint8 muxtable_dvd2_pal[];
extern uint8 muxtable_dvd2_long_ntsc[];
extern uint8 muxtable_dvd2_long_pal[];
extern uint8 muxtable_dvd2_5mbit_ntsc[];
extern uint8 muxtable_dvd2_5mbit_pal[];
extern uint8 muxtable_svcd1_mpeg2_ntsc[];
extern uint8 muxtable_svcd1_mpeg2_pal[];
extern uint8 muxtable_svcd1_svcd_ntsc[];
extern uint8 muxtable_svcd1_svcd_pal[];
extern uint8 muxtable_svcd2_mpeg2_ntsc[];
extern uint8 muxtable_svcd2_mpeg2_pal[];
extern uint8 muxtable_svcd2_svcd_ntsc[];
extern uint8 muxtable_svcd2_svcd_pal[];
extern uint8 muxtable_vcd1_mpeg1_ntsc[];
extern uint8 muxtable_vcd1_mpeg1_pal[];
extern uint8 muxtable_vcd1_vcd_ntsc[];
extern uint8 muxtable_vcd1_vcd_pal[];
extern uint8 muxtable_vcd2_mpeg1_ntsc[];
extern uint8 muxtable_vcd2_mpeg1_pal[];
extern uint8 muxtable_vcd2_vcd_ntsc[];
extern uint8 muxtable_vcd2_vcd_pal[];

int
DuneInitializeDevice(dunedev_t *bdev, duneparams_t *params)
{
  char *ucode = NULL;
  uint8 *mux = NULL;

  memcpy(&bdev->params, params, sizeof(duneparams_t));

  // sets saaid...
  SAA7114_Reset(bdev);
  if(bdev->saaid != 0x1f7114d0 && bdev->saaid != 0x1f7115d0)
    {
      fprintf(stderr, "Warning: Unhandled SAA711x chip (%x)\n", bdev->saaid);
      fprintf(stderr, "Please contact author.\n");
    }

  if(params->rectype == DUNE_DVD)
    {
      ucode = "k2.v"; // mpeg2
    }
  else if(params->rectype == DUNE_DVD_LONG)
    {
      ucode = "k2.v"; // mpeg2
    }
  else if(params->rectype == DUNE_DVD_5MBIT)
    {
      ucode = "k2.v"; // mpeg2
    }
  else if(params->rectype == DUNE_SVCD_MPEG2)
    {
      ucode = "k2.v"; // mpeg2
    }
  else if(params->rectype == DUNE_SVCD_SVCD)
    {
      ucode = "k2sp1.v"; // real svcd
    }
  else if(params->rectype == DUNE_VCD_MPEG1)
    {
      ucode = "k2_m1.v"; // mpeg1
    }
  else if(params->rectype == DUNE_VCD_VCD)
    {
      ucode = "k2sp.v"; // real vcd
    }
  else
    {
      fprintf(stderr, "Unsupported rectype: %d\n", params->rectype);
      return -1;
    }

  Tuner_Init(bdev);
  TDA9887_Init(bdev);

  if(params->regfile)
    {
      // experimental
      //SAA7114_Init_File(bdev, params, params->regfile);
    }
  else if(params->videostandard == DUNE_NTSC)
    {
      SAA7114_Init_NTSC(bdev, params);
    }
  else
    {
      SAA7114_Init_PAL(bdev, params);
    }

  MSP3400_Init(bdev);

  SAA7114_SetBrightness(bdev, params->brightness);

  if(DuneLoadMicrocode(bdev, ucode) < 0)
    return -1;

  if(dune_ucode_version == NULL)
    {
      fprintf(stderr, "Microcode loader did not set ucode_version!\n");
      return -1;
    }

  fprintf(stderr, "EZUSB2 FW Version: %d Microcode Version: %s\n", 
	  dune_ezusb2_version, dune_ucode_version);
  
  if(DuneKFirPreLoadParams(bdev) < 0)
    return -1;

  if(params->rectype == DUNE_DVD)
    {
      if(params->videostandard == DUNE_NTSC)
	{
	  if(strcmp(dune_ucode_version, UCODEVERSION) == 0)
	    {
	      KFir_Init_DVD2_NTSC(bdev);
	      mux = muxtable_dvd2_ntsc; 
	    }
	  else
	    {
	      KFir_Init_DVD1_NTSC(bdev);
	      mux = muxtable_dvd1_ntsc; 
	    }
	}
      else
	{
	  if(strcmp(dune_ucode_version, UCODEVERSION) == 0)
	    {
	      KFir_Init_DVD2_PAL(bdev);
	      mux = muxtable_dvd2_pal; 
	    }
	  else
	    {
	      KFir_Init_DVD1_PAL(bdev);
	      mux = muxtable_dvd1_pal; 
	    }
	}
    }
  else if(params->rectype == DUNE_DVD_LONG)
    {
      if(params->videostandard == DUNE_NTSC)
	{
	  if(strcmp(dune_ucode_version, UCODEVERSION) == 0)
	    {
	      KFir_Init_DVD2_LONG_NTSC(bdev);
	      mux = muxtable_dvd2_long_ntsc; 
	    }
	  else
	    {
	      fprintf(stderr, "Please install new firmware files\n");
	    }
	}
      else
	{
	  if(strcmp(dune_ucode_version, UCODEVERSION) == 0)
	    {
	      KFir_Init_DVD2_LONG_PAL(bdev);
	      mux = muxtable_dvd2_long_pal; 
	    }
	  else
	    {
	      fprintf(stderr, "Please install new firmware files\n");
	    }
	}
    }
  else if(params->rectype == DUNE_DVD_5MBIT)
    {
      if(params->videostandard == DUNE_NTSC)
	{
	  if(strcmp(dune_ucode_version, UCODEVERSION) == 0)
	    {
	      KFir_Init_DVD2_5MBIT_NTSC(bdev);
	      mux = muxtable_dvd2_5mbit_ntsc; 
	    }
	  else
	    {
	      fprintf(stderr, "Please install new firmware files\n");
	    }
	}
      else
	{
	  if(strcmp(dune_ucode_version, UCODEVERSION) == 0)
	    {
	      KFir_Init_DVD2_5MBIT_PAL(bdev);
	      mux = muxtable_dvd2_5mbit_pal; 
	    }
	  else
	    {
	      fprintf(stderr, "Please install new firmware files\n");
	    }
	}
    }
  else if(params->rectype == DUNE_SVCD_SVCD)
    {
      // Note: the file is full of sector size SYSTEM_PADDING packets! 
      // This is a waste of file space!!
      // MediaPlayer cannot even play these streams..
      if(params->videostandard == DUNE_NTSC)
	{
	  KFir_Init_SVCD2_SVCD_NTSC(bdev);
	  mux = muxtable_svcd2_svcd_ntsc; 
	}
      else
	{
	  KFir_Init_SVCD2_SVCD_PAL(bdev);
	  mux = muxtable_svcd2_svcd_pal; 
	}
    }
  else if(params->rectype == DUNE_SVCD_MPEG2)
    {
      if(params->videostandard == DUNE_NTSC)
	{
	  if(strcmp(dune_ucode_version, UCODEVERSION) == 0)
	    {
	      KFir_Init_SVCD2_MPEG2_NTSC(bdev);
	      mux = muxtable_svcd2_mpeg2_ntsc; 
	    }
	  else
	    {
	      // KFir_Init_SVCD1_MPEG2_NTSC(bdev);
	      // mux = muxtable_svcd1_mpeg2_ntsc; 
	      fprintf(stderr, "Please install new firmware files\n");
	    }
	}
      else
	{
	  if(strcmp(dune_ucode_version, UCODEVERSION) == 0)
	    {
	      KFir_Init_SVCD2_MPEG2_PAL(bdev);
	      mux = muxtable_svcd2_mpeg2_pal; 
	    }
	  else
	    {
	      KFir_Init_SVCD1_MPEG2_PAL(bdev);
	      mux = muxtable_svcd1_mpeg2_pal; 
	    }
	}
    }
  else if(params->rectype == DUNE_VCD_VCD)
    {
      // Note: the file seems to be full of PACKSTART packets! 
      // This is a waste of file space!!

      if(params->videostandard == DUNE_NTSC)
	{
	  KFir_Init_VCD2_VCD_NTSC(bdev);
	  mux = muxtable_vcd2_vcd_ntsc; 
	}
      else
	{
	  KFir_Init_VCD2_VCD_PAL(bdev);
	  mux = muxtable_vcd2_vcd_pal; 
	}
    }
  else if(params->rectype == DUNE_VCD_MPEG1)
    {
      if(params->videostandard == DUNE_NTSC)
	{
	  KFir_Init_VCD2_MPEG1_NTSC(bdev);
	  mux = muxtable_vcd2_mpeg1_ntsc; 
	}
      else
	{
	  KFir_Init_VCD2_MPEG1_PAL(bdev);
	  mux = muxtable_vcd2_mpeg1_pal; 
	}
    }

  if(mux == NULL)
    {
      fprintf(stderr, "Muxtable not supported\n");
      return -1;
    }

  if(DuneSendMuxTable(bdev, mux) < 0)
    return -1;

  if(debug || verbose)
    {
      DuneShowKFirRegs(bdev);
      DuneShowMuxTable(bdev, mux);
    }

  if(debug)
    {
      DuneDumpSAA7114Regs(bdev);
      DuneDumpKFirRegs(bdev);
      DuneDumpMuxTable(bdev, mux);
    }

  return 0;
}

//////////////////////////////////////////////////////////////////////

int
DuneShowChipInfo(dunedev_t *bdev, int infoflag)
{
  int i, n;
  uint8 i2c;
  uint8 buf[64];
  uint8 ebuf[512];
  uint32 lreg = 0;
  uint32 addr;
  uint32 kv;
  int bitrate = 0;

  double afctab[] = {
    -125.0, -62.5, 0.0, 62.5, 125.0
  };

  double afctab2[] = {
    -12.5, -37.5, -62.5, -87.5, -112.5, -137.5, -162.5, -187.5,
    +187.5, +162.5, +137.5, +112.5, +87.5, +62.5, +37.5, +12.5
  };

  if(infoflag > 2)
    {
      fprintf(stderr, "*** EPROM ***\n");
      DuneReadEPROM(bdev, ebuf, 512);
      for(i = 0; i < 512; i += 16)
	{
	  for(n = 0; n < 16; n++)
	    fprintf(stderr, "%02x ", ebuf[i+n]);

	  for(n = 0; n < 16; n++)
	    {
	      if(isprint(ebuf[i+n]))
		fprintf(stderr, "%c", ebuf[i+n]);
	      else
		fprintf(stderr, ".");
	    }

	  fprintf(stderr, "\n");
	}
      fprintf(stderr, "\n");
      
      fprintf(stderr, "VID %04x PID %04x\n",
	      *(uint16 *) (ebuf+1),
	      *(uint16 *) (ebuf+3));
      fprintf(stderr, "\n");
    }

  fprintf(stderr, "*** SAA7114 ***\n");

  DuneI2CRead(bdev, SAAI2CADDR, 0x00, 0x01, 0x01, &i2c);
  fprintf(stderr, "00 ChipVersion: 0x%02x\n", i2c);
  fprintf(stderr, " SAAID:            0x%x\n", bdev->saaid);

  DuneI2CRead(bdev, SAAI2CADDR, 0x1F, 0x01, 0x01, &i2c);
  fprintf(stderr, "1F DecoderStatus: 0x%02x\n", i2c);
  fprintf(stderr, " ReadyForCapture:  0x%02x\n", (i2c >> 0) & 0x01);
  fprintf(stderr, " Macrovision:      0x%02x\n", (i2c >> 1) & 0x01);
  fprintf(stderr, " FieldFreq:        %d Hz\n", ((i2c >> 5) & 0x01) ? 60 : 50);
  fprintf(stderr, " LoopsLocked:      0x%02x\n", (i2c >> 6) & 0x01);
  fprintf(stderr, " Interlace:        0x%02x\n", (i2c >> 7) & 0x01);

  DuneI2CRead(bdev, SAAI2CADDR, 0x02, 0x01, 0x01, &i2c);
  fprintf(stderr, "02 AnalogInputControl1: 0x%02x\n", i2c);
  fprintf(stderr, " Mode:             0x%x\n", i2c & 0x0F);
  fprintf(stderr, " UpdateHysteresis: 0x%x\n", (i2c >> 4) & 0x03);
  fprintf(stderr, " AnalogFunction:   0x%x\n", (i2c >> 6) & 0x03);
  switch((i2c >> 6) & 0x03)
    {
    case 0x00:
      fprintf(stderr, "  Amplifier and anti-alias filter bypassed\n");
      break;
    case 0x02:
      fprintf(stderr, "  Amplifier active\n");
      break;
    case 0x03:
      fprintf(stderr, "  Amplifier and anti-alias filter active\n");
      break;
    default:
      break;
    }

  DuneI2CRead(bdev, SAAI2CADDR, 0x03, 0x01, 0x01, &i2c);
  fprintf(stderr, "03 AnalogInputControl3: 0x%02x\n", i2c);
  fprintf(stderr, " StaticGain1:      0x%x\n", (i2c >> 0) & 0x01);
  fprintf(stderr, " StaticGain2:      0x%x\n", (i2c >> 1) & 0x01);
  fprintf(stderr, " GainControlFix:   0x%x\n", (i2c >> 2) & 0x01);
  fprintf(stderr, " AutoGainControl:  0x%x\n", (i2c >> 3) & 0x01);
  fprintf(stderr, " WhitePeakOff:     0x%x\n", (i2c >> 4) & 0x01);
  fprintf(stderr, " AGCHold:          0x%x\n", (i2c >> 5) & 0x01);
  fprintf(stderr, " HLNotRef:         0x%x\n", (i2c >> 6) & 0x01);

  DuneI2CRead(bdev, SAAI2CADDR, 0x08, 0x01, 0x01, &i2c);
  fprintf(stderr, "08 SyncControl: 0x%02x\n", i2c);
  // this is off by default
  fprintf(stderr, " AutoFieldDetect:  0x%02x\n", (i2c >> 7) & 0x01);
  // this is set by the pal/ntsc command
  fprintf(stderr, " FieldSelection:    %s\n", 
	  ((i2c >> 6) & 0x01) ? "60 Hz 525 Lines" : "50 Hz 625 Lines");
  fprintf(stderr, " OddEvenToggle:    0x%02x\n", (i2c >> 5) & 0x01);
  fprintf(stderr, " HorzTimeConstant: 0x%02x\n", (i2c >> 3) & 0x03);
  fprintf(stderr, " HorizontalPLL:    0x%02x\n", (i2c >> 2) & 0x01);
  fprintf(stderr, " VerticalNoiseRed: 0x%02x\n", (i2c >> 0) & 0x03);

  DuneI2CRead(bdev, SAAI2CADDR, 0x09, 0x01, 0x01, &i2c);
  fprintf(stderr, "09 LuminanceCtl: 0x%02x\n", i2c);
  fprintf(stderr, " Sharpness:        0x%02x\n", (i2c >> 0) & 0x0F);
  fprintf(stderr, " Remodulation:     0x%02x\n", (i2c >> 4) & 0x01);
  fprintf(stderr, " Delay:            0x%02x\n", (i2c >> 5) & 0x01);
  fprintf(stderr, " CombFilter:       0x%02x\n", (i2c >> 6) & 0x01);
  fprintf(stderr, " FilterBypass:     0x%02x\n", (i2c >> 7) & 0x01);

  DuneI2CRead(bdev, SAAI2CADDR, 0x0A, 0x01, 0x01, &i2c);
  fprintf(stderr, "0A BrightnessCtl: 0x%02x\n", i2c);
  fprintf(stderr, " Brightness:       0x%02x\n", i2c);

  DuneI2CRead(bdev, SAAI2CADDR, 0x0B, 0x01, 0x01, &i2c);
  fprintf(stderr, "0B ContrastCtl: 0x%02x\n", i2c);
  fprintf(stderr, " Contrast:         0x%02x\n", i2c);

  DuneI2CRead(bdev, SAAI2CADDR, 0x0C, 0x01, 0x01, &i2c);
  fprintf(stderr, "0C SaturationCtl: 0x%02x\n", i2c);
  fprintf(stderr, " Saturation:       0x%02x\n", i2c);

  DuneI2CRead(bdev, SAAI2CADDR, 0x0D, 0x01, 0x01, &i2c);
  fprintf(stderr, "0D HueCtl: 0x%02x\n", i2c);
  fprintf(stderr, " Hue:              0x%02x\n", i2c);

  DuneI2CRead(bdev, SAAI2CADDR, 0x0E, 0x01, 0x01, &i2c);
  fprintf(stderr, "0E ChrominanceCtl1: 0x%02x\n", i2c);
  // this is 0 by default: 50Hz -> PAL BGDHI, 60Hz -> NTSC M
  fprintf(stderr, " ColourStandard:    0x%02x\n", (i2c >> 4) & 0x03);
  fprintf(stderr, " VerticalFilterOff: %d\n", ((i2c >> 2 ) & 0x01));
  fprintf(stderr, " AdaptiveCombFilt:  %d\n", (i2c & 0x01));

  DuneI2CRead(bdev, SAAI2CADDR, 0x0F, 0x01, 0x01, &i2c);
  fprintf(stderr, "0F ChrominanceGainCtl: 0x%02x\n", i2c);
  fprintf(stderr, " ChrominanceGain:   0x%02x\n", (i2c & 0x8F));
  fprintf(stderr, " AutomaticGainCtl:  0x%02x\n", (i2c >> 7) & 0x01);

  DuneI2CRead(bdev, SAAI2CADDR, 0x10, 0x01, 0x01, &i2c);
  fprintf(stderr, "10 ChrominanceCtl2: 0x%02x\n", i2c);
  fprintf(stderr, " LCBW:              0x%02x\n", i2c & 0x7);
  fprintf(stderr, " CHBW:              0x%02x\n", (i2c >> 3) & 0x1);
  fprintf(stderr, " OFFV:              0x%02x\n", (i2c >> 4) & 0x3);
  fprintf(stderr, " OFFH:              0x%02x\n", (i2c >> 6) & 0x3);
  fprintf(stderr, "\n");

  fprintf(stderr, "Line control registers:\n");
  for(i = 0x41, n = 2; i <= 0x57; i++, n++)
    {
      DuneI2CRead(bdev, SAAI2CADDR, i, 0x01, 0x01, &i2c);
      fprintf(stderr, "LCR%02d FirstField 0x%x Second Field 0x%x\n",
	      n, (i2c >> 4) & 0x0f, (i2c >> 0) & 0x0f);
    }

  DuneI2CRead(bdev, SAAI2CADDR, 0x5a, 0x01, 0x01, &i2c);
  fprintf(stderr, "VBISlicerVertOff: 0x%02x\n", i2c);

  DuneI2CRead(bdev, SAAI2CADDR, 0x5d, 0x01, 0x01, &i2c);
  fprintf(stderr, "VBISlicerHeader:  0x%02x\n", i2c);
  fprintf(stderr, " F+V Selection:   0x%02x\n", (i2c >> 7) & 0x01);
  fprintf(stderr, " DID:             0x%02x\n", i2c & 0x3f);

  DuneI2CRead(bdev, SAAI2CADDR, 0x60, 0x01, 0x01, &i2c);
  fprintf(stderr, "VBISlicerStatus0: 0x%02x\n", i2c);

  DuneI2CRead(bdev, SAAI2CADDR, 0x61, 0x01, 0x01, &i2c);
  fprintf(stderr, "VBISlicerStatus1: 0x%02x\n", i2c);

  DuneI2CRead(bdev, SAAI2CADDR, 0x62, 0x01, 0x01, &i2c);
  fprintf(stderr, "VBISlicerStatus2: 0x%02x\n", i2c);

  // Scaler/IPort
  DuneI2CRead(bdev, SAAI2CADDR, 0x80, 0x01, 0x01, &i2c);
  fprintf(stderr, "TaskEnable:         0x%02x\n", i2c);
  fprintf(stderr, " TaskEnable:         0x%02x\n", (i2c) >> 4 & 0x05);

  DuneI2CRead(bdev, SAAI2CADDR, 0x86, 0x01, 0x01, &i2c);
  fprintf(stderr, "IPortFifoFlags:     0x%02x\n", i2c);
  fprintf(stderr, " Control:            0x%02x\n", (i2c >> 6) & 0x03);

  DuneI2CRead(bdev, SAAI2CADDR, 0x87, 0x01, 0x01, &i2c);
  fprintf(stderr, "IPortIOEnable:      0x%02x\n", i2c & 0x03);

  DuneI2CRead(bdev, SAAI2CADDR, 0x90, 0x01, 0x01, &i2c);
  fprintf(stderr, "EventHandler1:      0x%02x\n", i2c & 0x05);
  fprintf(stderr, "EventHandler2:      0x%02x\n", (i2c >> 3) & 0x05);
  fprintf(stderr, "EventHandler3:      0x%02x\n", (i2c >> 6) & 0x05);

  DuneI2CRead(bdev, SAAI2CADDR, 0x91, 0x01, 0x01, &i2c);
  fprintf(stderr, "ScalerInputFormat1:  0x%02x\n", i2c & 0x05);

  DuneI2CRead(bdev, SAAI2CADDR, 0x93, 0x01, 0x01, &i2c);
  fprintf(stderr, "IPortOutputFormat1:  0x%02x\n", i2c & 0x1F);

  DuneI2CRead(bdev, SAAI2CADDR, 0x94, 0x01, 0x01, &i2c);
  fprintf(stderr, "HorizontInpWStart1:  0x%02x\n", i2c);
  DuneI2CRead(bdev, SAAI2CADDR, 0x95, 0x01, 0x01, &i2c);
  fprintf(stderr, "HorizontInpWStart2:  0x%02x\n", i2c);

  DuneI2CRead(bdev, SAAI2CADDR, 0x96, 0x01, 0x01, &i2c);
  fprintf(stderr, "HorizontalInpWLen1:  0x%02x\n", i2c);
  DuneI2CRead(bdev, SAAI2CADDR, 0x97, 0x01, 0x01, &i2c);
  fprintf(stderr, "HorizontalInpWLen2:  0x%02x\n", i2c);

  DuneI2CRead(bdev, SAAI2CADDR, 0x98, 0x01, 0x01, &i2c);
  fprintf(stderr, "VerticalInpWStart1:  0x%02x\n", i2c);
  DuneI2CRead(bdev, SAAI2CADDR, 0x99, 0x01, 0x01, &i2c);
  fprintf(stderr, "VerticalInpWStart2:  0x%02x\n", i2c);

  DuneI2CRead(bdev, SAAI2CADDR, 0x9A, 0x01, 0x01, &i2c);
  fprintf(stderr, "VerticalInpWLen1:    0x%02x\n", i2c);
  DuneI2CRead(bdev, SAAI2CADDR, 0x9B, 0x01, 0x01, &i2c);
  fprintf(stderr, "VerticalInpWLen2:    0x%02x\n", i2c);

  DuneI2CRead(bdev, SAAI2CADDR, 0x9C, 0x01, 0x01, &i2c);
  fprintf(stderr, "HorizontalOutWLen1:  0x%02x\n", i2c);
  DuneI2CRead(bdev, SAAI2CADDR, 0x9D, 0x01, 0x01, &i2c);
  fprintf(stderr, "HorizontalOutWLen2:  0x%02x\n", i2c);

  DuneI2CRead(bdev, SAAI2CADDR, 0x9E, 0x01, 0x01, &i2c);
  fprintf(stderr, "VerticalOutWLen1:    0x%02x\n", i2c);
  DuneI2CRead(bdev, SAAI2CADDR, 0x9F, 0x01, 0x01, &i2c);
  fprintf(stderr, "VerticalOutWLen2:    0x%02x\n", i2c);

  fprintf(stderr, "\n");

  //////////////////////////////////////////////////////////////////////

  fprintf(stderr, "*** MSP3400 ***\n");

  MSP3400_GetVersion(bdev, buf);
  fprintf(stderr, "Version:          0x%02x%02x\n", buf[1], buf[0]);
  if(buf[1] == 0x0f)
    fprintf(stderr, " 3415G\n"); // no ntsc
  else if(buf[1] == 0x19)
    fprintf(stderr, " 3425G\n");
  else if(buf[1] == 0x2D)
    fprintf(stderr, " 3445G\n");
  else if(buf[1] == 0x37)
    fprintf(stderr, " 3455G\n");
  else if(buf[1] == 0x41)
    fprintf(stderr, " 3465G\n");

  MSP3400_GetStatus(bdev, buf);
  fprintf(stderr, "Status:           0x%02x%02x\n", buf[1], buf[0]);
  fprintf(stderr, " PrimaryCarrier:   %c\n", (buf[0] & 0x02) ? 'N' : 'Y');
  fprintf(stderr, " SecondaryCarrier: %c\n", (buf[0] & 0x04) ? 'N' : 'Y');
  fprintf(stderr, " DigitalSound:     %d\n", (buf[0] >> 5) & 0x01);
  fprintf(stderr, " Mono/Stereo:      %d\n", (buf[0] >> 6) & 0x01);
  fprintf(stderr, " Bilingual/SAP:    %d\n", (buf[0] >> 7) & 0x01);

  MSP3400_GetStandardResult(bdev, buf);
  fprintf(stderr, "StandardResult:   0x%02x%02x\n", buf[1], buf[0]);
  if(buf[0] == 0x00)
    fprintf(stderr, " No standard detected\n");
  else if(buf[0] == 0x03)
    fprintf(stderr, " B/G-FM\n");
  else if(buf[0] == 0x08)
    fprintf(stderr, " B/G-NICAM\n");
  else if(buf[0] == 0x0A)
    fprintf(stderr, " I\n");
  else if(buf[0] == 0x40)
    fprintf(stderr, " FM-Radio\n");
  else if(buf[0] == 0x02 || buf[0] == 0x20 || buf[0] == 0x30)
    fprintf(stderr, " M-BTSC/Korea/Japan\n");

  fprintf(stderr, "\n");

  fprintf(stderr, "*** FI1236MK3 ***\n");
  Tuner_GetStatus(bdev, &i2c);
  fprintf(stderr, "Status:           0x%02x\n", i2c);
  fprintf(stderr, " PowerOnReset:     %d\n", (i2c >> 7) & 0x01);
  fprintf(stderr, " LoopLocked:       %d\n", (i2c >> 6) & 0x01);
  fprintf(stderr, " IOPort:           0x%x\n", (i2c >> 3) & 0x7);
  fprintf(stderr, " AFC:              %.1f kHz\n", afctab[(i2c >> 0) & 0x7]);
  fprintf(stderr, "\n");

  fprintf(stderr, "*** TDA9887 ***\n");
  TDA9887_GetStatus(bdev, &i2c);
  fprintf(stderr, "Status:           0x%02x\n", i2c);
  fprintf(stderr, " AFC:              %.1f kHz\n", afctab2[(i2c >> 1) & 0xF]);
  fprintf(stderr, " CARRDET:          %d\n", (i2c >> 5) & 1);
  fprintf(stderr, " VCO:              %d\n", (i2c >> 7) & 1);
  fprintf(stderr, "\n");

  fprintf(stderr, "*** KFIR ***\n");
  DuneShowKFirRegs(bdev);

  return 0;
}

//////////////////////////////////////////////////////////////////////

int
DuneInitParams(duneparams_t *params)
{
  params->videostandard = DUNE_PAL;
  params->rectype = DUNE_DVD;
  params->inputsource = DUNE_COMPOSITE;
  params->brightness = 0x80;
  params->teletext = 0x00;
  params->regfile = NULL;
  params->kfir_filter = 75;

  return 0;
}

int
DuneReadConfig(const char *fname, duneparams_t *params)
{
  char buf[126];
  char *p, *v;
  FILE *f;
  int nline = 0;

  if((f = fopen(fname, "r")) == NULL)
    {
      perror(fname);
      return -1;
    }

  while(fgets(buf, sizeof(buf), f))
    {
      nline++;

      if((p = strchr(buf, '\n')))
	*p = 0;

      if(buf[0] == 0 || buf[0] == '#')
	continue;

      if((p = strtok(buf, "=")) == NULL)
	{
	  fprintf(stderr, "Error in config file, line %d\n", nline);
	  continue;
	}
      if((v = strtok(NULL, "=")) == NULL)
	{
	  fprintf(stderr, "Error in config file, line %d\n", nline);
	  continue;
	}

      if(stricmp(p, "videostandard") == 0)
	{
	  if(stricmp(v, "pal") == 0)
	    params->videostandard = DUNE_PAL;
	  else if(stricmp(v, "ntsc") == 0)
	    params->videostandard = DUNE_NTSC;
	  else
	    fprintf(stderr, "Error in config file, line %d\n", nline);
	}
      else if(stricmp(p, "rectype") == 0)
	{
	  if(stricmp(v, "dvd") == 0)
	    params->rectype = DUNE_DVD;
	  else if(stricmp(v, "dvdlong") == 0)
	    params->rectype = DUNE_DVD_LONG;
	  else if(stricmp(v, "dvd5mbit") == 0)
	    params->rectype = DUNE_DVD_5MBIT;
	  else if(stricmp(v, "svcd") == 0)
	    params->rectype = DUNE_SVCD_SVCD;
	  else if(stricmp(v, "svcd2") == 0)
	    params->rectype = DUNE_SVCD_MPEG2;
	  else if(stricmp(v, "vcd") == 0)
	    params->rectype = DUNE_VCD_VCD;
	  else if(stricmp(v, "vcd2") == 0)
	    params->rectype = DUNE_VCD_MPEG1;
	  else
	    fprintf(stderr, "Error in config file, line %d\n", nline);
	}
      else if(stricmp(p, "inputsource") == 0)
	{
	  if(stricmp(v, "tuner") == 0)
	    params->inputsource = DUNE_TUNER;
	  else if(stricmp(v, "composite") == 0)
	    params->inputsource = DUNE_COMPOSITE;
	  else if(stricmp(v, "svideo") == 0)
	    params->inputsource = DUNE_SVIDEO;
	  else
	    fprintf(stderr, "Error in config file, line %d\n", nline);
	}
      else if(stricmp(p, "brightness") == 0)
	{
	  sscanf(v, "%x", &params->brightness);
	}
      else if(stricmp(p, "teletext") == 0)
	{
	  sscanf(v, "%x", &params->teletext);
	}
      else if(stricmp(p, "regfile") == 0)
	{
	  params->regfile = strdup(v);
	}
      else if(stricmp(p, "kfir_filter") == 0)
	{
	  params->kfir_filter = atol(v);
	}
      else
	{
	  fprintf(stderr, "Error in config file, line %d\n", nline);
	}
    }

  fclose(f);

  return 0;
}

