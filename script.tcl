############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
## Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
############################################################


set ::env(LD_LIBRARY_PATH) /tools/Xilinx/Vitis_HLS/2024.1/lnx64/tools/fpo_v7_1:$::env(LD_LIBRARY_PATH)
set ::env(LD_LIBRARY_PATH) /tools/Xilinx/Vitis_HLS/2024.1/lnx64/tools/fft_v9_1:$::env(LD_LIBRARY_PATH)
set ::env(LD_LIBRARY_PATH) /tools/Xilinx/Vitis_HLS/2024.1/lnx64/tools/fir_v7_0:$::env(LD_LIBRARY_PATH)
set ::env(LD_LIBRARY_PATH) /tools/Xilinx/Vitis_HLS/2024.1/lnx64/tools/dds_v6_0:$::env(LD_LIBRARY_PATH)
set ::env(LD_LIBRARY_PATH) /tools/Xilinx/Vitis_HLS/2024.1/tps/lnx64/gcc-8.3.0/lib:$::env(LD_LIBRARY_PATH)
set ::env(LD_LIBRARY_PATH) /tools/Xilinx/Vitis_HLS/2024.1/lib/lnx64.o/Ubuntu:$::env(LD_LIBRARY_PATH)
set ::env(LD_LIBRARY_PATH) /usr/lib/x86_64-linux-gnu:$::env(LD_LIBRARY_PATH)




set csimflags "-L/usr/local/lib/ -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs"
open_project video_stab
set_top video_stab
add_files video_stab.cpp -cflags "-I/usr/local/include/opencv4/ -I/tools/Xilinx/Vitis_Libraries/vision/L1/include -I/home/anthony/Repositories/video_stab_hls/ -D__SDSVHLS__ --std=c++14"
add_files -tb /home/anthony/Repositories/video_stab_hls/img0.jpg
add_files -tb /home/anthony/Repositories/video_stab_hls/video_stab_tb.cpp -cflags "-I/usr/local/include/opencv4/ -D__XFCV_HLS_MODE__ -I/tools/Xilinx/Vitis_Libraries/vision/L1/include/ -I/home/anthony/Repositories/video_stab_hls/ " -csimflags "-I/usr/include/opencv4 $csimflags --std=c++14"
open_solution "solution1" -flow_target vivado
set_part {xck26-sfvc784-2LV-c}
create_clock -period 10 -name default
#source "./video_stab/solution1/directives.tcl"
csim_design -ldflags "-L/usr/local/lib -L/tools/Xilinx/Vitis_Libraries/L1/lib -lopencv_imgcodecs -lopencv_imgproc -lopencv_core -lopencv_highgui -lopencv_flann -lopencv_features2d -lopencv_videoio -lopencv_video -lopencv_calib3d" -argv {/home/anthony/Repositories/video_stab_hls/img0.jpg}
#csynth_design
cosim_design
#export_design -format ip_catalog
