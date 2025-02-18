/**
 * @file compressor.cpp
 * @brief Исполняемый файл для сжатия строки.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <spdlog/spdlog.h>

#include "libcompressor/libcompressor.hpp"
/**
 * @param argc - Количество аргументов должно быть ровно 3
 * @param argv - Аргументы, где 1 аргумент - указание алгоритма сжатия, а 2 аргумент - строка для сжатия.
 * @brief Сжимает строку
 */
int main(int argc, char** argv) {
    if (argc != 3) {
        spdlog::error("Not enough arguments(should be 2)");
        exit(-1);
    }    

    libcompressor_CompressionAlgorithm algo;
    libcompressor_Buffer buff;
    buff.data = argv[2];
    buff.size = strlen(argv[2]);

    if (!strcmp(argv[1], "zlib")) {
        algo = libcompressor_zlib;
    } else if (!strcmp(argv[1], "bzip")) {
        algo = libcompressor_bzip;
    } else {
        spdlog::error("User must use zlib or bzip only");
        exit(-1);
    }

    libcompressor_Buffer result = libcompressor_compress(algo, buff);

    if (result.data == NULL) {
        spdlog::error("Empty buffer returned");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < result.size; i++)
    {
        printf("%0.2hhx", result.data[i]);
    }
    printf("\n");
    free(result.data);
    return EXIT_SUCCESS;
}