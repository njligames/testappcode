#!/bin/bash

PLATFORM=$1
# BOT=$2
# CONFIGURATION=Debug
CONFIGURATION=Release
INSTALL_PREFIX=install

if [ "${PLATFORM}" != "android" ]
then
  BUILD_DIR=.build_$PLATFORM
  rm -rf $BUILD_DIR

  mkdir -p $BUILD_DIR
  cd $BUILD_DIR
fi

if [ "${PLATFORM}" == "emscripten" ]
then
    source ${EMSDK_DIR}/emsdk_env.sh

    export EMSCRIPTEN_INCLUDE_LOCATION=${EMSDK_DIR}/system/include

    emcmake cmake .. \
        -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
        -G "Ninja"

elif [ "${PLATFORM}" == "facebook" ]
then

  emcmake cmake .. \
  -DFACEBOOK-APP-ID="344740292600474" \
  -DFACEBOOK-API-VERSION="v2.12" \
  -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
  -DFACEBOOK:BOOL=TRUE \
  -G "Ninja"

elif [ "${PLATFORM}" == "windows64" ]
then

  cmake .. \
    -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
    -G "Visual Studio 14 2015 Win64"

elif [ "${PLATFORM}" == "windows32" ]
then

  cmake .. \
    -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
    -G "Visual Studio 14 2015"

elif [ "${PLATFORM}" == "macOS" ]
then

  cmake .. \
    -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
    -G "Xcode"

elif [ "${PLATFORM}" == "linux" ]
then
  export CC=/usr/bin/cc
  export CXX=/usr/bin/c++

  cmake .. \
    -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
    -G "Unix Makefiles"

elif [ "${PLATFORM}" == "ios" ]
then

  cmake .. \
    -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
    -G "Xcode" \
    -DIOS:BOOL=TRUE

elif [ "${PLATFORM}" == "appletv" ]
then

  cmake .. \
    -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
    -G "Xcode" \
    -DTVOS:BOOL=TRUE

elif [ "${PLATFORM}" == "android" ]
then

  rm -rf android/app/.externalNativeBuild

  cd android
  ./gradlew clean
  ./gradlew assemble${CONFIGURATION}

elif [ "${PLATFORM}" == "oculus_windows64" ]
then

  cmake .. \
    -DOCULUS=TRUE \
    -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
    -G "Visual Studio 14 2015 Win64"

elif [ "${PLATFORM}" == "oculus_windows32" ]
then

  cmake .. \
    -DOCULUS=TRUE \
    -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
    -G "Visual Studio 14 2015"

elif [ "${PLATFORM}" == "oculus_macOS" ]
then

  cmake .. \
    -DOCULUS=TRUE \
    -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
    -G "Xcode"

elif [ "${PLATFORM}" == "vr_ios" ]
then

  cmake .. \
    -DVR=TRUE \
    -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
    -G "Xcode" \
    -DIOS:BOOL=TRUE

else
  cmake -E env CFLAGS='-O0 -g' cmake .. -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX}
fi

cmake --build . --target clean
# cmake --build . --config ${CONFIGURATION} --target install
cmake --build . --config ${CONFIGURATION} --target package

cd ..

