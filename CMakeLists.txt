cmake_minimum_required(VERSION 2.8.8)

project(game-test)

set(THIRDPARTY_DIR "${CMAKE_SOURCE_DIR}/thirdparty")
set(GAME_INCLUDE_DIRS ${CMAKE_SOURCE_DIR})

file(GLOB_RECURSE SRCS ${CMAKE_SOURCE_DIR}/*.c*)
file(GLOB_RECURSE HDRS ${CMAKE_SOURCE_DIR}/*.h)

list(APPEND GAME_INCLUDE_DIRS ${THIRDPARTY_DIR}/includes/glm)
list(APPEND GAME_INCLUDE_DIRS ${THIRDPARTY_DIR}/includes/NJLICEngine)

#if( DEFINED ${TARGET_PLATFORM})
#
#    add_subdirectory(${TARGET_PLATFORM})
#else()
#    add_library(game STATIC ${SRCS} ${HDRS})
#
#    set_target_properties(
#        game PROPERTIES
#        OUTPUT_NAME "game"
#        POSITION_INDEPENDENT_CODE ON
#        INCLUDE_DIRECTORIES "${GAME_INCLUDE_DIRS}"
#     )
#endif()

add_subdirectory(${TARGET_PLATFORM})

