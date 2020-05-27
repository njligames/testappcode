if(DEFINED SWIG_WRAP_LANG)
    set(INCLUDE_DIRS ${SWIG_WRAP_INCLUDE} ${SWIG_INCLUDE_DIR} ${GAME_INCLUDE_DIRS} )
    include_directories(${INCLUDE_DIRS})
    message(STATUS "${INCLUDE_DIRS}")

    target_include_directories(${PROJECT_NAME}-${SWIG_WRAP_LANG}-static
        PRIVATE $<BUILD_INTERFACE:${INCLUDE_DIRS}>
        )
    target_link_libraries(${PROJECT_NAME}-${SWIG_WRAP_LANG}-static
        ${SWIG_WRAP_LIBRARIES}
        )

    # target_include_directories(${PROJECT_NAME}-${SWIG_WRAP_LANG}
    #     PUBLIC $<BUILD_INTERFACE:${INCLUDE_DIRS}>
    #     )
    # target_link_libraries(${PROJECT_NAME}-${SWIG_WRAP_LANG}
    #     ${SWIG_WRAP_LIBRARIES}
    #     )

    # target_include_directories(${PROJECT_NAME}-${SWIG_WRAP_LANG}-module
    #     PUBLIC $<BUILD_INTERFACE:${INCLUDE_DIRS}>
    #     )
    # target_link_libraries(${PROJECT_NAME}-${SWIG_WRAP_LANG}-module
    #     ${SWIG_WRAP_LIBRARIES}
    #     )
endif()


