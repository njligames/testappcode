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

### Game Source file locations
* `./src/`
    * Source files go into this directory.
* `./src/include/`
    * **PRIVATE** include files go into this directory.
* `./src/public_include/`
    * **PUBLIC** include files go into this directory.
* `./src/platform/${PLATFORM_NAME}/include/`
    * Platform specific **PRIVATE** include files go into this directory.
* `./src/platform/${PLATFORM_NAME}/public_include/`
    * Platform specific **PUBLIC** include files go into this directory.

### Unit-Test Game Source file locations
* `./test/`
    * Source and Header *test* files go into this directory. The tests should be testing what is in the ./src files only.

### Wrapped Game Source file locations
* `./swig.in/swig.h`
    * The include files that are stored in the .src/public_include .

### Thirdparty library locations
* `./thirdparty/`
    * The thirdparty include files and libraries. In the future, this will be downloaded.
