set(SWIG_WRAP_LANG "javascript")
string(TOUPPER ${SWIG_WRAP_LANG} SWIG_WRAP_LANG_UPPER)

list(APPEND CMAKE_SWIG_FLAGS "-node")

list(APPEND SWIG_WRAP_INCLUDE "/usr/local/include/node/")

# HOWTO:
# https://stackoverflow.com/questions/9629677/how-can-i-use-a-c-library-from-node-js
# need: node-gyp

