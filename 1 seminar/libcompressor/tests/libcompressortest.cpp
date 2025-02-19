#include <gtest/gtest.h>
#include "libcompressor/libcompressor.hpp"

TEST(CompressorTest, zero_length_buffer_test) {
  char input[0] = {};

  libcompressor_Buffer buf;
  buf.data = input;
  buf.size = 0;

  libcompressor_Buffer res_bzip = libcompressor_compress(libcompressor_bzip, buf);

  ASSERT_TRUE(res_bzip.data == NULL);

  libcompressor_Buffer res_zlib = libcompressor_compress(libcompressor_zlib, buf);

  ASSERT_TRUE(res_zlib.data == NULL);
}

TEST(CompressorTest, empty_buffer_test) {
  char* input = NULL;

  libcompressor_Buffer buf;
  buf.data = input;
  buf.size = 11;

  libcompressor_Buffer res_bzip = libcompressor_compress(libcompressor_bzip, buf);

  ASSERT_TRUE(res_bzip.data == NULL);

  libcompressor_Buffer res_zlib = libcompressor_compress(libcompressor_zlib, buf);

  ASSERT_TRUE(res_zlib.data == NULL);
}

TEST(CompressorTest, bzip_compression_test) {
  char input_string[] = "test_string";
  unsigned char exp_res_bzip[] = {
      0x42, 0x5a, 0x68, 0x31, 0x31, 0x41, 0x59, 0x26, 0x53, 0x59, 0x4a, 0x7c, 0x69, 0x05, 0x00, 0x00,
      0x04, 0x83, 0x80, 0x00, 0x00, 0x82, 0xa1, 0x1c, 0x00, 0x20, 0x00, 0x22, 0x03, 0x68, 0x84, 0x30,
      0x22, 0x50, 0xdf, 0x04, 0x99, 0xe2, 0xee, 0x48, 0xa7, 0x0a, 0x12, 0x09, 0x4f, 0x8d, 0x20, 0xa0,
  };

  libcompressor_Buffer buf;
  buf.data = input_string;
  buf.size = 11;

  libcompressor_Buffer res_bzip = libcompressor_compress(libcompressor_bzip, buf);

  ASSERT_TRUE(!memcmp(res_bzip.data, exp_res_bzip, res_bzip.size));
}

TEST(CompressorTest, zlib_compression_test) {
  char input_string[] = "test_string";
  unsigned char exp_res_zlib[] = {
      0x78, 0x9c, 0x2b, 0x49, 0x2d, 0x2e, 0x89, 0x2f, 0x2e, 0x29, 0xca, 0xcc, 0x4b, 0x07, 0x00, 0x1c, 0x79, 0x04, 0xb7,
  };

  libcompressor_Buffer buf;
  buf.data = input_string;
  buf.size = 11;
  libcompressor_Buffer res_zlib = libcompressor_compress(libcompressor_zlib, buf);

  ASSERT_TRUE(!memcmp(res_zlib.data, exp_res_zlib, res_zlib.size));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
