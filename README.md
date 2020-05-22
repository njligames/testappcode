# Game Template

## File Locations

### {PLATFORM_NAME} - platform specific directory name. 
One of: 
* emscripten
* windows64
* windows32
* ios
* appletvos
* macos
* linux
* unix
* android

### C++ Game Source file locations
* ${ROOT}/src
    * All c++ compiled files go into this directory.
* ${ROOT}/src/include
* ${ROOT}/src/public_include
* ${ROOT}/src/platform/${PLATFORM_NAME}/include
* ${ROOT}/src/platform/${PLATFORM_NAME}/public_include

### C++ Unit-Test Game Source file locations
* ${ROOT}/test

### C++ Wrapped Game Source file locations
* ${ROOT}/swig.in/swig.h

* platform - Store platform specific source and header files.
    PLATFORM - platform specific directory name. 
    One of: 
    * emscripten
    * windows64
    * windows32
    * ios
    * appletvos
    * macos
    * linux
    * unix
    * android
        include - Store platform specific build include files.
        public_include - Store public interface include files.
* src - Store general source and header files.
    include - Store platform specific build include files.
    public_include - Store public interface include files.
* thirdparty - Store thirdparty header and library files.

