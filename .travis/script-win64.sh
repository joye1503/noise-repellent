#!/bin/bash

set -e

# Preparation
_FLAGS="-msse -msse2 -mfpmath=sse -ffast-math -fomit-frame-pointer -fno-finite-math-only \
        -DPTW32_STATIC_LIB -Werror -I/opt/mingw64/x86_64-w64-mingw32/include -I/opt/mingw64/include"
_PREFIX=x86_64-w64-mingw32
export AR=${_PREFIX}-ar
export CC=${_PREFIX}-gcc
export CFLAGS="${_FLAGS}"
export PATH=/opt/mingw64/${_PREFIX}/bin:/opt/mingw64/bin:${PATH}
export CROSS_COMPILING=true
export STRIP=${_PREFIX}-strip
export LV2LDFLAGS="-Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed"
export LIB_EXT=.dll
export PKG_CONFIG_PATH=/opt/mingw64/lib/pkgconfig

# Start clean
make clean >/dev/null

# Build now
make

# Compress build in a zip file
zip -9 -jr noise-repellent-win64.zip ./build/*
