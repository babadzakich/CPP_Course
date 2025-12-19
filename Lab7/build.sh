rm -r build
mkdir build
cd build
conan install .. --install-folder=. --build=missing
cmake .. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build . --target=docs
cmake --build . --target=format
cmake --build .
cmake --install . --prefix ./install
ctest -V -j4
