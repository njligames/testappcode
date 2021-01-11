//
//  TestBackgroundRenderer.hpp
//  game
//
//  Created by James Folk on 1/9/21.
//

#ifndef TestBackgroundRenderer_hpp
#define TestBackgroundRenderer_hpp

#include "gtest/gtest.h"

#include "Color.h"
#include "TestRender.h"

// The fixture for testing class Foo.
class TestBackgroundRenderer : public TestRender {

  protected:
    // You can do set-up work for each test here.
    TestBackgroundRenderer();

    // You can do clean-up work that doesn't throw exceptions here.
    virtual ~TestBackgroundRenderer();
    void handleInput();
    //    virtual void holdView(unsigned int seconds);
    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    // Code here will be called immediately after the constructor (right
    // before each test).
    virtual void SetUp();

    // Code here will be called immediately after each test (right
    // before the destructor).
    virtual void TearDown();

  public:
    virtual void update(double step) final;
    virtual void render() final;

  private:
    Color mColor;
};

#endif /* TestBackgroundRenderer_hpp */
