set(SWIG_WRAP_LANG "php")
string(TOUPPER ${SWIG_WRAP_LANG} SWIG_WRAP_LANG_UPPER)

find_package(PHP4 REQUIRED)

if (PHP4_FOUND)
    
    message(STATUS "PHP4_INCLUDE_PATH ${PHP4_INCLUDE_PATH}")

    list(APPEND SWIG_WRAP_INCLUDE ${PHP4_INCLUDE_PATH})

else ()
    message(FATAL_ERROR "PHP need to be installed.")
endif ()
