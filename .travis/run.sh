#!/bin/bash

set -e
set -x

mkdir build && cd build
conan install .. --build missing
cmake -DCMAKE_BUILD_TYPE=Release -DReplay_USE_CONAN=ON -DReplay_ENABLE_UNIT_TESTS=OFF -DReplay_USE_STBIMAGE=OFF -DReplay_USE_LIBPNG=OFF ..
cmake --build .
