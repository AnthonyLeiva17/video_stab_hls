#include <stdint.h>

#include <hls_stream.h>

#include "imgproc/xf_custom_convolution.hpp"
#include <common/xf_structs.hpp>
#include <common/xf_utility.hpp>

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
using DataStrm = ap_axiu<BITS_DATA,0,0,0>;

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



extern "C"{


 void video_stab(DataT* in, DataT* out, int width, int height) {
#pragma HLS INTERFACE m_axi port = in bundle = gmem0
#pragma HLS INTERFACE m_axi port = out bundle = gmem1
#pragma HLS INTERFACE s_axilite register port = width
#pragma HLS INTERFACE s_axilite register port = height
#pragma HLS INTERFACE s_axilite port = return

        // Streams para el flujo de datos
        hls::stream<DataT> inputStream("inputStream");
        hls::stream<DataT> outputStream("outputStream");
#pragma HLS STREAM depth=1024 variable=inputStream
#pragma HLS STREAM depth=1024 variable=outputStream

        // Matrices para procesamiento de imágenes
        xf::cv::Mat<TYPE, HEIGHT_MAX, WIDTH_MAX, NPC, XF_CV_DEPTH_NPC> imgInput(height, width);
        xf::cv::Mat<TYPE, HEIGHT_MAX, WIDTH_MAX, NPC, XF_CV_DEPTH_NPC> imgOutput(height, width);

#pragma HLS dataflow
        // 1. Cargar datos en el stream
        loadBuffer(in, inputStream, (width * height) / (BITS_DATA / 8));

         xf::cv::Array2xfMat<BITS_DATA, TYPE, HEIGHT_MAX, WIDTH_MAX, NPC, XF_CV_DEPTH_NPC>(inputStream.read(), imgInput);
         xf::cv::filter2D<XF_BORDER_REPLICATE, FILTER_WIDTH, FILTER_WIDTH, TYPE, TYPE, HEIGHT_MAX, WIDTH_MAX, NPC, XF_CV_DEPTH_NPC, XF_CV_DEPTH_NPC>(
         imgInput, imgOutput, EROSION_KERNEL, 0);
        xf::cv::xfMat2axiStrm<BITS_DATA, TYPE, HEIGHT_MAX, WIDTH_MAX, NPC, XF_CV_DEPTH_NPC>(imgOutput, outputStream);

        // 5. Escribir resultados del stream a la memoria
        write_result(out, outputStream, (width * height) / (BITS_DATA / 8));
    }
}

