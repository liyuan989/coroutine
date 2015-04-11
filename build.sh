#!/bin/sh

set -x

CURRENT_DIR=`pwd`
BUILD_TYPE=${BUILD_TYPE:-release}
BUILD_BIR=${CURRENT_DIR}/build/${BUILD_TYPE}

if [ ${BUILD_TYPE} != "release" ] && [ ${BUILD_TYPE} != "debug" ]
then
    echo "Usage: BUILD_TYPE=debug/release"
else
    mkdir -p ${BUILD_BIR}
    cd ${BUILD_BIR}
    cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
          ${CURRENT_DIR}
    make $*
fi
