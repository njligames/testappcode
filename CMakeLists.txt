cmake_minimum_required(VERSION 2.8.8)

project(game-test)

include(cmake/internal_utils.cmake)
set(THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")
set(TARGET_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})


file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.c*)
file(GLOB_RECURSE HDRS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

list(APPEND TARGET_INCLUDE_DIRS ${THIRDPARTY_DIR}/includes/glm)

add_library(game STATIC ${SRCS} ${HDRS})

set_property(TARGET game PROPERTY INCLUDE_DIRECTORIES ${TARGET_INCLUDE_DIRS})
