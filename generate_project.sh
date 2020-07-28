#!/bin/bash

PLATFORM=$1
# BOT=$2
CONFIGURATION=Debug
# CONFIGURATION=Release
INSTALL_PREFIX=install

function unittest {
    exit
}

if [ "${PLATFORM}" != "android" ]
then
  BUILD_DIR=.build_$PLATFORM
  rm -rf $BUILD_DIR

  mkdir -p $BUILD_DIR
  cd $BUILD_DIR
fi

if [ "${PLATFORM}" == "emscripten" ]
then
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

elif [ "${PLATFORM}" == "windows" ]
then

  cmake .. \
    -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
    -G "Visual Studio 16 2019"

elif [ "${PLATFORM}" == "macOS" ]
then
    cmake .. \
        -Dgame_DOC:BOOL=ON \
        -Dgame_TEST:BOOL=ON \
        -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
        -G "Xcode"

    function unittest {
        ./.build_macOS/src/platform/macos/test/cpp/Debug/game-test-static --gtest_output="xml" &
        # sleep 2
        # /usr/local/Cellar/python/3.7.7/Frameworks/Python.framework/Versions/3.7/bin/screenshot game-test-static

        test=$(echo 'cat //failure/@message' | xmllint --shell "test_detail.xml" | grep -v ">")

        if [ -z "$test" ]
        then
            echo "\$test is empty; PASSED!"
        else
            echo "\$test is NOT empty; FAILED!"
        fi
    }

elif [ "${PLATFORM}" == "linux" ]
then
  export CC=/usr/bin/cc
  export CXX=/usr/bin/c++

  cmake .. \
      -Dgame_DOC:BOOL=ON \
      -Dgame_TEST:BOOL=ON \
      -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
      -G "Unix Makefiles"

    function unittest {
        ./.build_linux/src/platform/linux/test/cpp/game-test-static --gtest_output="xml"
        # sleep 2
        # /usr/local/Cellar/python/3.7.7/Frameworks/Python.framework/Versions/3.7/bin/screenshot game-test-static

        test=$(echo 'cat //failure/@message' | xmllint --shell "test_detail.xml" | grep -v ">") || true

        if [ -z "$test" ]
        then
            echo "PASSED!"
        else
            echo "FAILED!"
            echo $test
        fi
    }

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

elif [ "${PLATFORM}" == "oculus_windows" ]
then

  cmake .. \
    -DOCULUS=TRUE \
    -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} \
    -G "Visual Studio 16 2019"

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
cmake --build . --config ${CONFIGURATION} --target install
cmake --build . --config ${CONFIGURATION} --target package

cd ..

unittest

