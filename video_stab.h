#ifndef __VIDEOSTAB_H__
#define __VIDEOSTAB_H__

#include <stdint.h>
#include <ap_int.h>
#include <ap_fixed.h>
#include <hls_stream.h>

#include <iostream>
#include "common/xf_common.hpp"
#include "common/xf_utility.hpp"
#include "common/xf_types.hpp"
#include "video/xf_dense_npyr_optical_flow.hpp"


#define WORD_SZ 1
#define KMED 25
#define XF_USE_URAM false

#define TYPE XF_8UC1
#define NPC XF_NPPC8
#define XF_CV_DEPTH_NPC 2

#define HEIGHT_MAX 2160
#define WIDTH_MAX 3840

//Warp 
#define NUM_STORE_ROWS 100
#define START_PROC 50
#define TRANSFORM_TYPE 1
#define INTERPOLATION 1

#define FILTER_WIDTH 3
#define BITS_DATA 32
using DataT = ap_uint<BITS_DATA>;


#define XF_CV_DEPTH_IN 3
#define XF_CV_DEPTH_OUT 4

#define XF_CV_DEPTH_IN_0 3
#define XF_CV_DEPTH_OUT_0 4

#define XF_CV_DEPTH_IN_1 3
#define XF_CV_DEPTH_OUT_1 3
#define XF_CV_DEPTH_IN_2 2
#define XF_CV_DEPTH_OUT_2 4
#define NPC1 XF_NPPC1



static constexpr int WIDTH = 1920;
static constexpr int HEIGHT = 1080;

#define INPUT_IMAGE "rover.bmp"


extern "C" {
    void video_stab(ap_uint<32 * NPC1>* imgInput, ap_uint<8 * NPC1>* imgOutput);
}

#endif // __VIDEOSTAB_H__

 
