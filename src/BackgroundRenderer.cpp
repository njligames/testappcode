//
//  BackgroundRenderer.cpp
//  SDLSkeleton
//
//  Created by James Folk on 2/25/20.
//

#include "BackgroundRenderer.h"
#include "MaterialProperty.h"
#include "SDL.h"
#include "Util.h"
#include <string>

namespace NJLIC {

    //    static GLuint sWidth(1920);
    //    static GLuint sHeight(1080);

    enum { ATTRIB_VERTEX, ATTRIB_COLOR, ATTRIB_TEXTUREPOSITON, NUM_ATTRIBUTES };

    static void setupVertexBuffer(GLuint &vao, GLuint &vertexBuffer,
                                  GLuint &indexBuffer) {

        static GLfloat _vertices[] = {
            1.0,  -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, // right-top
            1.0,  1.0,  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, // right-bottom
            -1.0, 1.0,  1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, // left-bottom
            -1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, // left-top
        };

        static GLubyte _indices[] = {0, 1, 2, 2, 3, 0};

        glGenVertexArraysAPPLE(1, &vao);

        Util::glErrorCheck();
        glBindVertexArrayAPPLE(vao);
        Util::glErrorCheck();

        glGenBuffers(GLsizei(1), &vertexBuffer);

        Util::glErrorCheck();
        glBindBuffer(GLenum(GL_ARRAY_BUFFER), vertexBuffer);
        Util::glErrorCheck();

        size_t v_count = 4; //_vertices.size() / 7;
        size_t v_size = sizeof(GLfloat) * 9;
        const void *vertices = (const void *)_vertices;
        glBufferData(GLenum(GL_ARRAY_BUFFER), v_count * v_size, vertices,
                     GLenum(GL_STATIC_DRAW));
        Util::glErrorCheck();

        glGenBuffers(GLsizei(1), &indexBuffer);

        Util::glErrorCheck();
        glBindBuffer(GLenum(GL_ELEMENT_ARRAY_BUFFER), indexBuffer);
        Util::glErrorCheck();
        size_t i_count = 6; //_indices.size();
        size_t i_size = sizeof(GLubyte);
        const void *indices = (const void *)_indices;
        glBufferData(GLenum(GL_ELEMENT_ARRAY_BUFFER), i_count * i_size, indices,
                     GLenum(GL_STATIC_DRAW));
        Util::glErrorCheck();

        GLsizei s1 = GLsizei(sizeof(GLfloat) * 9);
        int p1 = 0;
        glEnableVertexAttribArray(ATTRIB_VERTEX);
        Util::glErrorCheck();
        glVertexAttribPointer(ATTRIB_VERTEX, 3, GLenum(GL_FLOAT),
                              GLboolean(GL_FALSE), s1, (const GLvoid *)p1);
        Util::glErrorCheck();

        GLsizei s2 = GLsizei(sizeof(GLfloat) * 9);
        int p2 = (3 * sizeof(GLfloat));
        glEnableVertexAttribArray(ATTRIB_COLOR);
        Util::glErrorCheck();
        glVertexAttribPointer(ATTRIB_COLOR, 4, GLenum(GL_FLOAT),
                              GLboolean(GL_FALSE), s2, (const GLvoid *)p2);
        Util::glErrorCheck();

        GLsizei s3 = GLsizei(sizeof(GLfloat) * 9);
        int p3 = ((3 + 4) * sizeof(GLfloat));
        glEnableVertexAttribArray(ATTRIB_TEXTUREPOSITON);
        Util::glErrorCheck();
        glVertexAttribPointer(ATTRIB_TEXTUREPOSITON, 2, GLenum(GL_FLOAT),
                              GLboolean(GL_FALSE), s3, (const GLvoid *)p3);
        Util::glErrorCheck();

        glBindVertexArrayAPPLE(0);
        glBindBuffer(GLenum(GL_ARRAY_BUFFER), 0);
        glBindBuffer(GLenum(GL_ELEMENT_ARRAY_BUFFER), 0);
    }

