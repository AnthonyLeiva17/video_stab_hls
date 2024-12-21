#include <stdio.h>
#include <ap_int.h>            // Tipos de datos específicos de HLS
#include <hls_stream.h>        // Para la definición de streams de datos
#include "common/xf_utility.hpp"
#include <common/xf_structs.hpp>
#include <ap_axi_sdata.h>
#include "video_stab.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
// Definir tipo de matriz XF


// Función para leer datos del archivo y llenar el buffer de entrada
void read_txt_and_prepare_input(const char* filename, DataT* imgInput) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        return;
    }
    printf("Archivo cargado\n");

    // Variables temporales para almacenar los valores RGB
    unsigned int b, g, r;

    // Llenar el buffer de entrada
    for (int i = 0; i < HEIGHT * WIDTH; i++) {
        infile >> b >> g >> r; // Leer valores BGR
        ap_uint<32 * NPC1> pixel_pack = 0;

        // Empaquetar los valores RGB en un ap_uint<32 * NPC1>
        pixel_pack.range(7, 0) = b;   // Azul   
        pixel_pack.range(15, 8) = g;  // Verde
        pixel_pack.range(23, 16) = r; // Rojo
        // Si es necesario, agregar un valor extra para Alpha (por ejemplo, 255 para opacidad total)
        pixel_pack.range(31, 24) = 0; // Alpha (opacidad total)

        imgInput[i] = pixel_pack; // Asignar al buffer
    }

    infile.close();
}


// Función para escribir los resultados en un archivo de salida
void write_output_to_txt(const char* filename, ap_uint<8 * NPC1>* imgOutput) {
    std::ofstream outfile(filename);
    if (!outfile) {
        std::cerr << "Error al abrir el archivo de salida: " << filename << std::endl;
        return;
    }

    // Leer el buffer de salida y escribir los valores en el archivo
    for (int i = 0; i < (HEIGHT-1) * WIDTH; i++) {
        ap_uint<8 * NPC1> pixel = imgOutput[i];
        unsigned int gray = pixel.to_uint();  // Convertir a entero para escribir
        outfile << gray << " "; // Escribir el valor de intensidad (grayscale)
        if ((i + 1) % WIDTH == 0) {
            outfile << std::endl; // Nueva línea para cada fila
        }
    }

    outfile.close();
}




void print_imgInput(DataT* imgInput) {
    for (int i = 0; i < HEIGHT * WIDTH; i++) {
        // Desempaquetar los valores RGB
        unsigned int b = imgInput[i].range(23, 16);
        unsigned int g = imgInput[i].range(15, 8);
        unsigned int r = imgInput[i].range(7, 0);

        std::cout << "Pixel " << i << " -> R: " << r << ", G: " << g << ", B: " << b << std::endl;
    }
}

// Función principal
int main() {
    // Buffers de entrada y salida
    DataT imgInput[HEIGHT * WIDTH]; // Buffer de entrada con 32 bits por píxel
    ap_uint<8 * NPC1> imgOutput[HEIGHT * WIDTH-1]; // Buffer de salida con 8 bits por píxel

    // Leer datos desde el archivo .txt
    read_txt_and_prepare_input("img1_data.txt", imgInput);
    
    // Imprimir los valores de imgInput para verificar
    print_imgInput(imgInput);

    // Llamar a la función de estabilización (acelerador)
    printf("Buffer asignado\n");
    video_stab(imgInput, imgOutput);
    printf("video estabilizado\n");

    // Escribir los resultados en un archivo .txt
    write_output_to_txt("../../../../output_data.txt", imgOutput);
    printf("Archivo escrito\n");

    return 0;
}
