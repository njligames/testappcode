add_library(SDL_test UNKNOWN IMPORTED)
set_property(TARGET SDL_test APPEND PROPERTY IMPORTED_LOCATION  "${THIRDPARTY_INCLUDE_DIR}/platform/${TARGET_PLATFORM}/libSDL2_test.a")
list(APPEND TARGET_LIBRARIES SDL_test)

list(APPEND GAME_INCLUDE_DIRS ${GTEST_INCLUDE_DIRS})
list(APPEND GAME_INCLUDE_DIRS ${GMOCK_INCLUDE_DIRS})
list(APPEND GAME_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.c*)
file(GLOB_RECURSE HDRS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

list(APPEND GAME_SOURCES_TEST ${SRCS} ${HDRS})

set(CMAKE_CXX_FLAGS_DEBUG "--js-opts 0 -g4")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG --llvm-opts 3 --js-opts 1 --closure 1 -s IGNORE_CLOSURE_COMPILER_ERRORS=1")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG --llvm-opts 3 --js-opts 1 --closure 1 -s IGNORE_CLOSURE_COMPILER_ERRORS=1")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 --js-opts 0 -g4 -DNDEBUG")

set(CMAKE_C_FLAGS_DEBUG "--js-opts 0 -g4")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG --llvm-opts 3 --js-opts 1 --closure 1 -s IGNORE_CLOSURE_COMPILER_ERRORS=1")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG --llvm-opts 3 --js-opts 1 --closure 1 -s IGNORE_CLOSURE_COMPILER_ERRORS=1")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 --js-opts 0 -g4 -DNDEBUG")

# if(PTHREADS)
#   list(APPEND EXTRA_LDFLAGS "-s USE_PTHREADS=1")
#   list(APPEND EXTRA_LDFLAGS "-s PTHREAD_POOL_SIZE=8")
# endif()
# list(APPEND EXTRA_LDFLAGS "--no-check-features")

list(APPEND EXTRA_LDFLAGS "-s FULL_ES2=1")
list(APPEND EXTRA_LDFLAGS "-s TOTAL_MEMORY=1GB")

list(APPEND EXTRA_LDFLAGS "-s ERROR_ON_UNDEFINED_SYMBOLS=0")
list(APPEND EXTRA_LDFLAGS "-s WASM=1")

list(APPEND EXTRA_FLAGS "--memory-init-file")

string(TOUPPER CMAKE_BUILD_TYPE CMAKE_BUILD_TYPE_UPPER)
if (CMAKE_BUILD_TYPE_UPPER EQUAL "DEBUG")
    list(APPEND EXTRA_LDFLAGS "--emrun")
    list(APPEND EXTRA_LDFLAGS "-s ASSERTIONS=2")
    list(APPEND EXTRA_LDFLAGS "-s DEMANGLE_SUPPORT=1")
    list(APPEND EXTRA_LDFLAGS "-s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=1")
    list(APPEND EXTRA_LDFLAGS "--source-map-base")
endif ()

if (CMAKE_BUILD_TYPE_UPPER EQUAL "RELEASE")
    list(APPEND EXTRA_LDFLAGS "--profiling")
endif ()

# configure_file("${CMAKE_CURRENT_SOURCE_DIR}/resources/release_shell.html.in" "${CMAKE_BINARY_DIR}/release_shell.html")
# list(APPEND EXTRA_LDFLAGS "--shell-file ${CMAKE_BINARY_DIR}/release_shell.html")

SET(CMAKE_EXECUTABLE_SUFFIX ".html")

# set( GAME_RESOURCES "${TARGET_RESOURCES}" )
# list(APPEND TARGET_RESOURCES ${GAME_RESOURCES})

list(APPEND EXTRA_LDFLAGS "--memory-init-file 1")
# list(APPEND EXTRA_LDFLAGS "--embed-file ${GAME_RESOURCES}@/")

# add_executable( ${PROJECT_NAME}-test-static ${GAME_SOURCES_TEST} ${TARGET_RESOURCES})
add_executable( ${PROJECT_NAME}-test-static ${GAME_SOURCES_TEST})
# set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY XCODE_STARTUP_PROJECT ${PROJECT_NAME}-test-static)

set_target_properties(
  ${PROJECT_NAME}-test-static PROPERTIES
  INCLUDE_DIRECTORIES "${GAME_INCLUDE_DIRS}"
  )

# configure_file("${CMAKEIN_REPO_DIRECTORY}/platform.in/macOS/Info.plist.in" "${CMAKE_BINARY_DIR}/Info.plist")
  # MACOSX_BUNDLE_INFO_PLIST "${CMAKE_BINARY_DIR}/Info.plist"

list(APPEND INSTALL_LIBS ${PROJECT_NAME}-test-static)

target_compile_definitions( ${PROJECT_NAME}-test-static PUBLIC ${TARGET_DEFINITIONS})
target_link_libraries( ${PROJECT_NAME}-test-static ${PROJECT_NAME}-static gtest gmock_main ${EXTRA_LDFLAGS})

add_test(NAME ${PROJECT_NAME}-test-static COMMAND ${PROJECT_NAME}-test-static)

set(CLANG_FORMAT_BIN "/usr/local/bin/clang-format" CACHE STRING "use: `brew install clang-format` to install")
add_custom_target(
    ${PROJECT_NAME}-clang-format-test
    COMMAND ${CLANG_FORMAT_BIN} -style=file -i ${GAME_SOURCES_TEST} -verbose
    SOURCES
    "${CMAKE_SOURCE_DIR}/.clang-format"
    )
