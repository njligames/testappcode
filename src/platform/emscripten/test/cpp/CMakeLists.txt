add_library(SDL_test UNKNOWN IMPORTED)
set_property(TARGET SDL_test APPEND PROPERTY IMPORTED_LOCATION  "${THIRDPARTY_INCLUDE_DIR}/platform/${TARGET_PLATFORM}/libSDL2_test.a")
list(APPEND TARGET_LIBRARIES SDL_test)

list(APPEND GAME_INCLUDE_DIRS ${GTEST_INCLUDE_DIRS})
list(APPEND GAME_INCLUDE_DIRS ${GMOCK_INCLUDE_DIRS})
list(APPEND GAME_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.c*)
file(GLOB_RECURSE HDRS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

list(APPEND GAME_SOURCES_TEST ${SRCS} ${HDRS})

add_executable( ${PROJECT_NAME}-test-static ${GAME_SOURCES_TEST} ${TARGET_RESOURCES})
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY XCODE_STARTUP_PROJECT ${PROJECT_NAME}-test-static)

# configure_file("${CMAKEIN_REPO_DIRECTORY}/platform.in/macOS/Info.plist.in" "${CMAKE_BINARY_DIR}/Info.plist")
  # MACOSX_BUNDLE_INFO_PLIST "${CMAKE_BINARY_DIR}/Info.plist"

list(APPEND INSTALL_LIBS ${PROJECT_NAME}-test-static)

target_compile_definitions( ${PROJECT_NAME}-test-static PUBLIC ${TARGET_DEFINITIONS})
target_link_libraries( ${PROJECT_NAME}-test-static ${PROJECT_NAME} gtest gmock_main ${TARGET_LIBRARIES})

add_test(NAME ${PROJECT_NAME}-test-static COMMAND ${PROJECT_NAME}-test-static)

set(CLANG_FORMAT_BIN "/usr/local/bin/clang-format" CACHE STRING "use: `brew install clang-format` to install")
add_custom_target(
    ${PROJECT_NAME}-clang-format-test
    COMMAND ${CLANG_FORMAT_BIN} -style=file -i ${GAME_SOURCES_TEST} -verbose
    SOURCES
    "${CMAKE_SOURCE_DIR}/.clang-format"
    )