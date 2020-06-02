
* ![android-x86_64-swig-lua](https://github.com/njligames/testappcode/workflows/android-x86_64-swig-lua/badge.svg?branch=master)
* ![android-x86-swig-lua](https://github.com/njligames/testappcode/workflows/android-x86-swig-lua/badge.svg?branch=master)
* ![android-armeabi-v7a--swig-lua](https://github.com/njligames/testappcode/workflows/android-armeabi-v7a--swig-lua/badge.svg?branch=master)
* ![android-arm64-v8a-swig-lua](https://github.com/njligames/testappcode/workflows/android-arm64-v8a-swig-lua/badge.svg?branch=master)
* ![macOS-swig-lua](https://github.com/njligames/testappcode/workflows/macOS-swig-lua/badge.svg?branch=master)
* ![ios-swig-lua](https://github.com/njligames/testappcode/workflows/ios-swig-lua/badge.svg?branch=master)
* ![appletv-swig-lua](https://github.com/njligames/testappcode/workflows/appletv-swig-lua/badge.svg?branch=master)
* ![ios-vr-swig-lua](https://github.com/njligames/testappcode/workflows/ios-vr-swig-lua/badge.svg?branch=master)

# Game Template

### `{PLATFORM_NAME}` - platform specific directory name. 
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

## Adding new files.

### Source

* Put the `.cpp` file into `./src/`
* If you want the header to be a part of the library interface put the `.h` into `./public_include/`
    * If platform specific, put the `.h` into `./src/platform/${PLATFORM_NAME}/public_include/`
* If you want the header to only be a part of the build interface put the `.h` into `./include/`
    * If platform specific, put the `.h` into `./src/platform/${PLATFORM_NAME}/include/`
* If you want the header to be wrapped by swig, make sure to include it's header into `./swig.in/swig.h`

### Third Party
* Put the third party include into `./thirdparty/includes`
* Put the third party libraries into `./thirdparty/platform/${PLATFORM_NAME}`

## File Locations

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

## Testing
### Example Test files.
* `./src/public_include/Color.h`
    * The source file to be tested.
* `./test/TestColor.h`
    * GoogleTest class.
* `./test/lua/main.lua`
    * The Color class was included in `swig.in/swig.h`
    * Testing for the lua wrapper. (lua module must be put into lua_path)

## Code Formatting
All of the source code is formatted with the clang formatter. To change how the code will be formatted, edit: `./.clang-format`
## Code Documentation
All of the source code is documentated using Doxygen. To change how the Doxygen file is run, edit: `./doc/doxygen/Doxyfile.in`
