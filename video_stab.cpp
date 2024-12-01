#include <stdint.h>

#include <hls_stream.h>

#include "imgproc/xf_custom_convolution.hpp"
#include <common/xf_structs.hpp>
#include <common/xf_utility.hpp>
#include "video/xf_dense_npyr_optical_flow.hpp"




//definitions
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
#define BITS_DATA 64
using DataT = ap_uint<BITS_DATA>;


//optical flow config
#define INPUT_PTR_WIDTH_OF 32
#define OUTPUT_PTR_WIDTH_OF 32

static constexpr int _XF_DEPTH_IN =
    (HEIGHT_MAX * WIDTH_MAX * (XF_PIXELWIDTH(XF_8UC1, XF_NPPC1)) / 8) / (INPUT_PTR_WIDTH_OF / 8);
static constexpr int _XF_DEPTH_OUT =
    (HEIGHT_MAX * WIDTH_MAX * (XF_PIXELWIDTH(XF_32FC1, XF_NPPC1)) / 32) / (OUTPUT_PTR_WIDTH_OF / 32);



short int EROSION_KERNEL[FILTER_WIDTH * FILTER_WIDTH] = {
    0, -1, 0, -1, 5, -1, 0, -1, 0};


float transformation_matrix[9] = {
  0.38567165, -0.45962748, 374.93268,
  0.45962748, 0.38567165, 139.60342,
  0, 0, 1
};    


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
}


static void optical_flow(ap_uint<INPUT_PTR_WIDTH_OF>* img_curr,
                        ap_uint<INPUT_PTR_WIDTH_OF>* img_prev,
                        ap_uint<OUTPUT_PTR_WIDTH_OF>* img_outx,
                        ap_uint<OUTPUT_PTR_WIDTH_OF>* img_outy,
                            int rows,
                            int cols) {
    


}


extern "C"{


    void video_stab(DataT* in, DataT* out, int width, int height){
#pragma HLS INTERFACE m_axi port = in bundle = gmem0
#pragma HLS INTERFACE m_axi port = out bundle = gmem1
#pragma HLS INTERFACE s_axilite register port=width
#pragma HLS INTERFACE s_axilite register port=height
#pragma HLS INTERFACE s_axilite  port=return


  xf::cv::Mat<TYPE, HEIGHT_MAX, WIDTH_MAX, NPC, XF_CV_DEPTH_NPC> imgInput(height, width);
  xf::cv::Mat<TYPE, HEIGHT_MAX, WIDTH_MAX, NPC, XF_CV_DEPTH_NPC> imgOutput(height, width);

#pragma HLS dataflow
  // dataflow pragma instruct compiler to run following three APIs in parallel
  xf::cv::Array2xfMat<BITS_DATA, TYPE, HEIGHT_MAX, WIDTH_MAX, NPC, XF_CV_DEPTH_NPC>(in, imgInput);
  xf::cv::filter2D<XF_BORDER_REPLICATE, FILTER_WIDTH, FILTER_WIDTH, TYPE, TYPE, HEIGHT_MAX, WIDTH_MAX, NPC, XF_CV_DEPTH_NPC, XF_CV_DEPTH_NPC>(
    imgInput, imgOutput, EROSION_KERNEL, 0);
  xf::cv::xfMat2Array<BITS_DATA, TYPE, HEIGHT_MAX, WIDTH_MAX, NPC, XF_CV_DEPTH_NPC>(imgOutput, out);

        
    }

}


