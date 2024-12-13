#include <stdio.h>
#include <ap_int.h>            // Tipos de datos específicos de HLS
#include <hls_stream.h>        // Para la definición de streams de datos
#include "common/xf_utility.hpp"
#include "opencv2/opencv.hpp"
#include <ap_axi_sdata.h>
#include "video_stab.h"

int main()
{
	 cv::Mat imgInput0, imgInput1, imgInput2;
	 /* cv::Mat refOutput0, refOutput1, refOutput2; */
	 cv::Mat errImg0, errImg1, errImg2;
	imgInput0 = cv::imread(INPUT_IMAGE);

    cvtColor(imgInput0, imgInput0, cv::COLOR_BGR2RGB);
    ap_uint<32 * NPC1>* _imgInput0 = (ap_uint<32 * NPC1>*)imgInput0.data;

    /* refOutput0 = cv::imread(REF_IMAGE);
    if (!refOutput0.data) {
        fprintf(stderr, "Can't open image %s !!.\n ", argv[2]);
        return -1;
    } */
    ap_uint<8 * NPC1>* _imgOutput0 = (ap_uint<8 * NPC1>*)malloc(HEIGHT * WIDTH * 8);

    cv::Mat imgOutput0(HEIGHT, WIDTH, CV_8UC1);

    video_stab(_imgInput0, _imgOutput0);

    imgOutput0.data = (unsigned char*)_imgOutput0;

    cv::imwrite("out_GRAY.png", imgOutput0);
    cv::Size S0(WIDTH, HEIGHT);
    errImg0.create(S0, CV_8UC1);

    /* cv::absdiff(refOutput0, imgOutput0, errImg0); */

    cv::imwrite("err_GRAY.png", imgOutput0);
}

