#!/usr/bin/bash
set -e
set -x

BUILD_DIR=/data/openpilot_release
SOURCE_DIR="$(git rev-parse --show-toplevel)"

# do the files copy
echo "[-] copying files T=$SECONDS"
cd $SOURCE_DIR
cp -pR --parents $(./release/release_files.py) $BUILD_DIR/

echo "copy done"

