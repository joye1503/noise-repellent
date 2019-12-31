#!/bin/bash

set -e

# Preparation

# Build now
meson build --buildtype release
cd build
ninja -v

# Move needed sources to bin folder
cd .. && mkdir -p bin/noise-repellent.lv2
cp build/nrepel.* ./bin/noise-repellent.lv2
cp build/manifest.ttl ./bin/noise-repellent.lv2
cp lv2ttl/nrepel.ttl ./bin/noise-repellent.lv2

# Compress build in a zip file
zip -9 -jr noise-repellent-linux.zip ./bin/

# Clean
rm -rf build bin || true