#ifndef KFIR_H
#define KFIR_H 1

//////////////////////////////////////////////////////////////////////
// old kfir1 regs

// 8 least significant bits of the bit rate, in units of 400 bits/sec
#define KFIR_ADR_BIT_RATE_0           0x05
// 8 intern significant bits of the bit rate, in units of 400 bits/sec
#define KFIR_ADR_BIT_RATE_1           0x06
// 8 most significant bits of the bit rate, in units of 400 bits/sec
#define KFIR_ADR_BIT_RATE_2           0x07
// Internal, 3 = IPPB
#define KFIR_ADR_M                    0x31
// Internal, determines GOP size
#define KFIR_ADR_MULTIPLE             0x32
// Variable bit rate encoding flag
#define KFIR_ADR_VBR_ENABLE           0x36

//////////////////////////////////////////////////////////////////////
// new kfir2 regs

#define KFIR_ADR_AUDIO_FORMAT        0x86
// Sample frequency, 48,44.1,32
#define KFIR_ADR_AUDIO_SAMPLEFREQ    0x94
// Only for vbr
#define KFIR_ADR_PEAK_BITRATE_LO     0x190
#define KFIR_ADR_PEAK_BITRATE_HI     0x191

//////////////////////////////////////////////////////////////////////
// old kfir1 regs

#define KFIR_ADDR_EREADY          0xFE
#define KFIR_EREADY_OK            0x0001
#define KFIR_EREADY_STOPPED       0x0003
#define KFIR_EREADY_SEQEND        0x0004
#define KFIR_EREADY_ERROR         0x00FE

#define KFIR_ADDR_RECORD          0xFF
#define KFIR_RECORD_STOP          0x0000
#define KFIR_RECORD_START         0x0001

//////////////////////////////////////////////////////////////////////
// old kfir1 mux and audio table

#define MUX_AUDIO_ADR_STREAM_TYPE             0

#define MUX_AUDIO_ADR_AUDIO_STREAM_TYPE       3
#define MUX_AUDIO_ADR_AUDIO_STREAM_ID         8

#define MUX_AUDIO_ADR_VIDEO_BITRATE_LOW       9
#define MUX_AUDIO_ADR_VIDEO_BITRATE_HIGH      10

#define MUX_AUDIO_ADR_AUDIO_BITRATE_LOW       11
#define MUX_AUDIO_ADR_AUDIO_BITRATE_HIGH      12

#define MUX_AUDIO_ADR_ES_RATE_LOW             56
#define MUX_AUDIO_ADR_ES_RATE_HIGH            57

#define MUX_AUDIO_ADR_PROGRAM_MUX_RATE_LOW    58
#define MUX_AUDIO_ADR_PROGRAM_MUX_RATE_HIGH   59

#define MUX_AUDIO_ADR_RATE_BOUND_LOW          60
#define MUX_AUDIO_ADR_RATE_BOUND_HIGH         61

// Number of video frames within 100 sec
// NTSC: 2997 PAL: 2500
#define MUX_AUDIO_ADR_FRAMES_PER_100_SEC      74

//////////////////////////////////////////////////////////////////////
// new kfir2 mux and audio table

// 2 for svcd and vcd, 0 for normal dvd, probably used only with 175 below
#define MUX_AUDIO_ADR_VARIABLE_BITRATE        152
// 0xd1
#define MUX_AUDIO_ADR_VBISTREAMID             153
// fixed sector size for vcd/svc, lot's of padding when used
#define MUX_AUDIO_ADR_SECTORSIZE              175

//////////////////////////////////////////////////////////////////////

#endif
