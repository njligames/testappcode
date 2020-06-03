cmake_minimum_required(VERSION 3.10.2)

project(game VERSION 1.0.0.0)

set(${PROJECT_NAME}_BUILD_DOC OFF CACHE BOOL "Build the framework library")

set(${PROJECT_NAME}_SWIG_LUA ON CACHE BOOL "Wrap it for lua")
set(${PROJECT_NAME}_SWIG_JAVA OFF CACHE BOOL "wrap it for java")
set(${PROJECT_NAME}_SWIG_PHP OFF CACHE BOOL "wrap it for php")
set(${PROJECT_NAME}_SWIG_JAVASCRIPT OFF CACHE BOOL "wrap it for javascript")

set(${PROJECT_NAME}_SWIG_PYTHON OFF CACHE BOOL "wrap it for python")
set(${PROJECT_NAME}_SWIG_GO OFF CACHE BOOL "wrap it for go")
set(${PROJECT_NAME}_SWIG_RUBY OFF CACHE BOOL "wrap it for ruby")
set(${PROJECT_NAME}_SWIG_OCTAVE OFF CACHE BOOL "wrap it for octave")
set(${PROJECT_NAME}_SWIG_CSHARP OFF CACHE BOOL "wrap it for csharp")
set(${PROJECT_NAME}_SWIG_PERL OFF CACHE BOOL "wrap it for perl")

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.13.0)
    cmake_policy(SET CMP0078 NEW)
    cmake_policy(SET CMP0086 NEW)
endif()

message(STATUS "VERSION (${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK})")

# Check for 64 or 32 bit
set(SIZEOF_VOIDP ${CMAKE_SIZEOF_VOID_P})
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(ARCH_64 TRUE)
  set(PROCESSOR_ARCH "x64")
else()
  set(ARCH_64 FALSE)
  set(PROCESSOR_ARCH "x86")
endif()

include(CheckCXXCompilerFlag)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/detectGL.cmake)

