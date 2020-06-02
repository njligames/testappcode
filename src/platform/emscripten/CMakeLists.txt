set(CMAKE_CXX_FLAGS_DEBUG "--js-opts 0 -g4")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG --llvm-opts 3 --js-opts 1 --closure 1 -s IGNORE_CLOSURE_COMPILER_ERRORS=1")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG --llvm-opts 3 --js-opts 1 --closure 1 -s IGNORE_CLOSURE_COMPILER_ERRORS=1")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 --js-opts 0 -g4 -DNDEBUG")

set(CMAKE_C_FLAGS_DEBUG "--js-opts 0 -g4")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG --llvm-opts 3 --js-opts 1 --closure 1 -s IGNORE_CLOSURE_COMPILER_ERRORS=1")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG --llvm-opts 3 --js-opts 1 --closure 1 -s IGNORE_CLOSURE_COMPILER_ERRORS=1")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 --js-opts 0 -g4 -DNDEBUG")


add_library(SDL2 UNKNOWN IMPORTED)
set_property(TARGET SDL2 APPEND PROPERTY IMPORTED_LOCATION  "${THIRDPARTY_INCLUDE_DIR}/platform/${TARGET_PLATFORM}/libSDL2.a")
list(APPEND GAME_LIBRARIES SDL2)
set_target_properties(SDL2 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${THIRDPARTY_INCLUDE_DIR}/includes/SDL2)

list(APPEND GAME_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
list(APPEND GAME_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/public_include)

set(${PROJECT_NAME}_BUILD_STATIC ON CACHE BOOL "Build the static library")

if(${PROJECT_NAME}_BUILD_STATIC)
    add_library(${PROJECT_NAME}-static STATIC ${SRCS} ${HDRS})
    target_compile_definitions(${PROJECT_NAME}-static PUBLIC ${GAME_DEFINITIONS})

    target_link_libraries( ${PROJECT_NAME}-static ${GAME_LIBRARIES})

    set_property(TARGET ${PROJECT_NAME}-static PROPERTY PUBLIC_HEADER ${GAME_PUBLIC_HEADER})
    set_property(TARGET ${PROJECT_NAME}-static PROPERTY INCLUDE_DIRECTORIES ${GAME_INCLUDE_DIRS})
    set_property(TARGET ${PROJECT_NAME}-static PROPERTY OUTPUT_NAME "${PROJECT_NAME}")
    set_property(TARGET ${PROJECT_NAME}-static PROPERTY XCODE_ATTRIBUTE_PRODUCT_NAME "${PROJECT_NAME}")

    list(APPEND INSTALL_LIBS ${PROJECT_NAME}-static)
endif()

set(CLANG_FORMAT_BIN "/usr/local/bin/clang-format" CACHE STRING "use: `brew install clang-format` to install")
add_custom_target(
    ${PROJECT_NAME}-clang-format
    COMMAND ${CLANG_FORMAT_BIN} -style=file -i ${SRCS} ${HDRS} -verbose
    SOURCES
    "${CMAKE_SOURCE_DIR}/.clang-format"
    )

install(TARGETS ${INSTALL_LIBS} EXPORT gameTargets
    PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
    LIBRARY DESTINATION lib/${PROJECT_NAME}
    ARCHIVE DESTINATION lib/${PROJECT_NAME}
    RUNTIME DESTINATION bin/${PROJECT_NAME}
    FRAMEWORK DESTINATION Frameworks/${PROJECT_NAME}
    )

# enable_testing()
# add_subdirectory(test)

