list(APPEND GAME_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
list(APPEND GAME_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/public_include)

list(APPEND GAME_DEFINITIONS "GL_SILENCE_DEPRECATION=1")

add_library(game STATIC ${SRCS} ${HDRS})
target_compile_definitions(game PUBLIC ${GAME_DEFINITIONS})

list(APPEND GAME_LIBRARIES "-framework CoreVideo")
list(APPEND GAME_LIBRARIES "-framework Cocoa")
list(APPEND GAME_LIBRARIES "-framework IOKit")
list(APPEND GAME_LIBRARIES "-framework ForceFeedback")
list(APPEND GAME_LIBRARIES "-framework Carbon")
list(APPEND GAME_LIBRARIES "-framework CoreAudio")
list(APPEND GAME_LIBRARIES "-framework AudioToolbox")
list(APPEND GAME_LIBRARIES "-framework Metal")
list(APPEND GAME_LIBRARIES "-framework OpenGL")

target_link_libraries( game ${GAME_LIBRARIES})

set_property(TARGET game PROPERTY PUBLIC_HEADER ${GAME_PUBLIC_HEADER})
set_property(TARGET game PROPERTY INCLUDE_DIRECTORIES ${GAME_INCLUDE_DIRS})
set_property(TARGET game PROPERTY OUTPUT_NAME "game")
set_property(TARGET game PROPERTY XCODE_ATTRIBUTE_PRODUCT_NAME "game")

install(TARGETS game EXPORT gameTargets
    PUBLIC_HEADER DESTINATION game/include
    LIBRARY DESTINATION game/lib
    ARCHIVE DESTINATION game/archive
    RUNTIME DESTINATION game/bin)


set(CLANG_FORMAT_BIN "/usr/local/bin/clang-format" CACHE STRING "use: `brew install clang-format` to install")
add_custom_target(
    clang-format
    COMMAND ${CLANG_FORMAT_BIN} -style=file -i ${SRCS} ${HDRS} -verbose
    SOURCES
    "${CMAKE_SOURCE_DIR}/.clang-format"
    )

enable_testing()
add_subdirectory(test)
