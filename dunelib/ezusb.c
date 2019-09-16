// ezusb.c - libusb emulation using ezusb.sys/ezusb2.sys
//
// Time-stamp: <28/12/02 07:29:36 keuchel@rainer-keuchel.de>

#include <windows.h>
#include <stdio.h>
#include "ezusbsys.h"
#include "ezusb.h"

int 
usb_control_msg(HANDLE hFile, int reqtype, int req, int val, int index, 
		unsigned char *buf, int len, int timeout)
{
  VENDOR_OR_CLASS_REQUEST_CONTROL ctl;
  DWORD dwRetBytes = 0;

  // short transfer is always added by driver
  ctl.direction = (reqtype >> 0) & 0x0F; // 0 = out, 1 = in
  ctl.requestType = (reqtype >> 4) & 0x0F; // 2 = vendor device
  ctl.recepient = 0; // 0 = device
  // never used
  ctl.requestTypeReservedBits = 0;
  ctl.request = req;
  ctl.value = val;
  ctl.index = index;

#ifdef PATCHED_EZUSB
  ctl.timeout = timeout;
#endif

#if 0
  fprintf(stderr, "VENDOR: dir = %d type = %d len %d\n",
	  ctl.direction, ctl.requestType, len);
#endif

  if(DeviceIoControl(hFile, IOCTL_EZUSB_VENDOR_OR_CLASS_REQUEST,
		     &ctl, sizeof(ctl),
		     buf, len, 
		     &dwRetBytes, NULL) == FALSE)
    {
      fprintf(stderr, "DeviceIOControl: %d\n", GetLastError());
      return -1;
    }

  return 0;
}

int 
usb_bulk_read(HANDLE hFile, int ep, unsigned char *buf, int len, 
	      int timeout)
{
  BULK_TRANSFER_CONTROL ctl;
  DWORD dwRetBytes = 0;

  ctl.pipeNum = ezusb_getpipenum(hFile, ep);
#ifdef PATCHED_EZUSB
  ctl.timeout = timeout;
#endif

  if(DeviceIoControl(hFile, IOCTL_EZUSB_BULK_READ,
		     &ctl, sizeof(ctl),
		     buf, len, 
		     &dwRetBytes, NULL) == FALSE)
    {
      return -1;
    }

  return dwRetBytes;
}

int 
usb_bulk_write(HANDLE hFile, int ep, unsigned char *buf, int len, 
	       int timeout)
{
  BULK_TRANSFER_CONTROL ctl;
  DWORD dwRetBytes = 0;

  ctl.pipeNum = ezusb_getpipenum(hFile, ep);

  if(DeviceIoControl(hFile, IOCTL_EZUSB_BULK_WRITE,
		     &ctl, sizeof(ctl),
		     buf, len, 
		     &dwRetBytes, NULL) == FALSE)
    {
      return -1;
    }

  return dwRetBytes;
}

char *
usb_strerror()
{
  static char buf[126];

  sprintf(buf, "Error %d", GetLastError());
  return buf;
}

int
usleep(int milli)
{
  Sleep(milli);
  return 0;
}

static UCHAR InterfaceInfo[1024];
static PUSBD_INTERFACE_INFORMATION pInterfaceInfo;

int
ezusb_getinfo(HANDLE hFile)
{
  DWORD nBytes;
  PUSBD_PIPE_INFORMATION pPipeInfo;
  int i;

  if(pInterfaceInfo)
    return 0;

  if(DeviceIoControl(hFile,
		     IOCTL_Ezusb_GET_PIPE_INFO,
		     NULL,
		     0,
		     &InterfaceInfo,
		     sizeof(InterfaceInfo),
		     &nBytes,
		     NULL) == FALSE)
    {
      return -1;
    }

  pInterfaceInfo = (PUSBD_INTERFACE_INFORMATION) &InterfaceInfo;

  for(i = 0; i < pInterfaceInfo->NumberOfPipes; i++)
    {
      fprintf(stderr, "Pipe %d: EP 0x%x\n",
	      i, pInterfaceInfo->Pipes[i].EndpointAddress);
    }

  return 0;
}

int
ezusb_getnumpipes(HANDLE hFile)
{
  if(ezusb_getinfo(hFile) < 0)
    return -1;

  return pInterfaceInfo->NumberOfPipes;
}

int
ezusb_getpipenum(HANDLE hFile, int ep)
{
  PUSBD_PIPE_INFORMATION pPipeInfo;
  int i;

  if(ezusb_getinfo(hFile) < 0)
    return -1;

  for(i = 0; i < pInterfaceInfo->NumberOfPipes; i++)
    {
      if(pInterfaceInfo->Pipes[i].EndpointAddress == ep)
	return i;
    }

  return -1;
}
