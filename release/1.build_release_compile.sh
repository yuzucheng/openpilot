#!/usr/bin/bash
set -e
set -x

BUILD_DIR=/data/openpilot_release

# in the directory
cd $BUILD_DIR

# Build
export PYTHONPATH="$BUILD_DIR"
scons -j$(nproc) --cache-disable

# release panda fw
scons -j$(nproc) --cache-disable panda/

echo "compile done"