    // bool Shader::load(const std::string &vertShaderSource, const std::string
    // &fragShaderSource, GLuint &programPointer);
    static bool loadShader(const std::string &vertShaderSource,
                           const std::string &fragShaderSource,
                           GLuint &programPointer) {
        GLuint vertexShader(0), fragShader(0);

        programPointer = glCreateProgram();

        if (!Util::compileShader(vertexShader, GL_VERTEX_SHADER,
                                    vertShaderSource)) {
            return false;
        }

        if (!Util::compileShader(fragShader, GL_FRAGMENT_SHADER,
                                    fragShaderSource)) {
            return false;
        }

        // Attach vertex shader to program.
        glAttachShader(programPointer, vertexShader);

        // Attach fragment shader to program.
        glAttachShader(programPointer, fragShader);

        // Bind attribute locations.
        // This needs to be done prior to linking.
        glBindAttribLocation(programPointer, ATTRIB_VERTEX, "a_Position");
        Util::glErrorCheck();
        glBindAttribLocation(programPointer, ATTRIB_COLOR, "a_Color");
        Util::glErrorCheck();
        glBindAttribLocation(programPointer, ATTRIB_TEXTUREPOSITON,
                             "a_Texture");
        Util::glErrorCheck();
        //    glBindAttribLocation(programPointer, ATTRIB_TEXTUREPOSITON,
        //    "inputTextureCoordinate");

        // Link program.
        if (!Util::linkProgram(programPointer)) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                         "Failed to link program: %d", programPointer);

            if (vertexShader) {
                glDeleteShader(vertexShader);
                vertexShader = 0;
            }
            if (fragShader) {
                glDeleteShader(fragShader);
                fragShader = 0;
            }
            if (programPointer) {
                glDeleteProgram(programPointer);
                programPointer = 0;
            }