# message(FATAL_ERROR "CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}")

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(THIRDPARTY_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")

file(GLOB SRCS ${SRC_DIR}/*.c*)
file(GLOB HDRS ${SRC_DIR}/include/*.h)
file(GLOB PUBLIC_HDRS ${SRC_DIR}/public_include/*.h)

set(GAME_INCLUDE_DIRS ${SRC_DIR}/include)
list(APPEND GAME_INCLUDE_DIRS ${SRC_DIR}/public_include)

set(BUILT_GTEST FALSE)

if(NOT DEFINED ${TARGET_PLATFORM})
    if(EMSCRIPTEN)
        set(BUILT_GTEST TRUE)

        set(TARGET_PLATFORM "emscripten")
        list(APPEND GAME_DEFINITIONS "__EMSCRIPTEN__=1")
        if(VR)
            list(APPEND GAME_DEFINITIONS "__VR__=1")
        endif()
    elseif(WIN32)
        set(BUILT_GTEST TRUE)

        if(ARCH_64)
            set(TARGET_PLATFORM "windows")
            list(APPEND GAME_DEFINITIONS "__WINDOWS64__=1")
            if(OCULUS)
                list(APPEND GAME_DEFINITIONS "__VR__=1")
            endif()
        else()
            set(TARGET_PLATFORM "windows")
            list(APPEND GAME_DEFINITIONS "__WINDOWS32__=1")
            if(OCULUS)
                list(APPEND GAME_DEFINITIONS "__VR__=1")
            endif()
        endif()
    elseif(APPLE)
        if(IOS)
            set(BUILT_GTEST TRUE)

            set(TARGET_PLATFORM "ios")
            list(APPEND GAME_DEFINITIONS "__IPHONEOS__=1")
            if(VR)
                list(APPEND GAME_DEFINITIONS "__VR__=1")
            endif()
        elseif(TVOS)
            set(TARGET_PLATFORM "appletvos")
            list(APPEND GAME_DEFINITIONS "__APPLETVOS__=1")
        else()
            set(BUILT_GTEST TRUE)

            set(TARGET_PLATFORM "macos")
            list(APPEND GAME_DEFINITIONS "__MACOS__=1")
            if(OCULUS)
                list(APPEND GAME_DEFINITIONS "__VR__=1")
            endif()
        endif()
    elseif(UNIX AND NOT APPLE AND NOT ANDROID)
        set(BUILT_GTEST TRUE)

        if(CMAKE_SYSTEM_NAME MATCHES ".*Linux")
            set(TARGET_PLATFORM "linux")
            list(APPEND GAME_DEFINITIONS "__LINUX__=1")
            if(OCULUS)
                list(APPEND GAME_DEFINITIONS "__VR__=1")
            endif()
        elseif(UNIX)
            set(TARGET_PLATFORM "unix")
            list(APPEND GAME_DEFINITIONS "__UNIX__=1")
        endif()
    elseif(ANDROID)
        set(BUILT_GTEST TRUE)
        set(TARGET_PLATFORM "android")
        list(APPEND GAME_DEFINITIONS "__ANDROID__=1")
    endif()
endif()

set(PLATFORM_DIR "${SRC_DIR}/platform/${TARGET_PLATFORM}")

file(GLOB PLATFORM_PUBLIC_HDRS ${PLATFORM_DIR}/public_include/*.h)

set(GAME_PUBLIC_HEADER ${PUBLIC_HDRS} ${PLATFORM_PUBLIC_HDRS})

set(GAME_PUBLIC_DIRS ${SRC_DIR}/public_include)
set(GAME_PLATFORM_PUBLIC_DIRS ${PLATFORM_DIR}/public_include)

set(INCLUDE_DIRS ${GAME_PUBLIC_DIRS} ${GAME_PLATFORM_PUBLIC_DIRS})

list(APPEND GAME_INCLUDE_DIRS ${THIRDPARTY_INCLUDE_DIR}/includes/SDL2)

list(APPEND GAME_INCLUDE_DIRS ${THIRDPARTY_INCLUDE_DIR}/includes/rapidjson)
list(APPEND GAME_INCLUDE_DIRS ${THIRDPARTY_INCLUDE_DIR}/includes/nlohmann)
list(APPEND GAME_INCLUDE_DIRS ${THIRDPARTY_INCLUDE_DIR}/includes/stb)
list(APPEND GAME_INCLUDE_DIRS ${THIRDPARTY_INCLUDE_DIR}/includes/glm)
list(APPEND GAME_INCLUDE_DIRS ${THIRDPARTY_INCLUDE_DIR}/includes/NJLICEngine)

if(BUILT_GTEST)
    # Download and unpack googletest at configure time
    configure_file(googletest.cmake.in
            googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )

    # Prevent GoogleTest from overriding our compiler/linker options
    # when building with Visual Studio
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This adds
    # the following targets: gtest, gtest_main, gmock
    # and gmock_main
    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
            ${CMAKE_CURRENT_BINARY_DIR}/googletest-build)

    # The gtest/gmock targets carry header search path
    # dependencies automatically when using CMake 2.8.11 or
    # later. Otherwise we have to add them here ourselves.
    if (CMAKE_VERSION VERSION_LESS 2.8.11)
        include_directories("${gtest_SOURCE_DIR}/include"
                "${gmock_SOURCE_DIR}/include")
    endif()
    list(APPEND GAME_INCLUDE_DIRS ${gtest_SOURCE_DIR}/include)
    list(APPEND GAME_INCLUDE_DIRS ${gmock_SOURCE_DIR}/include)
endif()


file(GLOB_RECURSE SRCS_TEST_CPP ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp)
file(GLOB_RECURSE SRCS_TEST_C ${CMAKE_CURRENT_SOURCE_DIR}/test/*.c)
file(GLOB_RECURSE HDRS_TEST ${CMAKE_CURRENT_SOURCE_DIR}/test/*.h)
set( GAME_SOURCES_TEST ${SRCS_TEST_CPP} ${SRCS_TEST_C} ${HDRS_TEST} )

if(${PROJECT_NAME}_BUILD_DOC)
    add_subdirectory(doc)
endif()

set(${PROJECT_NAME}_SWIG OFF)

if(${PROJECT_NAME}_SWIG_LUA)
    set(${PROJECT_NAME}_SWIG ON)
endif()

if(${PROJECT_NAME}_SWIG_JAVA)
    set(${PROJECT_NAME}_SWIG ON)
endif()

if(${PROJECT_NAME}_SWIG_PHP)
    set(${PROJECT_NAME}_SWIG ON)
endif()

if(${PROJECT_NAME}_SWIG_JAVASCRIPT)
    set(${PROJECT_NAME}_SWIG ON)
endif()

if(${PROJECT_NAME}_SWIG)

    find_package(SWIG REQUIRED)
    include(UseSWIG)

    if(${SWIG_FOUND})
        list(APPEND SWIG_WRAP_INCLUDE ${GAME_PUBLIC_DIRS})
        list(APPEND SWIG_WRAP_LIBRARIES ${GAME_LIBRARIES} ${PROJECT_NAME}-static)

        MESSAGE(STATUS "SWIG_FOUND ${SWIG_FOUND}")
        MESSAGE(STATUS "SWIG_DIR ${SWIG_DIR}")
        MESSAGE(STATUS "SWIG_EXECUTABLE ${SWIG_EXECUTABLE}")
        MESSAGE(STATUS "SWIG_VERSION ${SWIG_VERSION}")

        include(${SWIG_USE_FILE})

        if(${PROJECT_NAME}_SWIG_LUA)
            include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/swig/swig_lua.cmake)
        endif()

        if(${PROJECT_NAME}_SWIG_PYTHON)
            include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/swig/swig_python.cmake)
        endif()

        if(${PROJECT_NAME}_SWIG_JAVA)
            include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/swig/swig_java.cmake)
        endif()

        if(${PROJECT_NAME}_SWIG_GO)
            include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/swig/swig_go.cmake)
        endif()

        if(${PROJECT_NAME}_SWIG_RUBY)
            include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/swig/swig_ruby.cmake)
        endif()

        if(${PROJECT_NAME}_SWIG_OCTAVE)
            include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/swig/swig_octave.cmake)
        endif()

        if(${PROJECT_NAME}_SWIG_CSHARP)
            include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/swig/swig_csharp.cmake)
        endif()

        if(${PROJECT_NAME}_SWIG_JAVASCRIPT)
            include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/swig/swig_javascript.cmake)
        endif()

        if(${PROJECT_NAME}_SWIG_PHP)
            include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/swig/swig_php.cmake)
        endif()

        if(${PROJECT_NAME}_SWIG_PERL)
            include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/swig/swig_perl.cmake)
        endif()

        set(${SWIG_WRAP_LANG}_WRAP_TEST_IN ${CMAKE_CURRENT_SOURCE_DIR}/test/${SWIG_WRAP_LANG}/main.${SWIG_WRAP_LANG})
        include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/swig/swig.cmake)

    endif()
endif()

add_subdirectory(src/platform)

########################################################
###                   PACKAGING                      ###
### This is a quite INCOMPLETE set of variables that ###
### should be set for the various generators.        ###
### Consult the CPack documentations for a full set. ###
########################################################
if (WIN32)
    if (USE_WIX_TOOLSET)
        set(CPACK_GENERATOR "WIX") # this need WiX Tooset installed and a path to candle.exe
    else ()
        set(CPACK_GENERATOR "NSIS") # this needs NSIS installed, and available
    endif ()
    set(CPACK_SOURCE_GENERATOR "ZIP")
elseif ( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    # set(CPACK_GENERATOR "productbuild")
    set(CPACK_SOURCE_GENERATOR "ZIP")
else ()
    set(CPACK_SOURCE_GENERATOR "ZIP")
endif ()

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "James Folk") #required

set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.html")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_SOURCE_DIR}/README.html")

set(CPACK_SOURCE_IGNORE_FILES "${CMAKE_SOURCE_DIR}/build/;${CMAKE_SOURCE_DIR}/.git/")

include(CPack)
