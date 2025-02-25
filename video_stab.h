#ifndef _VIDEO_STAB_H_
#define _VIDEO_STAB_H_

#include <stdint.h>
#include <hls_stream.h>
#include <ap_int.h>
#include <common/xf_common.hpp>
#include <common/xf_structs.hpp>
#include <imgproc/xf_cvt_color.hpp>
#include <video/xf_dense_npyr_optical_flow.hpp>

#define HEIGHT 384
#define WIDTH 640
#define HEIGHT_MAX 1920
#define WIDTH_MAX 1080
#define INPUT_PTR_WIDTH 64
#define OUTPUT_PTR_WIDTH 32

#define NPC1 XF_NPPC1
extern "C"{
void video_stab(ap_uint<INPUT_PTR_WIDTH>* imgInput, ap_uint<INPUT_PTR_WIDTH>* imgInput2, ap_uint<8>* imgOutputGray1, ap_uint<8>* imgOutputGray2, ap_uint<OUTPUT_PTR_WIDTH>* imgOutput, ap_uint<OUTPUT_PTR_WIDTH>* imgOutput2);}

#endif // _VIDEO_STAB_H_
