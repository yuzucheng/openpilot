#!/usr/bin/bash
set -e
set -x

BUILD_DIR=/data/openpilot
cd $BUILD_DIR

# Cleanup
find . -name '*.a' -delete
find . -name '*.o' -delete
find . -name '*.os' -delete
find . -name '*.pyc' -delete
find . -name 'moc_*' -delete
find . -name '*.cc' -delete
find . -name '__pycache__' -delete
find selfdrive/ui/ -name '*.h' -delete
rm -rf .sconsign.dblite Jenkinsfile release/ || true
rm selfdrive/modeld/models/supercombo.onnx || true
rm -rf selfdrive/ui/replay/ || true

find third_party/ -name '*x86*' -exec rm -r {} +
find third_party/ -name '*Darwin*' -exec rm -r {} +

# Restore third_party
git checkout third_party/

# Mark as prebuilt release
touch prebuilt

echo "prebuilt done"
