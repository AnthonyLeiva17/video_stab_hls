############################################################
## This file is generated automatically by Vitis HLS.
## Please DO NOT edit it.
## Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
############################################################
open_project video_stab
set_top video_stab
set_include_path "/tools/Xilinx-2023.2/Vitis_Libraries/vision/L1/include"
add_files video_stab.cpp -cflags "-std=c++11"  -cflags " -I /home/lleon95/Documents/XRT-Kria/matmul"
add_files -tb	video_stab_tb.cpp
open_solution "solution1"
#set_part {xczu7ev-ffvc1156-2-e}
set_part {xck26-sfvc784-2LV-c}
create_clock -period 4 -name default

#source "./filter/solution1/directives.tcl"
#csim_design
csynth_design
#cosim_design
export_design -format ip_catalog
