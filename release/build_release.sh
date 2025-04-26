#!/usr/bin/bash
set -e
set -x

# git diff --name-status origin/release3-staging | grep "^A" | less

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null && pwd)"

cd $DIR

BUILD_DIR=/data/openpilot
SOURCE_DIR="$(git rev-parse --show-toplevel)"

if [ -z "$RELEASE_BRANCH" ]; then
  echo "RELEASE_BRANCH is not set"
  RELEASE_BRANCH=release-0971
fi


# set git identity
source $DIR/identity.sh
export GIT_SSH_COMMAND="ssh -i /data/gitkey"

echo "[-] Setting up repo T=$SECONDS"
rm -rf $BUILD_DIR
mkdir -p $BUILD_DIR
cd $BUILD_DIR
git init
# set git username/password
source $DIR/identity.sh
git remote add origin https://github.com/fishsp/openpilot.git
#git fetch origin $RELEASE_BRANCH
git fetch origin release-empty

# do the files copy
echo "[-] copying files T=$SECONDS"
cd $SOURCE_DIR
cp -pR --parents $(./release/release_files.py) $BUILD_DIR/

# in the directory
cd $BUILD_DIR

rm -f panda/board/obj/panda.bin.signed
rm -f panda/board/obj/panda_h7.bin.signed
rm -f panda/board/obj/bootstub.panda.bin
rm -f panda/board/obj/bootstub.panda_h7.bin

VERSION=$(cat common/version.h | awk -F[\"-]  '{print $2}')
echo "#define COMMA_VERSION \"$VERSION-release\"" > common/version.h

echo "[-] committing version $VERSION T=$SECONDS"
git add -f .
git commit -a -m "sunnypilot v$VERSION release"
#git branch --set-upstream-to=origin/$RELEASE_BRANCH
git branch --set-upstream-to=origin/release-empty

# Build
export PYTHONPATH="$BUILD_DIR"
scons -j$(nproc) --cache-disable

# release panda fw
scons -j$(nproc) --cache-disable panda/

echo "compile done"

# Ensure no submodules in release
if test "$(git submodule--helper list | wc -l)" -gt "0"; then
  echo "submodules found:"
  git submodule--helper list
  exit 1
fi
git submodule status

# Cleanup
find . -name '*.a' -delete
find . -name '*.o' -delete
find . -name '*.os' -delete
find . -name '*.pyc' -delete
find . -name 'moc_*' -delete
find . -name '*.cc' -delete
find . -name '__pycache__' -delete
find selfdrive/ui/ -name '*.h' -delete
rm -rf .sconsign.dblite Jenkinsfile release/
rm selfdrive/modeld/models/supercombo.onnx
rm -rf selfdrive/ui/replay/

find third_party/ -name '*x86*' -exec rm -r {} +
find third_party/ -name '*Darwin*' -exec rm -r {} +


# Restore third_party
git checkout third_party/

# Mark as prebuilt release
touch prebuilt

echo "prebuilt done"

# include source commit hash and build date in commit
GIT_HASH=$(git --git-dir=$SOURCE_DIR/.git rev-parse HEAD)
DATETIME=$(date '+%Y-%m-%dT%H:%M:%S')
SP_VERSION=$(cat $SOURCE_DIR/common/version.h | awk -F\" '{print $2}')

# Add built files to git
git add -f .
git commit --amend -m "sunnypilot v$VERSION"
git branch -m $RELEASE_BRANCH

# Run tests
#TEST_FILES="tools/"
#cd $SOURCE_DIR
#cp -pR -n --parents $TEST_FILES $BUILD_DIR/
#cd $BUILD_DIR
#RELEASE=1 pytest -n0 -s selfdrive/test/test_onroad.py
#system/manager/test/test_manager.py
#pytest selfdrive/car/tests/test_car_interfaces.py
#rm -rf $TEST_FILES

export GIT_SSH_COMMAND="ssh -i /data/gitkey"
echo "GIT_SSH_COMMAND = $GIT_SSH_COMMAND"

if [ ! -z "$RELEASE_BRANCH" ]; then
  echo "[-] pushing release T=$SECONDS"
  git push -f origin $RELEASE_BRANCH:$RELEASE_BRANCH
fi

echo "[-] done T=$SECONDS"
