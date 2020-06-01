//
//  TestRender.cpp
//  SDLSkeleton
//
//  Created by James Folk on 4/1/20.
//

#include "TestRender.h"

#include <string>

//#include "TestClass.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

using namespace std;


class Graphics {
  private:
    SDL_Window *_window;
    TestRender *_renderer;

  public:
    Graphics(SDL_Window *window, TestRender *renderer)
        : _window(window), _renderer(renderer) {}

    void update() {

        _renderer->render();

        SDL_GL_SwapWindow(_window);
    }
};

TestRender::TestRender() {}

TestRender::~TestRender(){};


static void UpdateFrame(void *param) {
    Graphics *graphics = (Graphics *)param;
    graphics->update();
}

static void RenderFrame(void *param) {
    
    Graphics *graphics = (Graphics *)param;
    graphics->update();
}

static void handleInput(TestRender *renderer) {
    bool callFinishKeys = false;
    SDL_Event event;
    SDL_PumpEvents();
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            renderer->setDone();
            break;
        case SDL_APP_WILLENTERBACKGROUND:
            SDL_Log("SDL_APP_WILLENTERBACKGROUND");
//#if (defined(__IPHONEOS__) && __IPHONEOS__)
//            SDL_iPhoneSetAnimationCallback(state->windows[0], 1, NULL,
//                                           renderer->getGraphics());
//#endif
            break;
        case SDL_APP_DIDENTERFOREGROUND:
            SDL_Log("SDL_APP_DIDENTERFOREGROUND");

//#if (defined(__IPHONEOS__) && __IPHONEOS__)
//            SDL_iPhoneSetAnimationCallback(state->windows[0], 1, UpdateFrame,
//                                           renderer->getGraphics());
//            SDL_iPhoneSetEventPump(SDL_TRUE);
//#endif
            break;
        }
    }
}

static auto startTime = chrono::steady_clock::now();
static auto endTime = chrono::steady_clock::now();
static unsigned int seconds;

static void Update(TestRender *renderer) {
    const double FPS = 60.;
    const double FPms = FPS / 1000.;

    std::chrono::steady_clock::time_point lastFrameTime =
        std::chrono::steady_clock::now();

    double totalFPS = 0.0;
    while (!renderer->isDone()) {

        double step = std::chrono::duration_cast<std::chrono::microseconds>(
                          std::chrono::steady_clock::now() - lastFrameTime)
                          .count() /
                      1000.0;

        totalFPS += step;

        if (totalFPS >= FPms) {
            totalFPS = totalFPS - FPms;
            step = FPms;
        }

        renderer->update(step);

        lastFrameTime = std::chrono::steady_clock::now();
    }
}

static void mainloop(void *param) {
    TestRender *renderer = (TestRender *)param;
    handleInput(renderer);
#if !(defined(__IPHONEOS__) && __IPHONEOS__)
    RenderFrame(renderer->getGraphics());
#endif

    endTime = chrono::steady_clock::now();

    if (seconds <
        chrono::duration_cast<chrono::seconds>(endTime - startTime).count()) {
        renderer->setDone();
    }
}
void TestRender::holdView(unsigned int s) {
    seconds = s;
    startTime = chrono::steady_clock::now();

#if defined(__EMSCRIPTEN__)
    //        emscripten_set_main_loop(mainloop, 0, 0);
    emscripten_set_main_loop_arg(mainloop, this, 0, 0);
#else

    mThread = new std::thread(Update, this);

    while (!mDone) {
        mainloop(this);
    }

    mDone = false;
    mThread->join();
    delete mThread;
#endif
}

void TestRender::SetUp() {

    mDone = true;

    SDL_SetMainReady();

    char *argv[] = {(char *)"", nullptr};
    state = SDLTest_CommonCreateState(argv, SDL_INIT_VIDEO);

    if (SDLTest_CommonInit(state)) {
        /* Create the windows and initialize the renderers */
        for (int i = 0; i < state->num_windows; ++i) {
            SDL_Renderer *renderer = state->renderers[i];
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
            SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF);
            SDL_RenderClear(renderer);
        }

        glContext = SDL_GL_CreateContext(state->windows[0]);

        mGraphics =
            std::unique_ptr<Graphics>(new Graphics(state->windows[0], this));
        mDone = false;

        glClearColor(1.0, 0.0, 0.0, 1.0f);
    }
};

void TestRender::TearDown() {

    SDLTest_CommonQuit(state);
    SDL_GL_DeleteContext(glContext);
};

void TestRender::update(double step) {}
void TestRender::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


TEST_F(TestRender, basic) {
    bool ret = true;

    glClearColor(0.0, 1.0, 0.0, 1.0f);

    ASSERT_EQ(true, ret);
    
}
