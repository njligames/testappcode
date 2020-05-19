list(APPEND TARGET_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

add_library(game SHARED ${SRCS} ${HDRS})
target_compile_definitions(game PUBLIC ${TARGET_DEFINITIONS})
target_link_libraries( game ${TARGET_LIBRARIES})
set_target_properties(game PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${TARGET_INCLUDE_DIRS}")
include_directories(${TARGET_INCLUDE_DIRS})
