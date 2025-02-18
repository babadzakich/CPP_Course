/**
 * @file libcompressor.cpp
 * @brief Исходный файл библиотеки для сжатия данных.
 */
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <bzlib.h>
#include <zlib.h>

#include "libcompressor/libcompressor.hpp"

/**
 * @brief Сжимает данные с использованием выбранного алгоритма.
 *
 * @param algo Алгоритм сжатия.
 * @param input Входные данные.
 * @return libcompressor_Buffer Сжатые данные.
 */
libcompressor_Buffer libcompressor_compress(libcompressor_CompressionAlgorithm algo, libcompressor_Buffer input) {
    libcompressor_Buffer result_buffer;
    
    if (input.size == 0 || input.data == NULL) {
        result_buffer.data = NULL;
        result_buffer.size = 0;
        return result_buffer;
    }

    result_buffer.data = (char*)malloc(input.size + 1024);

    if (algo == libcompressor_zlib) {
        z_stream data;
        data.zalloc = Z_NULL;
        data.zfree = Z_NULL;
        data.opaque = Z_NULL;
        
        if (deflateInit(&data, Z_DEFAULT_COMPRESSION) != Z_OK) {
            result_buffer.data = NULL;
            result_buffer.size = 0;
            return result_buffer;
        }

        result_buffer.size = deflateBound(&data, input.size);
        
        data.avail_in = input.size;
        data.next_in = (Bytef*)input.data;
        data.avail_out = result_buffer.size;
        data.next_out = (Bytef*)result_buffer.data;

        if (deflate(&data, Z_FINISH) != Z_STREAM_END) {
            free(result_buffer.data);
            deflateEnd(&data);
            result_buffer.size = 0;
            return result_buffer;
        }
        result_buffer.size = data.total_out;

        deflateEnd(&data);
    } else {
        if (BZ2_bzBuffToBuffCompress(result_buffer.data, (unsigned int*)&result_buffer.size, input.data, (unsigned int)input.size, 1, 0, 0) != BZ_OK) {
            result_buffer.size = 0;
            free(result_buffer.data);
        }
    }
    return result_buffer;
}