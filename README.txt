**********************************************************************
NEWS

NTSC support and support for new firmware files added.

The latest supported firmware is dunexxx.sys version 1.0.3.105
with k2.v version MPEG2.02.26.

Using the old driver with NTSC is currently not supported.

**********************************************************************
NOTES

The USB device/bus is not rescanned correctly after usb chip firmware
upload.  The program must be restarted.

Must be run as root.

It seems the European MSP cannot handle NTSC/BTSC sound.

**********************************************************************
ENVIRONMENT

DUNE_DIR        Path for dune files.
                Default: /usr/local/lib/dune on Unix
                or       c:/usr/local/lib/dune on Win32.

FILES

$DUNE_DIR/dune.sys
		Win32 driver. Needed for firmware init.

$DUNE_DIR/k2.v
		KFIR microcode from Win32.

$DUNE_DIR/k2_m1.v
		KFIR microcode from Win32.

$DUNE_DIR/k2sp.v
		KFIR microcode from Win32.

$DUNE_DIR/k2sp1.v
		KFIR microcode from Win32.

$DUNE_DIR/channels.txt 
	        Channel/frequency mapping.

$DUNE_DIR/ircodes.txt 
	        Ircode names.

**********************************************************************
INSTALLING

Unpack the tarball and copy dunerec to /usr/local/bin.

Create directory /usr/local/lib/dune.

Copy k2.v from windows to /usr/local/lib/dune/k2.v

Copy dunepal.sys or dunentsc.sys from windows to /usr/local/lib/dune/dune.sys.

Do "chown root /usr/local/bin/dunerec" and "chmod u+s /usr/local/bin/dunerec"
when you want to run it from a non-root account.

You can scan for channels by using dunerec with the -S option.
Put the frequencies in channels.txt.

**********************************************************************
COMPILING

You need libusb. I used libusb-0.1.6a.

**********************************************************************
SAMPLE SHELL SCRIPT

  #!/bin/sh

  dunerec $* -R - | mplayer -framedrop -vo x11 -

AUTHOR

Rainer Keuchel 9/2002 (keuchel@rainer-keuchel.de)

**********************************************************************
THESE PROGRAMS ARE FREEWARE AND COME WITH ABSOLUTELY NO WARRANTY!

THESE PROGRAMS ARE NOT SUPPORTED BY PINNACLE, SO DONT ASK THEM ANY
QUESTIONS!

**********************************************************************

