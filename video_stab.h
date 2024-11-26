#ifndef __VIDEOSTAB_H__
#define __VIDEOSTAB_H__

#include <stdint.h>
#include <ap_int.h>
#include <ap_fixed.h>
#include <hls_stream.h>

static constexpr int kBusWidth = 64;
static constexpr int kDataWidth = 16;
static constexpr int kDataInt = 6;
static constexpr int kPackets = kBusWidth / kDataWidth;
static constexpr int kShiftData = 2; // Packets 4


#define BITS_DATA 64
using DataT = ap_uint<BITS_DATA>;

extern "C" {

void video_stab(DataT* in, DataT* out, int width, int height);

}
#endif // __VIDEOSTAB_H__

