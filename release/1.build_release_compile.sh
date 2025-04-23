#!/usr/bin/bash
set -e
set -x

BUILD_DIR=/data/openpilot_release

# in the directory
cd $BUILD_DIR

# Build
export PYTHONPATH="$BUILD_DIR"
scons -j$(nproc)

# release panda fw
scons -j$(nproc) panda/

echo "compile done"
