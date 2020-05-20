cmake_minimum_required(VERSION 2.8.8)

project(game)

include(CheckCXXCompilerFlag)

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
set(THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")

file(GLOB SRCS ${SRC_DIR}/*.c*)
file(GLOB HDRS ${SRC_DIR}/include/*.h)
file(GLOB PUBLIC_HDRS ${SRC_DIR}/public_include/*.h)

set(GAME_INCLUDE_DIRS ${SRC_DIR}/include)
list(APPEND GAME_INCLUDE_DIRS ${SRC_DIR}/public_include)

list(APPEND GAME_INCLUDE_DIRS ${THIRDPARTY_DIR}/includes/glm)
list(APPEND GAME_INCLUDE_DIRS ${THIRDPARTY_DIR}/includes/NJLICEngine)

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
            set(TARGET_PLATFORM "windows64")
            list(APPEND GAME_DEFINITIONS "__WINDOWS64__=1")
            if(OCULUS)
                list(APPEND GAME_DEFINITIONS "__VR__=1")
            endif()
        else()
            set(TARGET_PLATFORM "windows32")
            list(APPEND GAME_DEFINITIONS "__WINDOWS32__=1")
            if(OCULUS)
                list(APPEND GAME_DEFINITIONS "__VR__=1")
            endif()
        endif()
    elseif(APPLE)
        if(IOS)
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

        if(LINUX)
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
        set(TARGET_PLATFORM "android")
        list(APPEND GAME_DEFINITIONS "__ANDROID__=1")
    endif()
endif()

set(PLATFORM_DIR "${SRC_DIR}/platform/${TARGET_PLATFORM}")

file(GLOB PLATFORM_PUBLIC_HDRS ${PLATFORM_DIR}/public_include/*.h)

set(GAME_PUBLIC_HEADER ${PUBLIC_HDRS} ${PLATFORM_PUBLIC_HDRS})

set(GAME_PUBLIC_DIRS ${PLATFORM_DIR}/public_include ${SRC_DIR}/public_include)

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
endif()


add_subdirectory(src/platform)

