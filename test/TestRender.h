//
//  TestRender.h
//  SDLSkeleton
//
//  Created by James Folk on 4/1/20.
//

#ifndef TestRender_hpp
#define TestRender_hpp

#include "GraphicsPlatform.h"

#include "gtest/gtest.h"

#include "SDL_test_common.h"
#include <mutex>
#include <thread>

class Graphics;

// The fixture for testing class Foo.
class TestRender : public ::testing::Test {

  protected:
    // You can do set-up work for each test here.
    TestRender();

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestRender();
    void handleInput();
    void holdView(unsigned int seconds);
    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp();

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown();

    SDLTest_CommonState *state;
    typedef struct {
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *background;
        SDL_Texture *sprite;
        SDL_Rect sprite_rect;
        int scale_direction;
    } DrawState;
    DrawState *drawstates;

    SDL_GLContext glContext;

    std::thread *updateThread;
    bool mDone;

    std::mutex mMutex;
    std::thread *mThread;

    std::unique_ptr<Graphics> mGraphics;

    const std::string currentDateTime() const;
    double mTotalSteps;

  public:
    virtual void update(double step);
    virtual void render();

    void setDone() {
        mMutex.lock();
        mDone = true;
        mMutex.unlock();
    }
    bool isDone() const { return mDone; }
    Graphics *getGraphics() const { return mGraphics.get(); }

    void screenShot() const;
    double totalSteps() const { return mTotalSteps; }
};

#endif /* TestRender_hpp */
