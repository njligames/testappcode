#!/bin/bash

MY_BUILT_LIBRARY=$1

MY_ANDROID_NDK=/Users/jamesfolk/Library/Android/sdk/ndk-bundle/
MY_ANDROID_NATIVE_API_LEVEL=29
CMAKE_BIN=/Users/jamesfolk/Library/Android/sdk//cmake/3.10.2.4988404/bin/cmake

_build_android()
{
    MY_BUILD_TYPE=$1
    MY_ABI=$2
    MY_TOOLS_SUFFIX=$3

    mkdir -p .build_android
    cd .build_android

    export LUA_DIR=/Users/jamesfolk/Work/third-party_libs/BUILD/lib/lua/android/28/Release/${MY_ABI}

    $CMAKE_BIN .. \
        ${CMAKE_LIB_OPTIONS} \
        -DCMAKE_INSTALL_PREFIX=./BUILD/lib/${MY_BUILT_LIBRARY}/android/${MY_ANDROID_NATIVE_API_LEVEL}/Release/${MY_ABI}\
        -DANDROID_ABI=${MY_ABI} \
        -DANDROID_NATIVE_API_LEVEL=${MY_ANDROID_NATIVE_API_LEVEL} \
        -DANDROID_STL=c++_shared \
        -DCMAKE_TOOLCHAIN_FILE=${MY_ANDROID_NDK}/build/cmake/android.toolchain.cmake \
        -DANDROID:BOOL=ON

    # $CMAKE_BIN -v --build . --config ${MY_BUILD_TYPE}
    $CMAKE_BIN --build .

    cd ..

}

build_android_abi()
{

  # ABIS=('armeabi-v7a' 'x86' 'arm64-v8a' 'x86_64' )
  # BUILD_TYPES=(Debug Release MinsizeRel RelWithDebugInfo)
  ABIS=('x86_64')
  BUILD_TYPES=(Release)

  for ABI in ${ABIS[@]};do

      if [ ${ABI} = 'armeabi-v7a' ]; then
        TOOLS=arm
      elif [ ${ABI} = 'x86' ]; then
        TOOLS=x86
      elif [ ${ABI} = 'mips' ]; then
        TOOLS=mips
      elif [ ${ABI} = 'arm64-v8a' ]; then
        TOOLS=arm64
      elif [ ${ABI} = 'x86_64' ]; then
        TOOLS=x86_64
      elif [ ${ABI} = 'mips64' ]; then
        TOOLS=mips64
      else
        echo "choose one for the second parameter (ABI for android): armeabi-v7a x86 mips arm64-v8a x86_64 mips64."
      fi

      if [ ! -z "$TOOLS" ]; then
          for BUILD_TYPE in ${BUILD_TYPES[@]};do
              _build_android ${BUILD_TYPE} ${ABI} ${TOOLS}
          done
      fi

  done
}

rm -rf .build_android
build_android_abi


