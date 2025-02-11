#include "video_stab.h"



extern "C"{
void video_stab(ap_uint<32>* imgInput, ap_uint<32>* imgInput2, ap_uint<32>* imgOutput, ap_uint<32>* imgOutput2) {
    #pragma HLS INTERFACE m_axi port=imgInput offset=slave bundle=gmem 
    #pragma HLS INTERFACE m_axi port=imgOutput offset=slave bundle=gmem 
    #pragma HLS INTERFACE s_axilite port=return

    xf::cv::Mat<XF_8UC3, HEIGHT_MAX, WIDTH_MAX, NPC1> imgHelper1(HEIGHT, WIDTH);
    xf::cv::Mat<XF_8UC1, HEIGHT_MAX, WIDTH_MAX, NPC1> imgHelper2(HEIGHT, WIDTH);
    xf::cv::Mat<XF_8UC3, HEIGHT_MAX, WIDTH_MAX, NPC1> imgHelper3(HEIGHT, WIDTH);
    xf::cv::Mat<XF_8UC1, HEIGHT_MAX, WIDTH_MAX, NPC1> imgHelper4(HEIGHT, WIDTH);
    xf::cv::Mat<XF_32FC1, HEIGHT_MAX, WIDTH_MAX, NPC1> imgHelper5(HEIGHT, WIDTH);
    xf::cv::Mat<XF_32FC1, HEIGHT_MAX, WIDTH_MAX, NPC1> imgHelper6(HEIGHT, WIDTH);
#pragma HLS stream variable=imgHelper1.data depth=16
#pragma HLS stream variable=imgHelper2.data depth=16
    #pragma HLS DATAFLOW
    xf::cv::Array2xfMat<32 , XF_8UC3, HEIGHT_MAX, WIDTH_MAX, NPC1>(imgInput, imgHelper1);
    xf::cv::Array2xfMat<32 , XF_8UC3, HEIGHT_MAX, WIDTH_MAX, NPC1>(imgInput2, imgHelper3);
    
    xf::cv::bgr2gray<XF_8UC3, XF_8UC1, HEIGHT_MAX, WIDTH_MAX, NPC1>(imgHelper1, imgHelper2);
    xf::cv::bgr2gray<XF_8UC3, XF_8UC1, HEIGHT_MAX, WIDTH_MAX, NPC1>(imgHelper3, imgHelper4);

    xf::cv::DenseNonPyrLKOpticalFlow<25, XF_8UC1, HEIGHT_MAX, WIDTH_MAX, NPC1>(imgHelper2, imgHelper4, imgHelper5, imgHelper6);

    xf::cv::xfMat2Array<32 , XF_32FC1, HEIGHT_MAX, WIDTH_MAX, NPC1>(imgHelper5, imgOutput);
    xf::cv::xfMat2Array<32 , XF_32FC1, HEIGHT_MAX, WIDTH_MAX, NPC1>(imgHelper6, imgOutput2);
}

}
