#include <stdio.h>
#include <ap_int.h>
#include <hls_stream.h>
#include "common/xf_utility.hpp"
#include <common/xf_structs.hpp>
#include <ap_axi_sdata.h>
#include "video_stab.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// Definir tipo de matriz XF

// Función para leer datos del archivo y llenar el buffer de entrada
void read_txt_and_prepare_input(const char* filename, ap_uint<INPUT_PTR_WIDTH>* imgInput) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        return;
    }
    printf("Archivo cargado\n");

    // Variables temporales para almacenar los valores RGB
    unsigned int r, g, b, x, r1, g1, b1, x1;

    // Llenar el buffer de entrada
    for (int i = 0; i < HEIGHT * WIDTH; i++) {
        infile >> b >> g >> r >> x >> b1 >> g1 >> r1 >> x1; // Leer valores BGR
        ap_uint<INPUT_PTR_WIDTH> pixel_pack = 0;

        // Empaquetar los valores RGB en un ap_uint<24 * NPC1>
        pixel_pack.range(7, 0) = b;   // Azul   
        pixel_pack.range(15, 8) = g;  // Verde
        pixel_pack.range(23, 16) = r; // Rojo
        pixel_pack.range(31, 24) = x; // Canal alfa (no se usa)
        pixel_pack.range(39, 32) = b1;   // Azul   
        pixel_pack.range(47, 40) = g1;  // Verde
        pixel_pack.range(55, 48) = r1; // Rojo
        pixel_pack.range(63, 56) = x1; // Canal alfa (no se usa)
        imgInput[i] = pixel_pack; // Asignar al buffer
    }

    infile.close();
}

// Función para escribir los resultados en un archivo de salida
void write_output_to_txt(const char* filename, ap_uint<OUTPUT_PTR_WIDTH>* imgOutput) {
    std::ofstream outfile(filename);
    if (!outfile) {
        std::cerr << "Error al abrir el archivo de salida: " << filename << std::endl;
        return;
    }
    for (int i = 0; i < HEIGHT * WIDTH; i++) {
        ap_uint<OUTPUT_PTR_WIDTH> pixel = imgOutput[i];
        outfile << pixel.to_int() << " ";
        if ((i + 1) % WIDTH == 0) {
            outfile << std::endl;
        }
    }
    outfile.close();
}
void write_output_to_txt_gray(const char* filename, ap_uint<8>* imgOutput) {
    std::ofstream outfile(filename);
    if (!outfile) {
        std::cerr << "Error al abrir el archivo de salida: " << filename << std::endl;
        return;
    }
    for (int i = 0; i < HEIGHT * WIDTH; i++) {
        ap_uint<8> pixel = imgOutput[i];
        outfile << pixel.to_uint() << " ";
        if ((i + 1) % WIDTH == 0) {
            outfile << std::endl;
        }
    }
    outfile.close();
}



void write_output_to_txt_float(const char* filename, ap_uint<OUTPUT_PTR_WIDTH>* imgOutput) {
    std::ofstream outfile(filename);
    if (!outfile) {
        std::cerr << "Error al abrir el archivo de salida: " << filename << std::endl;
        return;
    }
    
    for (int i = 0; i < HEIGHT * WIDTH; i++) {
        ap_uint<OUTPUT_PTR_WIDTH> pixel = imgOutput[i];
        float pixel_value;
        std::memcpy(&pixel_value, &pixel, sizeof(float)); // Interpretar los bits como float
        outfile << pixel_value << " ";
        
        if ((i + 1) % WIDTH == 0) {
            outfile << std::endl;
        }
    }
    
    outfile.close();
}

void print_imgInput(ap_uint<INPUT_PTR_WIDTH>* imgInput) {
    for (int i = 0; i < 12; i++) {
        // Desempaquetar los valores RGB
        unsigned int b = imgInput[i].range(7, 0);
        unsigned int g = imgInput[i].range(15, 8);
        unsigned int r = imgInput[i].range(23, 16);
        unsigned int x = imgInput[i].range(31, 24);

        std::cout <<  i << " " << b << " " << g << " " << r << " " << x << std::endl;
    }
}

// Función principal
int main() {
    // Buffers de entrada y salida
    ap_uint<INPUT_PTR_WIDTH> imgInput[HEIGHT * WIDTH];
    ap_uint<INPUT_PTR_WIDTH> imgInput2[HEIGHT * WIDTH];
    ap_uint<OUTPUT_PTR_WIDTH> imgOutput[HEIGHT * WIDTH];
    ap_uint<OUTPUT_PTR_WIDTH> imgOutput2[HEIGHT * WIDTH];
    ap_uint<8> imgOutputGray[HEIGHT * WIDTH];
    ap_uint<8> imgOutputGray2[HEIGHT * WIDTH];
    // Leer datos desde archivos de entrada
    read_txt_and_prepare_input("./data/img1_data.txt", imgInput);
    read_txt_and_prepare_input("img2_data.txt", imgInput2);
    
     // Llamar a la función de estabilización
    printf("Procesando video\n");
    print_imgInput(imgInput2);
    video_stab(imgInput, imgInput2,imgOutputGray,imgOutputGray2, imgOutput, imgOutput2);
    printf("Proceso completado\n");

    // Escribir los resultados en un archivo .txt
    write_output_to_txt_float("../../../../output_data.txt", imgOutput);
    write_output_to_txt_float("../../../../output_data2.txt", imgOutput2);
    write_output_to_txt_gray("../../../../output_gray_data.txt", imgOutputGray);
    write_output_to_txt_gray("../../../../output_gray_data2.txt", imgOutputGray2);
    printf("Archivos de salida generados\n");


    return 0;
}
