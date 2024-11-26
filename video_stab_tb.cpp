#include <iostream>
#include <stdint.h>
#include <hls_stream.h>
#include <cassert>
#include "video_stab.h" // Archivo que contiene la función `video_stab`

#define WIDTH 1280
#define HEIGHT 720
#define BUFFER_SIZE (WIDTH * HEIGHT / (BITS_DATA / 8))



int main() {
    // Configuración inicial
    int width = WIDTH;
    int height = HEIGHT;
    int bufferSize = (width * height) / (BITS_DATA / 8); // Tamaño del buffer

    // Reservar memoria para datos de entrada y salida
    DataT* inBuffer = new DataT[bufferSize];
    DataT* outBuffer = new DataT[bufferSize];
    
    // Inicializar los datos de entrada con valores aleatorios
    srand(42); // Semilla para reproducibilidad
    for (int i = 0; i < bufferSize; i++) {
        inBuffer[i] = rand() % 256; // Valores de 8 bits (0-255)
    }

    // Inicializar el buffer de salida con ceros
    memset(outBuffer, 0, bufferSize * sizeof(DataT));

    // Llamar al kernel
    video_stab(inBuffer, outBuffer, width, height);

    // Verificar y mostrar resultados
    std::cout << "Resultados del filtro:" << std::endl;
    for (int i = 0; i < 10; i++) { // Mostrar los primeros 10 resultados
        std::cout << "Pixel " << i << ": Entrada = " << inBuffer[i]
                  << ", Salida = " << outBuffer[i] << std::endl;
    }

    // Liberar memoria
    delete[] inBuffer;
    delete[] outBuffer;

    return 0;
}