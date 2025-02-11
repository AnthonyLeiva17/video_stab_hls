
# Open project
open_project video_stab_project

# Create a new solution
open_solution "solution1" -flow_target vivado

# Set the target part
set_part xc7z020clg484-1

# Set the top function
set_top video_stab


# Set include path of Vitis Libraries
set_include_path "/tools/Xilinx-2023.2/Vitis_Libraries/vision/L1/include"


# Add source files
add_files video_stab.cpp -cflags "-I/usr/include/opencv4 -I/home/aleiva/docs/video_stab_hls -I/tools/Xilinx-2023.2/Vitis_Libraries/vision/L1/include -std=c++14 -DALLOW_EMPTY_HLS_STREAM_READS"
add_files video_stab.h 

# Add testbench file
add_files -tb video_stab_tb.cpp -cflags "-I/usr/include/opencv4 -I/home/aleiva/docs/video_stab_hls -I/tools/Xilinx-2023.2/Vitis_Libraries/vision/L1/include -std=c++14 -DALLOW_EMPTY_HLS_STREAM_READS"
add_files -tb img1_data.txt
# Create the clock with a period of 10 ns
create_clock -period 10 -name default

# Set compilation options for OpenCV
open_solution solution1


# Run C simulation
csim_design 

# Run C synthesis
#csynth_design

# Run co-simulation
#cosim_design -clean

# Export the design
#export_design -format ip_catalog

# Close project
close_project
