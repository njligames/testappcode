
#if defined(__GL_ES2__)
    #define glGenVertexArrays_NJLIC glGenVertexArrays
    #define glBindVertexArray_NJLIC glBindVertexArray
    #define glDeleteVertexArrays_NJLIC glDeleteVertexArrays

    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
    #include <GLES2/gl2platform.h>
#elif defined(__GL_ES3__)
    #include <GLES3/gl3.h>
    #include <GLES3/gl3ext.h>
    #include <GLES3/gl3platform.h>
#elif defined(__GL_2__)
    #error Unsupported choice setting
#elif defined(__GL_3__)
    #error Unsupported choice setting
#else
    #error Unsupported choice setting
#endif
