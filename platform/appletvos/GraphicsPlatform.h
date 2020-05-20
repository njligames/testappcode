
// #if defined(__GL_ES2__)
#define glGenVertexArrays_NJLIC glGenVertexArraysOES
#define glBindVertexArray_NJLIC glBindVertexArrayOES
#define glDeleteVertexArrays_NJLIC glDeleteVertexArraysOES

#include <OpenGLES/ES2/glext.h>
#include <OpenGLES/ES2/gl.h>

// #elif defined(__GL_ES3__)
// #include <OpenGLES/ES3/glext.h>
// #include <OpenGLES/ES3/gl.h>
// #define NANOVG_GLES3_IMPLEMENTATION
// #elif defined(__GL_2__)
// #error Unsupported choice setting
// #elif defined(__GL_3__)
// #error Unsupported choice setting
// #else
// #error Unsupported choice setting
// #endif

