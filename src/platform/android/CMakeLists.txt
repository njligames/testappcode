add_library(SDL2 UNKNOWN IMPORTED)
set_property(TARGET SDL2 APPEND PROPERTY IMPORTED_LOCATION  "${THIRDPARTY_INCLUDE_DIR}/platform/android/${ANDROID_ABI}/libSDL2.so")
list(APPEND GAME_LIBRARIES SDL2)
set_target_properties(SDL2 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${THIRDPARTY_INCLUDE_DIR}/includes/SDL2")

find_library(log-lib log)
list(APPEND GAME_LIBRARIES "${log-lib}")

find_library(dl-lib dl)
list(APPEND GAME_LIBRARIES "${dl-lib}")

# IF( GL_ES2 )
   find_library(GLESv2-lib GLESv2)
   list(APPEND GAME_LIBRARIES "${GLESv2-lib}")
# ELSEIF( GL_ES3 )
#   FIND_LIBRARY(GLESv3-lib GLESv3)
#   list(APPEND GAME_LIBRARIES "${GLESv3-lib}")
# ELSE()
#   MESSAGE(FATAL_ERROR "opengl isn't defined for android")
# ENDIF()

find_library(m-lib m)
list(APPEND GAME_LIBRARIES "${m-lib}")

find_library(android-lib android)
list(APPEND GAME_LIBRARIES "${android-lib}")

list(APPEND GAME_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
list(APPEND GAME_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/public_include)

set(${PROJECT_NAME}_BUILD_SHARED ON CACHE BOOL "Build the shared library")
set(${PROJECT_NAME}_BUILD_STATIC ON CACHE BOOL "Build the shared library")

if(${PROJECT_NAME}_BUILD_STATIC)
    add_library(${PROJECT_NAME}-static STATIC ${SRCS} ${HDRS})

    target_link_libraries( ${PROJECT_NAME}-static ${GAME_LIBRARIES})

    set_property(TARGET ${PROJECT_NAME}-static PROPERTY PUBLIC_HEADER ${GAME_PUBLIC_HEADER})
    set_property(TARGET ${PROJECT_NAME}-static PROPERTY INCLUDE_DIRECTORIES ${GAME_INCLUDE_DIRS})
    set_property(TARGET ${PROJECT_NAME}-static PROPERTY OUTPUT_NAME "${PROJECT_NAME}")

    set_target_properties(${PROJECT_NAME}-static PROPERTIES POSITION_INDEPENDENT_CODE ON)

    list(APPEND INSTALL_LIBS ${PROJECT_NAME}-static)
endif()

if(${PROJECT_NAME}_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SRCS} ${HDRS})

    target_link_libraries( ${PROJECT_NAME} ${GAME_LIBRARIES})

    set_property(TARGET ${PROJECT_NAME} PROPERTY PUBLIC_HEADER ${GAME_PUBLIC_HEADER})
    set_property(TARGET ${PROJECT_NAME} PROPERTY INCLUDE_DIRECTORIES ${GAME_INCLUDE_DIRS})
    set_property(TARGET ${PROJECT_NAME} PROPERTY OUTPUT_NAME "${PROJECT_NAME}")
    # set_property(TARGET ${PROJECT_NAME} PROPERTY XCODE_ATTRIBUTE_PRODUCT_NAME "${PROJECT_NAME}")
    set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

    list(APPEND INSTALL_LIBS ${PROJECT_NAME})
endif()

set(CLANG_FORMAT_BIN "/usr/local/bin/clang-format" CACHE STRING "use: `brew install clang-format` to install")
add_custom_target(
    ${PROJECT_NAME}-clang-format
    COMMAND ${CLANG_FORMAT_BIN} -style=file -i ${SRCS} ${HDRS} -verbose
    SOURCES
    "${CMAKE_SOURCE_DIR}/.clang-format"
    )

# install(TARGETS ${INSTALL_LIBS} EXPORT gameTargets
#     PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
#     LIBRARY DESTINATION lib/${PROJECT_NAME}
#     ARCHIVE DESTINATION lib/${PROJECT_NAME}
#     RUNTIME DESTINATION bin/${PROJECT_NAME}
#     )

if(${PROJECT_NAME}_TEST)
    enable_testing()
    add_subdirectory(test)
endif()
