
#if defined(__GL_ES2__)
    #error Unsupported choice setting
#elif defined(__GL_ES3__)
    #error Unsupported choice setting
#elif defined(__GL_2__)
    #define glGenVertexArrays_NJLIC glGenVertexArrays
    #define glBindVertexArray_NJLIC glBindVertexArray
    #define glDeleteVertexArrays_NJLIC glDeleteVertexArrays

    #define GL_GLEXT_PROTOTYPES
    #include <GL/gl.h>
    #include <GL/glext.h>
#elif defined(__GL_3__)
    #include <OpenGL/gl3.h>
    #include <OpenGL/gl3ext.h>
#else
    #error Unsupported choice setting
#endif
