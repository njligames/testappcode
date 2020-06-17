set(SWIG_WRAP_LANG "lua")
string(TOUPPER ${SWIG_WRAP_LANG} SWIG_WRAP_LANG_UPPER)

if( ANDROID)

    add_library(lua UNKNOWN IMPORTED)
    set_property(TARGET lua APPEND PROPERTY IMPORTED_LOCATION  "${THIRDPARTY_INCLUDE_DIR}/platform/android/${ANDROID_ABI}/liblua.a")
    list(APPEND GAME_LIBRARIES lua)
    set_target_properties(lua PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${THIRDPARTY_INCLUDE_DIR}/includes/lua)

    list(APPEND SWIG_WRAP_INCLUDE ${THIRDPARTY_INCLUDE_DIR}/includes/lua)
    list(APPEND SWIG_WRAP_LIBRARIES lua)
elseif(CMAKE_SYSTEM_NAME MATCHES ".*Linux")
    add_library(lua UNKNOWN IMPORTED)
    set_property(TARGET lua APPEND PROPERTY IMPORTED_LOCATION  "${THIRDPARTY_INCLUDE_DIR}/platform/linux/liblua.so")
    list(APPEND GAME_LIBRARIES lua)
    set_target_properties(lua PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${THIRDPARTY_INCLUDE_DIR}/includes/lua)

    list(APPEND SWIG_WRAP_INCLUDE ${THIRDPARTY_INCLUDE_DIR}/includes/lua)
    list(APPEND SWIG_WRAP_LIBRARIES lua)
elseif(WIN32)
    add_library(lua UNKNOWN IMPORTED)
    set_property(TARGET lua APPEND PROPERTY IMPORTED_LOCATION  "${THIRDPARTY_INCLUDE_DIR}/platform/windows/lua.lib")
    list(APPEND GAME_LIBRARIES lua)
    set_target_properties(lua PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${THIRDPARTY_INCLUDE_DIR}/includes/lua)

    list(APPEND SWIG_WRAP_INCLUDE ${THIRDPARTY_INCLUDE_DIR}/includes/lua)
    list(APPEND SWIG_WRAP_LIBRARIES lua)
elseif(EMSCRIPTEN)
    add_library(lua UNKNOWN IMPORTED)
    set_property(TARGET lua APPEND PROPERTY IMPORTED_LOCATION  "${THIRDPARTY_INCLUDE_DIR}/platform/emscripten/liblua.a")
    list(APPEND GAME_LIBRARIES lua)
    set_target_properties(lua PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${THIRDPARTY_INCLUDE_DIR}/includes/lua)

    list(APPEND SWIG_WRAP_INCLUDE ${THIRDPARTY_INCLUDE_DIR}/includes/lua)
    list(APPEND SWIG_WRAP_LIBRARIES lua)
else()
    find_package(Lua REQUIRED)
    if (LUA_FOUND)
        message(STATUS "LUA_FOUND ${LUA_FOUND}")
        message(STATUS "LUA_LIBRARIES ${LUA_LIBRARIES}")
        message(STATUS "LUA_INCLUDE_DIR ${LUA_INCLUDE_DIR}")
        message(STATUS "LUA_VERSION_STRING ${LUA_VERSION_STRING}")
        message(STATUS "LUA_VERSION_MAJOR ${LUA_VERSION_MAJOR}")
        message(STATUS "LUA_VERSION_MINOR ${LUA_VERSION_MINOR}")
        message(STATUS "LUA_VERSION_PATCH ${LUA_VERSION_PATCH}")

        list(APPEND SWIG_WRAP_INCLUDE ${LUA_INCLUDE_DIR})
        list(APPEND SWIG_WRAP_LIBRARIES ${LUA_LIBRARIES})

    else (LUA_FOUND)
        message(FATAL_ERROR "Lua need to be installed.")
    endif (LUA_FOUND)
endif()
