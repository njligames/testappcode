set(SWIG_WRAP_LANG "lua")
string(TOUPPER ${SWIG_WRAP_LANG} SWIG_WRAP_LANG_UPPER)

find_package(Lua REQUIRED)

if (LUA_FOUND)
    message(STATUS "LUA_FOUND ${LUA_FOUND}")
    message(STATUS "LUA_LIBRARIES ${LUA_LIBRARIES}")
    message(STATUS "LUA_INCLUDE_DIR ${LUA_INCLUDE_DIR}")
    message(STATUS "LUA_VERSION_STRING ${LUA_VERSION_STRING}")
    message(STATUS "LUA_VERSION_MAJOR ${LUA_VERSION_MAJOR}")
    message(STATUS "LUA_VERSION_MINOR ${LUA_VERSION_MINOR}")
    message(STATUS "LUA_VERSION_PATCH ${LUA_VERSION_PATCH}")

    list(APPEND EXTRA_LIBS ${LUA_LIBRARIES})

    list(APPEND SWIG_WRAP_INCLUDE ${LUA_INCLUDE_DIR})
    list(APPEND SWIG_WRAP_LIBRARIES ${LUA_LIBRARIES})

else (LUA_FOUND)
    message(FATAL_ERROR "Lua need to be installed.")
endif (LUA_FOUND)