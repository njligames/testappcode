add_library(game SHARED ${SRCS} ${HDRS})

set_target_properties(
    game PROPERTIES
    OUTPUT_NAME "game"
    POSITION_INDEPENDENT_CODE ON
    INCLUDE_DIRECTORIES "${TARGET_INCLUDE_DIRS}"
 )
