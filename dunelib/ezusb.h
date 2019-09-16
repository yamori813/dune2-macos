#ifndef EZUSB_H
#define EZUSB_H 1

#define USB_TYPE_CLASS   0x10
#define USB_TYPE_VENDOR  0x20

#define USB_ENDPOINT_OUT 0x00
#define USB_ENDPOINT_IN  0x01

typedef PVOID USBD_PIPE_HANDLE;
typedef PVOID USBD_CONFIGURATION_HANDLE;
typedef PVOID USBD_INTERFACE_HANDLE;

typedef enum _USBD_PIPE_TYPE {
  UsbdPipeTypeControl,
  UsbdPipeTypeIsochronous,
  UsbdPipeTypeBulk,
  UsbdPipeTypeInterrupt
} USBD_PIPE_TYPE;

typedef struct _USBD_PIPE_INFORMATION {
  USHORT MaximumPacketSize;
  UCHAR EndpointAddress;  
  UCHAR Interval; 
  USBD_PIPE_TYPE PipeType;
  USBD_PIPE_HANDLE PipeHandle;
  ULONG MaximumTransferSize;
  ULONG PipeFlags;                                   
} USBD_PIPE_INFORMATION, *PUSBD_PIPE_INFORMATION;

typedef struct _USBD_INTERFACE_INFORMATION {
  USHORT Length;
  UCHAR InterfaceNumber;
  UCHAR AlternateSetting;
  UCHAR Class;
  UCHAR SubClass;
  UCHAR Protocol;
  UCHAR Reserved;
  USBD_INTERFACE_HANDLE InterfaceHandle;
  ULONG NumberOfPipes; 
  USBD_PIPE_INFORMATION Pipes[0];
} USBD_INTERFACE_INFORMATION, *PUSBD_INTERFACE_INFORMATION;

//////////////////////////////////////////////////////////////////////

int usb_control_msg(HANDLE hFile, int reqtype, int req, int val, int index, 
		    unsigned char *buf, int len, int timeout);

int usb_bulk_read(HANDLE hFile, int ep, unsigned char *buf, int len, 
		  int timeout);

int usb_bulk_write(HANDLE hFile, int ep, unsigned char *buf, int len, 
		   int timeout);

int ezusb_getnumpipes(HANDLE hFile);
int ezusb_getpipenum(HANDLE hFile, int ep);

int usleep(int milli);

#endif
