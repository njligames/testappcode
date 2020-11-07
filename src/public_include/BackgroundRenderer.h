
////
////  BackgroundRenderer.h
////  SDLSkeleton
////
////  Created by James Folk on 2/25/20.
////

#include "GraphicsPlatform.h"
#include <string>

namespace NJLIC {
    class BackgroundRenderer {
        static BackgroundRenderer *sBackgroundRenderer;

        enum { UNIFORM_VIDEOFRAME, NUM_UNIFORMS };

        GLint mUniforms[NUM_UNIFORMS];
        GLuint mProgram;

        GLuint mVao;
        GLuint mVertexBuffer;
        GLuint mIndexBuffer;
        GLuint mVideoFrameTexture;

        unsigned char *mBufferData;
        int mwidth, mheight, mchannels_in_file;
        bool mShouldReload;

        int mTextureIndex;

        BackgroundRenderer();
        ~BackgroundRenderer();
        
        BackgroundRenderer(const BackgroundRenderer &rhs) = delete;
        const BackgroundRenderer &operator=(const BackgroundRenderer &rhs) = delete;
        BackgroundRenderer(const BackgroundRenderer &&rhs) = delete;
        const BackgroundRenderer &&operator=(const BackgroundRenderer &&rhs) = delete;
      public:
        static BackgroundRenderer *getInstance();
        static void destroyInstance();

        void init(const std::string &file="");
        void unInit();
        void render(GLuint width, GLuint height);
    };
} // namespace NJLIC
