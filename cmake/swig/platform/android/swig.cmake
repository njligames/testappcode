if(DEFINED SWIG_WRAP_LANG)

    target_include_directories(${PROJECT_NAME}-${SWIG_WRAP_LANG}-static
        PRIVATE $<BUILD_INTERFACE:${SWIG_WRAP_INCLUDE}>
        PRIVATE $<BUILD_INTERFACE:${SWIG_INCLUDE_DIR}>
        PRIVATE $<BUILD_INTERFACE:${GAME_INCLUDE_DIRS}>
        )
    target_link_libraries(${PROJECT_NAME}-${SWIG_WRAP_LANG}-static
        ${SWIG_WRAP_LIBRARIES}
        )

    target_include_directories(${PROJECT_NAME}-${SWIG_WRAP_LANG}
        PRIVATE $<BUILD_INTERFACE:${SWIG_WRAP_INCLUDE}>
        PRIVATE $<BUILD_INTERFACE:${SWIG_INCLUDE_DIR}>
        PRIVATE $<BUILD_INTERFACE:${GAME_INCLUDE_DIRS}>
        )
    target_link_libraries(${PROJECT_NAME}-${SWIG_WRAP_LANG}
        ${SWIG_WRAP_LIBRARIES}
        )

    target_include_directories(${PROJECT_NAME}-${SWIG_WRAP_LANG}-module
        PRIVATE $<BUILD_INTERFACE:${SWIG_WRAP_INCLUDE}>
        PRIVATE $<BUILD_INTERFACE:${SWIG_INCLUDE_DIR}>
        PRIVATE $<BUILD_INTERFACE:${GAME_INCLUDE_DIRS}>
        )
    target_link_libraries(${PROJECT_NAME}-${SWIG_WRAP_LANG}-module
        ${SWIG_WRAP_LIBRARIES}
        )
endif()