            return false;
        }

        if (!Util::validateProgram(programPointer)) {
            return false;
        }

        // Release vertex and fragment shaders.
        if (vertexShader) {
            glDeleteShader(vertexShader);
        }
        if (fragShader) {
            glDeleteShader(fragShader);
        }

        return true;
    }

    BackgroundRenderer *BackgroundRenderer::sBackgroundRenderer = nullptr;

    BackgroundRenderer::BackgroundRenderer() :
        mProgram(GL_INVALID_VALUE),
        mVao(GL_INVALID_VALUE),
        mVertexBuffer(GL_INVALID_VALUE),
        mIndexBuffer(GL_INVALID_VALUE),
        mVideoFrameTexture(GL_INVALID_VALUE),
        mBufferData(nullptr),
        mwidth(0),
        mheight(0),
        mchannels_in_file(0),
        mShouldReload(true),
        mTextureIndex(-1)
    {
        memset(&mUniforms[0], GL_INVALID_VALUE, sizeof(GLint)*NUM_UNIFORMS);
    }
    BackgroundRenderer::~BackgroundRenderer() {
        
    }

    BackgroundRenderer *BackgroundRenderer::getInstance() {
        if (nullptr == sBackgroundRenderer)
            sBackgroundRenderer = new BackgroundRenderer();
        return sBackgroundRenderer;
    }

    void BackgroundRenderer::destroyInstance() {
        if (nullptr != sBackgroundRenderer)
            delete sBackgroundRenderer;
        sBackgroundRenderer = nullptr;
    }
    void BackgroundRenderer::init(const std::string &file) {

        const std::string fragmentSource = R"(

        uniform sampler2D videoFrame;

        varying vec2 textureCoordinate;
        varying vec4 frag_Color;

        void main(void) {
            gl_FragColor = texture2D(videoFrame, textureCoordinate) * frag_Color;
        }

        )";

        const std::string vertexSource = R"(
         
        attribute vec4 a_Position;
        attribute vec4 a_Color;
        attribute vec4 a_Texture;

        varying vec2 textureCoordinate;
        varying vec4 frag_Color;

        void main(void) {
            frag_Color = a_Color;
            gl_Position = a_Position;
            textureCoordinate = a_Texture.xy;
        }


        )";

        if (loadShader(vertexSource, fragmentSource, mProgram)) {

            int width, height, channels_in_file;
            unsigned char *buffer = (unsigned char *)Util::loadImage(
                "assets_engine/loading.jpg", &width, &height, &channels_in_file);

            mBufferData = (unsigned char *)Util::loadImage(file.c_str(),
                                                           &mwidth,
                                                           &mheight,
                                                           &mchannels_in_file);

            mTextureIndex = 0;
            //            MaterialProperty::addReference(size_t(mTextureIndex));
            MaterialProperty::addReference(mTextureIndex);

            // Create a new texture from the camera frame data, display that
            // using the shaders
            glGenTextures(1, &mVideoFrameTexture);

            Util::glErrorCheck();
            glBindTexture(GL_TEXTURE_2D, mVideoFrameTexture);
            Util::glErrorCheck();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // This is necessary for non-power-of-two textures
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Using BGRA extension to pull in video frame data directly
            // GL_API void GL_APIENTRY glTexImage2D (GLenum target, GLint level,
            // GLint internalformat, GLsizei width, GLsizei height, GLint
            // border, GLenum format, GLenum type, const void *pixels);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width,
                         (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                         nullptr);
            Util::glErrorCheck();

            GLint videoFrame = glGetUniformLocation(mProgram, "videoFrame");
            mUniforms[UNIFORM_VIDEOFRAME] = videoFrame;

            setupVertexBuffer(mVao, mVertexBuffer, mIndexBuffer);

            glActiveTexture(GL_TEXTURE0 + mTextureIndex);
            Util::glErrorCheck();
            glBindTexture(GL_TEXTURE_2D, mVideoFrameTexture);
            Util::glErrorCheck();
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GLsizei(width),
                            GLsizei(height), GL_RGBA, GL_UNSIGNED_BYTE, buffer);
            Util::glErrorCheck();
            free(buffer);
        }
    }

    void BackgroundRenderer::unInit() {

        glDeleteProgram(mProgram);
        glDeleteTextures(1, &mVideoFrameTexture);
        glDeleteVertexArraysAPPLE(1, &mVao);
        glDeleteBuffers(GLsizei(1), &mVertexBuffer);
        glDeleteBuffers(GLsizei(1), &mIndexBuffer);
    }
    void BackgroundRenderer::render(GLuint width, GLuint height) {

        glViewport(0, 0, width * 2, height * 2);

        glUseProgram(mProgram);
        Util::glErrorCheck();

        glActiveTexture(GL_TEXTURE0 + mTextureIndex);
        Util::glErrorCheck();
        glBindTexture(GL_TEXTURE_2D, mVideoFrameTexture);
        Util::glErrorCheck();

        // Update uniform values
        glUniform1i(mUniforms[UNIFORM_VIDEOFRAME], mTextureIndex);
        Util::glErrorCheck();

        glBindVertexArrayAPPLE(mVao);
        Util::glErrorCheck();
        glDrawElements(GLenum(GL_TRIANGLES), GLsizei(6),
                       GLenum(GL_UNSIGNED_BYTE), 0);
        Util::glErrorCheck();
        glBindVertexArrayAPPLE(0);

        if (nullptr != mBufferData && mShouldReload) {
            glBindTexture(GL_TEXTURE_2D, mVideoFrameTexture);
            Util::glErrorCheck();
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GLsizei(mwidth),
                            GLsizei(mheight), GL_RGBA, GL_UNSIGNED_BYTE,
                            mBufferData);
            Util::glErrorCheck();
            mShouldReload = false;
        }
    }

} // namespace NJLIC
