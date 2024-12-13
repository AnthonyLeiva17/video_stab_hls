#include <stdint.h>
#include "video_stab.h"
#include <hls_stream.h>
#include <common/xf_structs.hpp>
#include <common/xf_utility.hpp>
#include "common/xf_common.hpp"
#include "imgproc/xf_cvt_color.hpp"
#include "ap_int.h"
#include "video/xf_dense_npyr_optical_flow.hpp"







float transformation_matrix[9] = {
  0.38567165, -0.45962748, 374.93268,
  0.45962748, 0.38567165, 139.60342,
  0, 0, 1
};    

/*
static void loadBuffer(DataT* in, hls::stream<DataT>& inBuffer,
                       int bufferSize){
      mem_rd:
    for (int i = 0; i < bufferSize; i++) {
#pragma HLS LOOP_TRIPCOUNT min = size max = size
        // Blocking write command to inBuffer
        inBuffer << in[i];
    }

}


static void write_result(DataT* out, hls::stream<DataT>& outStream,
                         int bufferSize) {
// Auto-pipeline is going to apply pipeline to this loop
mem_wr:
    for (int i = 0; i < bufferSize; i++) {
#pragma HLS LOOP_TRIPCOUNT min = size max = size
        // Blocking read command to inStream
        out[i] = outStream.read();
    }
}*/




void cvtcolor_rgb2gray(ap_uint<32 * NPC1>* imgInput, ap_uint<8 * NPC1>* imgOutput) {
    static constexpr int __XF_DEPTH_INP_0 = ((HEIGHT) * (WIDTH) * (XF_PIXELWIDTH(XF_8UC3, NPC1))) / (32 * NPC1);
    static constexpr int __XF_DEPTH_OUT_0 = ((HEIGHT) * (WIDTH) * (XF_PIXELWIDTH(XF_8UC1, NPC1))) / (8 * NPC1);

// clang-format off
    #pragma HLS INTERFACE m_axi      port=imgInput    offset=slave  bundle=gmem_in0  depth=__XF_DEPTH_INP_0
    #pragma HLS INTERFACE m_axi      port=imgOutput   offset=slave  bundle=gmem_out0 depth=__XF_DEPTH_OUT_0
    #pragma HLS INTERFACE s_axilite  port=return
    // clang-format on

    xf::cv::Mat<XF_8UC3, HEIGHT, WIDTH, NPC1, XF_CV_DEPTH_IN> _imgInput(HEIGHT, WIDTH);
    xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH, NPC1, XF_CV_DEPTH_OUT> _imgOutput(HEIGHT, WIDTH);

// clang-format off
    #pragma HLS DATAFLOW
    // clang-format on

    xf::cv::Array2xfMat<32 * NPC1, XF_8UC3, HEIGHT, WIDTH, NPC1, XF_CV_DEPTH_IN>(imgInput, _imgInput);

    xf::cv::rgb2gray<XF_8UC3, XF_8UC1, HEIGHT, WIDTH, NPC1, XF_CV_DEPTH_IN, XF_CV_DEPTH_OUT>(_imgInput, _imgOutput);

    xf::cv::xfMat2Array<8 * NPC1, XF_8UC1, HEIGHT, WIDTH, NPC1, XF_CV_DEPTH_OUT>(_imgOutput, imgOutput);
}


static constexpr int __XF_DEPTH_INP_0 = ((HEIGHT) * (WIDTH) * (XF_PIXELWIDTH(XF_8UC3, NPC1))) / (32 * NPC1);
static constexpr int __XF_DEPTH_OUT_0 = ((HEIGHT) * (WIDTH) * (XF_PIXELWIDTH(XF_8UC1, NPC1))) / (8 * NPC1);

extern "C"{


void video_stab(ap_uint<32 * NPC1>* imgInput, ap_uint<8 * NPC1>* imgOutput){

#pragma HLS INTERFACE m_axi      port=imgInput    offset=slave  bundle=gmem_in0  depth=__XF_DEPTH_INP_0
#pragma HLS INTERFACE m_axi      port=imgOutput   offset=slave  bundle=gmem_out0 depth=__XF_DEPTH_OUT_0
#pragma HLS INTERFACE s_axilite  port=return




  xf::cv::Mat<XF_8UC3, HEIGHT, WIDTH, NPC1, XF_CV_DEPTH_IN> imgInput1(HEIGHT, WIDTH);
  xf::cv::Mat<XF_8UC1, HEIGHT, WIDTH, NPC1, XF_CV_DEPTH_OUT> ImgOutput1(HEIGHT, WIDTH);


  xf::cv::Mat<TYPE, HEIGHT_MAX, WIDTH_MAX, NPC, XF_CV_DEPTH_NPC> imgInput2(HEIGHT, WIDTH);
  xf::cv::Mat<TYPE, HEIGHT_MAX, WIDTH_MAX, NPC, XF_CV_DEPTH_NPC> imgOutput2(HEIGHT, WIDTH);

  xf::cv::Mat<TYPE, HEIGHT_MAX, WIDTH_MAX, NPC, XF_CV_DEPTH_NPC> imgInput3(HEIGHT, WIDTH);
  xf::cv::Mat<TYPE, HEIGHT_MAX, WIDTH_MAX, NPC, XF_CV_DEPTH_NPC> imgOutput3(HEIGHT, WIDTH);

  xf::cv::Mat<TYPE, HEIGHT_MAX, WIDTH_MAX, NPC, XF_CV_DEPTH_NPC> imgInput4(HEIGHT, WIDTH);
  xf::cv::Mat<TYPE, HEIGHT_MAX, WIDTH_MAX, NPC, XF_CV_DEPTH_NPC> imgOutput5(HEIGHT, WIDTH);

#pragma HLS dataflow
  // dataflow pragma instruct compiler to run following three APIs in parallel
  xf::cv::Array2xfMat<32 * NPC1, XF_8UC3, HEIGHT, WIDTH, NPC1, XF_CV_DEPTH_IN>(imgInput, imgInput1);
  xf::cv::rgb2gray<XF_8UC3, XF_8UC1, HEIGHT, WIDTH, NPC1, XF_CV_DEPTH_IN, XF_CV_DEPTH_OUT>(imgInput1, ImgOutput1);
  xf::cv::xfMat2Array<8 * NPC1, XF_8UC1, HEIGHT, WIDTH, NPC1, XF_CV_DEPTH_OUT>(ImgOutput1, imgOutput);

    
        
    }

}
