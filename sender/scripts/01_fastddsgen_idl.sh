#/bin/bash

MODULE_ROOT_DIR="$(dirname $(dirname $(realpath $0)))"

echo "============================================================"
echo "Starting FastDDSGen"
echo "============================================================"

mkdir -p "$MODULE_ROOT_DIR/idl/srcs"
fastddsgen "$MODULE_ROOT_DIR/idl/idl/HelloWorld.idl" -replace -d "$MODULE_ROOT_DIR/idl/srcs/"
