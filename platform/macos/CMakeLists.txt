list(APPEND GAME_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

list(APPEND GAME_DEFINITIONS "GL_SILENCE_DEPRECATION=1")

add_library(game STATIC ${SRCS} ${HDRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(game PUBLIC ${GAME_DEFINITIONS})

target_link_libraries( game ${GAME_LIBRARIES})

set_property(TARGET game PROPERTY PUBLIC_HEADER ${GAME_PUBLIC_HEADER})
set_property(TARGET game PROPERTY INCLUDE_DIRECTORIES ${GAME_INCLUDE_DIRS})
set_property(TARGET game PROPERTY OUTPUT_NAME "game")
set_property(TARGET game PROPERTY XCODE_ATTRIBUTE_PRODUCT_NAME "game")

install(TARGETS game EXPORT gameTargets
    PUBLIC_HEADER DESTINATION include
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION archive
    RUNTIME DESTINATION bin)

# set(INCLUDE_FILES "")
# foreach(include_dir ${GAME_INCLUDE_DIRS})
#     set(_INCLUDE_FILES "")
#     file(GLOB _INCLUDE_FILES ${include_dir}/*.h*)
#     list(APPEND INCLUDE_FILES ${INCLUDE_FILES})
# endforeach()

# install(FILES ${INCLUDE_FILES} DESTINATION include/game)
