
include(${CMAKE_SOURCE_DIR}/cmake/macros.cmake)

macro(OCTAVE_SWIG)
  set(SWIG_WRAP_LANG "octave")
  string(TOUPPER ${SWIG_WRAP_LANG} SWIG_WRAP_LANG_UPPER)

  find_package(Octave REQUIRED)
  if(OCTAVE_FOUND)
    message(STATUS "OCTAVE_FOUND ${OCTAVE_FOUND}")
    message(STATUS "OCTAVE_LIBRARIES ${OCTAVE_LIBRARIES}")
    message(STATUS "OCTAVE_INCLUDE_DIR ${OCTAVE_INCLUDE_DIR}")

    list(APPEND EXTRA_LIBS ${OCTAVE_LIBRARIES})

    set(SWIG_PACKAGE_INCLUDE_DIR ${OCTAVE_INCLUDE_DIR})

  else (OCTAVE_FOUND)
    message(FATAL_ERROR "Octave needs to be installed.")
  endif (OCTAVE_FOUND)

  set(CMAKE_SWIG_OUTDIR "${CMAKE_BINARY_DIR}/swig/${SWIG_WRAP_LANG}/${CMAKE_SYSTEM_NAME}")
  set(SWIG_OUTFILE_DIR "${CMAKE_BINARY_DIR}/swig/${SWIG_WRAP_LANG}/${CMAKE_SYSTEM_NAME}")

  if(NOT CMAKE_PROJECT_NAME)
    MESSAGE(FATAL_ERROR "CMAKE_PROJECT_NAME variable is empty.")
  endif()

  set(PROJECT_SWIG_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/swig.in")

  if(NOT PROJECT_SWIG_INCLUDE_DIR)
    MESSAGE(FATAL_ERROR "PROJECT_SWIG_INCLUDE_DIR variable is empty.")
  endif()

  if(NOT INCLUDE_DIR)
    MESSAGE(FATAL_ERROR "INCLUDE_DIR variable is empty.")
  endif()

  if(NOT EXISTS "${PROJECT_SWIG_INCLUDE_DIR}")
    MESSAGE(FATAL_ERROR "Path doesn't exist: ${PROJECT_SWIG_INCLUDE_DIR}")
  endif()

  if(NOT EXISTS "${INCLUDE_DIR}")
    MESSAGE(FATAL_ERROR "Path doesn't exist: ${INCLUDE_DIR}")
  endif()

  if(NOT EXISTS "${PROJECT_SWIG_INCLUDE_DIR}/evr-internals-api.i")
    MESSAGE(FATAL_ERROR "Path doesn't exist: ${PROJECT_SWIG_INCLUDE_DIR}/evr-internals-api.i")
  endif()

  file(GLOB_RECURSE SWIG_SOURCE_FILES
    ${PROJECT_SWIG_INCLUDE_DIR}/*.swg
    )

  list(APPEND SWIG_SOURCE_FILES
    "${PROJECT_SWIG_INCLUDE_DIR}/evr-internals-api.i"
    ${PUBLIC_INCLUDE_FILES}
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
    # "${LUA_INCLUDE_DIR}/lua.h"
    )

  SUBDIRLIST(SUBDIRS "${PROJECT_SWIG_INCLUDE_DIR}" PROJECT_SWIG_INCLUDE_DIRS)
  list(APPEND PROJECT_SWIG_INCLUDE_DIRS ${PUBLIC_INCLUDE_DIR})

  set(CMAKE_SWIG_FLAGS "-includeall")
  # list(APPEND CMAKE_SWIG_FLAGS "-fcompact")
  list(APPEND CMAKE_SWIG_FLAGS "-fvirtual")
  list(APPEND CMAKE_SWIG_FLAGS "-v")
  list(APPEND CMAKE_SWIG_FLAGS "-w201")
  list(APPEND CMAKE_SWIG_FLAGS "-w312")
  list(APPEND CMAKE_SWIG_FLAGS "-ignoremissing")
  # list(APPEND CMAKE_SWIG_FLAGS "-cpperraswarn")
  list(APPEND CMAKE_SWIG_FLAGS "-DSWIG_TYPE_TABLE=myprojectname")

  foreach(_SWIG_SOURCE_FILE ${SWIG_SOURCE_FILES})
    set_property(SOURCE ${_SWIG_SOURCE_FILE} PROPERTY CPLUSPLUS ON)
    set_property(SOURCE ${_SWIG_SOURCE_FILE} PROPERTY SWIG_INCLUDE_DIRECTORIES "${PROJECT_SWIG_INCLUDE_DIRS}")
  endforeach()

  include_directories(
    ${PROJECT_SWIG_INCLUDE_DIRS}
    ${PUBLIC_INCLUDE_DIR}
    ${INCLUDE_DIR}
    # ${LUA_INCLUDE_DIR}
    # ${INCLUDE_DIR}
    # ${SOURCE_FILES}
    # ${INCLUDE_FILES}
    )

  get_property(_INCLUDE_DIRECTORIES DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
  list(REMOVE_ITEM _INCLUDE_DIRECTORIES "/usr/include")
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES ${_INCLUDE_DIRECTORIES} )

  set(SWIG_TARGET_NAME ${CMAKE_PROJECT_NAME}-${SWIG_WRAP_LANG}-swig-static)

  swig_add_library(
    ${SWIG_TARGET_NAME}
    TYPE STATIC
    LANGUAGE ${SWIG_WRAP_LANG}
    SOURCES "${SWIG_SOURCE_FILES}"
    )

  if(APPLE)
    if(IOS OR TVOS)
      SET_TARGET_PROPERTIES (
        ${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME} PROPERTIES
        XCODE_PRODUCT_TYPE "com.apple.product-type.library.static"
        )
    endif(IOS OR TVOS)
  endif()
  target_link_libraries(${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME} ${CMAKE_PROJECT_NAME}-static)
  foreach(EXTRA_LIB ${EXTRA_LIBS})
    target_link_libraries( ${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME} ${EXTRA_LIB})
  endforeach()

  list(APPEND EXTRA_LIBS ${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME})
  list(APPEND INTERFACE_FILES ${SWIG_SOURCE_FILES})

  set(_INSTALL_LIBS "${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME}" ${_INSTALL_LIBS})
  target_include_directories(${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME}
    PRIVATE $<BUILD_INTERFACE:${THIRDPARTY_INCLUDE_DIR}>
    PRIVATE $<BUILD_INTERFACE:${INCLUDE_DIR}>
    PRIVATE $<BUILD_INTERFACE:${SWIG_PACKAGE_INCLUDE_DIR}>
    PUBLIC $<BUILD_INTERFACE:${PUBLIC_INCLUDE_DIR}>
    )

  if(NOT LINUX AND NOT EMSCRIPTEN AND NOT IOS AND NOT TVOS AND NOT ANDROID)

    set(SWIG_TARGET_NAME ${CMAKE_PROJECT_NAME}-${SWIG_WRAP_LANG}-swig)

    swig_add_library(
      ${SWIG_TARGET_NAME}
      TYPE SHARED
      LANGUAGE ${SWIG_WRAP_LANG}
      SOURCES "${SWIG_SOURCE_FILES}"
      )

    if(APPLE)
      # set_target_properties(${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME} PROPERTIES MACOSX_RPATH 1)
      set_target_properties(${SWIG_MODULE_${EVR_NAME}_REAL_NAME} PROPERTIES SUFFIX ".so" INSTALL_RPATH "@loader_path")
      set_property(TARGET ${SWIG_MODULE_${EVR_NAME}_REAL_NAME} APPEND PROPERTY LINK_FLAGS "-flat_namespace -undefined suppress")
    elseif(UNIX AND NOT ANDROID)
      set_target_properties(${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME} PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        VERSION ${LT_VERSION}
        SOVERSION ${LT_REVISION}
        OUTPUT_NAME "${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME}-${LT_RELEASE}")
    else()
      set_target_properties(${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN")
    endif()
    if(MSVC AND NOT LIBC)
      list(APPEND EXTRA_LIBS "msvcrt.lib")
      # Don't try to link with the default set of libraries.
      set_target_properties(${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME} PROPERTIES LINK_FLAGS_RELEASE "/NODEFAULTLIB:LIBCMT")
      set_target_properties(${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME} PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:LIBCMT")
      target_link_libraries(${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME} ${EXTRA_LDFLAGS} debug "msvcrtd.lib")
    endif()
    target_link_libraries(${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME} ${CMAKE_PROJECT_NAME}-static)
    foreach(EXTRA_LIB ${EXTRA_LIBS})
      target_link_libraries( ${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME} ${EXTRA_LIB})
    endforeach()

    set(_INSTALL_LIBS "${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME}" ${_INSTALL_LIBS})
    target_include_directories(${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME}
      PRIVATE $<BUILD_INTERFACE:${THIRDPARTY_INCLUDE_DIR}>
      PRIVATE $<BUILD_INTERFACE:${INCLUDE_DIR}>
      PRIVATE $<BUILD_INTERFACE:${SWIG_PACKAGE_INCLUDE_DIR}>
      PUBLIC $<BUILD_INTERFACE:${PUBLIC_INCLUDE_DIR}>
      )
  endif()

  ##################################################################################

  if(NOT EMSCRIPTEN AND NOT IOS AND NOT TVOS AND NOT ANDROID AND NOT LINUX)
    #So... non-mobile (WINDOWS, LINUX, MAC

    set(SWIG_TARGET_NAME evr)

    swig_add_library(
      ${SWIG_TARGET_NAME}
      TYPE MODULE
      LANGUAGE ${SWIG_WRAP_LANG}
      SOURCES "${SWIG_SOURCE_FILES}"
      )

    if(APPLE)
      # set_target_properties(${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME} PROPERTIES MACOSX_RPATH 1)
      set_target_properties(${SWIG_MODULE_${EVR_NAME}_REAL_NAME} PROPERTIES SUFFIX ".so" INSTALL_RPATH "@loader_path")
      set_property(TARGET ${SWIG_MODULE_${EVR_NAME}_REAL_NAME} APPEND PROPERTY LINK_FLAGS "-flat_namespace -undefined suppress")
    elseif(UNIX AND NOT ANDROID)
      set_target_properties(${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME} PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        VERSION ${LT_VERSION}
        SOVERSION ${LT_REVISION}
        OUTPUT_NAME "${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME}-${LT_RELEASE}")
    else()
      set_target_properties(${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN")
    endif()
    if(MSVC AND NOT LIBC)
      list(APPEND EXTRA_LIBS "msvcrt.lib")
      # Don't try to link with the default set of libraries.
      set_target_properties(${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME} PROPERTIES LINK_FLAGS_RELEASE "/NODEFAULTLIB:LIBCMT")
      set_target_properties(${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME} PROPERTIES LINK_FLAGS_DEBUG "/NODEFAULTLIB:LIBCMT")
      target_link_libraries(${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME} ${EXTRA_LDFLAGS} debug "msvcrtd.lib")
    endif()
    target_link_libraries(${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME} ${CMAKE_PROJECT_NAME}-static )
    foreach(EXTRA_LIB ${EXTRA_LIBS})
      target_link_libraries( ${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME} ${EXTRA_LIB})
    endforeach()
    SET_TARGET_PROPERTIES(${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME} PROPERTIES PREFIX "")

    set(_INSTALL_LIBS "${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME}" ${_INSTALL_LIBS})
    target_include_directories(${SWIG_MODULE_${SWIG_TARGET_NAME}_REAL_NAME}
      PRIVATE $<BUILD_INTERFACE:${THIRDPARTY_INCLUDE_DIR}>
      PRIVATE $<BUILD_INTERFACE:${INCLUDE_DIR}>
      PRIVATE $<BUILD_INTERFACE:${SWIG_PACKAGE_INCLUDE_DIR}>
      PUBLIC $<BUILD_INTERFACE:${PUBLIC_INCLUDE_DIR}>
      )
  endif()

  file(GLOB_RECURSE SWIG_GENERATED_FILES
    "${SWIG_OUTFILE_DIR}/*.*"
    )

  install(DIRECTORY ${CMAKE_SWIG_OUTDIR} DESTINATION ${CMAKE_INSTALL_PREFIX}/test/${SWIG_WRAP_LANG} FILES_MATCHING PATTERN "*.so")
  install(DIRECTORY ${CMAKE_SWIG_OUTDIR} DESTINATION ${CMAKE_INSTALL_PREFIX}/test/${SWIG_WRAP_LANG} FILES_MATCHING PATTERN "*.py")

  set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${SWIG_GENERATED_FILES})
  set_source_files_properties( ${SWIG_GENERATED_FILES} PROPERTIES GENERATED TRUE )

endmacro()
