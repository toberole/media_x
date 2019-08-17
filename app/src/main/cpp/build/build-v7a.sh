#! /bin/sh

#make clean
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain-armv7a.cmake -DCMAKE_BUILD_TYPE=Release -DDLIB_USE_CUDA=0  ../
make -j 8
