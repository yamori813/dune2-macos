#include "dunelib.h"
#include "kfir.h"

int
vendorrequest_write(dunedev_t *bdev, int req, int val,
                    unsigned char *buf, int size)
{

  int status;
        
  status = libusb_control_transfer(bdev->usb,
    LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_VENDOR,
//    LIBUSB_RECIPIENT_DEVICE,
    req, val, 0, buf, (uint16_t)size, 1000);

  return 0;
}

int
vendorrequest_write2(dunedev_t *bdev, int req, int val, int idx,
                     unsigned char *buf, int size)
{

  int status;
        
  status = libusb_control_transfer(bdev->usb,
    LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_VENDOR,
    req, val, idx, buf, (uint16_t)size, 1000);

  return 0;
}

int
vendorrequest_read(dunedev_t *bdev, int req, int val,
                   unsigned char *buf, int size)
{

  int status;

  status = libusb_control_transfer(bdev->usb,
    LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR,
    req, val, 0, buf, (uint16_t)size, 1000);

  return 0;
}
