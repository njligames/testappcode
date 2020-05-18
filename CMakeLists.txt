cmake_minimum_required(VERSION 2.8.8)

project(game-test)

set(THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")
set(TARGET_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})


file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.c*)
file(GLOB_RECURSE HDRS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

# list(APPEND TARGET_INCLUDE_DIRS ${THIRDPARTY_DIR}/includes/glm)
# list(APPEND TARGET_INCLUDE_DIRS ${THIRDPARTY_DIR}/includes/NJLICEngine)

# if(${TARGET_PLATFORM})
# list(APPEND TARGET_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_PLATFORM})
    include_directories(${TARGET_INCLUDE_DIRS})
    # MESSAGE(FATAL_ERROR "TARGET_INCLUDE_DIRS ${TARGET_INCLUDE_DIRS}")
    add_subdirectory(${TARGET_PLATFORM})
    # else()
    #     add_library(game STATIC ${SRCS} ${HDRS})
    #
    #     set_target_properties(
    #         game PROPERTIES
    #         OUTPUT_NAME "game"
    #         XCODE_ATTRIBUTE_PRODUCT_NAME game
    #         POSITION_INDEPENDENT_CODE ON
    #         XCODE_ATTRIBUTE_ENABLE_BITCODE NO
    #         INCLUDE_DIRECTORIES "${TARGET_INCLUDE_DIRS}"
    #      )
    # endif()


# set_property(TARGET game PROPERTY INCLUDE_DIRECTORIES ${TARGET_INCLUDE_DIRS})
