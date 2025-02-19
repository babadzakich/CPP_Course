/**
 * @file libcompressor.hpp
 * @brief Заголовочный файл библиотеки libcompressor.
 */
#ifndef LIBCOMPRR
#define LIBCOMPRR

/**
 * @brief Перечисление имеющихся алгоритмов сжатия.
 */
enum libcompressor_CompressionAlgorithm { libcompressor_zlib, libcompressor_bzip };

/**
 * @brief Контейнер с данными.
 */
typedef struct libcompressor_Buffer {
  char* data;
  int size;
} libcompressor_Buffer;

/**
 * @param algo - алгоритм сжатия
 * @param input - входные данные
 * @brief Функция сжатия разными алгоритмами.
 */
libcompressor_Buffer libcompressor_compress(libcompressor_CompressionAlgorithm algo, libcompressor_Buffer input);

#endif