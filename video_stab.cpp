#include "video_stab.h"

static void loadBuffer(ap_uint<INPUT_PTR_WIDTH>* in, hls::stream<ap_uint<INPUT_PTR_WIDTH>>& inBuffer, int bufferSize){
mem_rd:
    for (int i = 0; i < bufferSize; i++) {
    #pragma HLS LOOP_TRIPCOUNT min = size max = size
    // Blocking write command to inBuffer
    inBuffer << in[i];
    }

}


static void write_result(ap_uint<INPUT_PTR_WIDTH>* out, hls::stream<ap_uint<INPUT_PTR_WIDTH>>& outStream,
      int bufferSize) {
// Auto-pipeline is going to apply pipeline to this loop
mem_wr:
    for (int i = 0; i < bufferSize; i++) {
    #pragma HLS LOOP_TRIPCOUNT min = size max = size
    // Blocking read command to inStream
    out[i] = outStream.read();
}
}

extern "C"{
void video_stab(ap_uint<INPUT_PTR_WIDTH>* imgInput, ap_uint<INPUT_PTR_WIDTH>* imgInput2, ap_uint<8>* imgOutputGray1, ap_uint<8>* imgOutputGray2, ap_uint<OUTPUT_PTR_WIDTH>* imgOutput, ap_uint<OUTPUT_PTR_WIDTH>* imgOutput2) {
    #pragma HLS INTERFACE m_axi port=imgInput offset=slave bundle=gmem0
    #pragma HLS INTERFACE m_axi port=imgInput2 offset=slave bundle=gmem1
    #pragma HLS INTERFACE m_axi port=imgOutput offset=slave bundle=gmem2
    #pragma HLS INTERFACE m_axi port=imgOutput2 offset=slave bundle=gmem3
    #pragma HLS INTERFACE m_axi port=imgOutputGray1 offset=slave bundle=gmem4
    #pragma HLS INTERFACE m_axi port=imgOutputGray2 offset=slave bundle=gmem5
    #pragma HLS INTERFACE s_axilite port=return bundle=control

    
    xf::cv::Mat<XF_8UC3, HEIGHT_MAX, WIDTH_MAX, NPC1> imgHelper1(HEIGHT, WIDTH);
    xf::cv::Mat<XF_8UC1, HEIGHT_MAX, WIDTH_MAX, NPC1> imgHelper2(HEIGHT, WIDTH);
    xf::cv::Mat<XF_8UC3, HEIGHT_MAX, WIDTH_MAX, NPC1> imgHelper3(HEIGHT, WIDTH);
    xf::cv::Mat<XF_8UC1, HEIGHT_MAX, WIDTH_MAX, NPC1> imgHelper4(HEIGHT, WIDTH);
    xf::cv::Mat<XF_32FC1, HEIGHT_MAX, WIDTH_MAX, NPC1> imgHelper5(HEIGHT, WIDTH);
    xf::cv::Mat<XF_32FC1, HEIGHT_MAX, WIDTH_MAX, NPC1> imgHelper6(HEIGHT, WIDTH);
#pragma HLS stream variable=imgHelper1.data depth=HEIGHT*WIDTH/NPC1
#pragma HLS stream variable=imgHelper2.data depth=HEIGHT*WIDTH/NPC1
    #pragma HLS DATAFLOW
    xf::cv::Array2xfMat<INPUT_PTR_WIDTH , XF_8UC3, HEIGHT_MAX, WIDTH_MAX, NPC1>(imgInput, imgHelper1);
    xf::cv::Array2xfMat<INPUT_PTR_WIDTH , XF_8UC3, HEIGHT_MAX, WIDTH_MAX, NPC1>(imgInput2, imgHelper3);
    
    xf::cv::bgr2gray<XF_8UC3, XF_8UC1, HEIGHT_MAX, WIDTH_MAX, NPC1>(imgHelper1, imgHelper2);
    xf::cv::bgr2gray<XF_8UC3, XF_8UC1, HEIGHT_MAX, WIDTH_MAX, NPC1>(imgHelper3, imgHelper4);

    xf::cv::DenseNonPyrLKOpticalFlow<25, XF_8UC1, HEIGHT_MAX, WIDTH_MAX, NPC1>(imgHelper2, imgHelper4, imgHelper5, imgHelper6);

    xf::cv::xfMat2Array<OUTPUT_PTR_WIDTH , XF_32FC1, HEIGHT_MAX, WIDTH_MAX, NPC1>(imgHelper5, imgOutput);
    xf::cv::xfMat2Array<OUTPUT_PTR_WIDTH , XF_32FC1, HEIGHT_MAX, WIDTH_MAX, NPC1>(imgHelper6, imgOutput2);
    xf::cv::xfMat2Array<8 , XF_8UC1, HEIGHT_MAX, WIDTH_MAX, NPC1>(imgHelper2, imgOutputGray1);
    xf::cv::xfMat2Array<8 , XF_8UC1, HEIGHT_MAX, WIDTH_MAX, NPC1>(imgHelper4, imgOutputGray2);
}

}
