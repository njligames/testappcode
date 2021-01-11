//
//  TestBackgroundRenderer.cpp
//  game
//
//  Created by James Folk on 1/9/21.
//

#include "TestBackgroundRenderer.hpp"

#include "BackgroundRenderer.h"
#include "MaterialProperty.h"

TestBackgroundRenderer::TestBackgroundRenderer() {}

TestBackgroundRenderer::~TestBackgroundRenderer(){};

void TestBackgroundRenderer::SetUp() {
    TestRender::SetUp();
    
    NJLIC::MaterialProperty::initReferences();

    mColor.setRGB("FF0000");
//    mColor.setRGB(0xFF00FF);
//    mColor.setRGB(glm::vec4(1.0, 0., 0.0, 1.0));

    glClearColor(mColor.red(), mColor.green(), mColor.blue(), 1.0f);
    
    NJLIC::BackgroundRenderer::getInstance()->init(
        "assets/Medical-abstract-background-Polygon-and-dot-line-graphic-.jpg");
}

void TestBackgroundRenderer::TearDown() { TestRender::TearDown();
    NJLIC::BackgroundRenderer::destroyInstance();
}

void TestBackgroundRenderer::update(double step) {
    float h(std::numeric_limits<float>::denorm_min());
//    mColor = Color::modifyHue(mColor, 0.0001);
}
void TestBackgroundRenderer::render() {

    
    
    
    static size_t sWidth(1920);
    static size_t sHeight(1080);
    //    printf("render\n");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    NJLIC::BackgroundRenderer::getInstance()->render(sWidth, sHeight);
    
    
    glViewport(-4000, -4000, 4000 << 1, 4000 << 1);
    
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

//    glClearColor(mColor.red(), mColor.green(), mColor.blue(), 1.0f);

    //    printf("%f,%f,%f", mColor.red(), mColor.green(), mColor.blue());
}

//static void test_rgb(const unsigned &r, const unsigned &g, const unsigned &b) {
//    glm::vec4 rgb_before(glm::vec4(r / 255., g / 255., b / 255., 1.0));
//
//    glm::vec4 hsv(Color::rgb2hsv(rgb_before));
//    glm::vec4 rgb_after(Color::hsv2rgb(hsv));
//
//    ASSERT_EQ(rgb_before.r, rgb_after.r);
//    ASSERT_EQ(rgb_before.g, rgb_after.g);
//    ASSERT_EQ(rgb_before.b, rgb_after.b);
//    ASSERT_EQ(rgb_before.a, rgb_after.a);
//
//    ASSERT_EQ(rgb_before, rgb_after);
//}
//
//static void test_hsv(const float &h, const unsigned &s, const unsigned &v) {
//
//    glm::vec4 hsv_before(glm::vec4(h, s / 255., v / 255., 1.0));
//
//    glm::vec4 rgb(Color::hsv2rgb(hsv_before));
//    glm::vec4 hsv_after(Color::rgb2hsv(rgb));
//
//    ASSERT_EQ(hsv_before.r, hsv_after.r);
//    ASSERT_EQ(hsv_before.g, hsv_after.g);
//    ASSERT_EQ(hsv_before.b, hsv_after.b);
//    ASSERT_EQ(hsv_before.a, hsv_after.a);
//
//    ASSERT_EQ(hsv_before, hsv_after);
//}
//
//TEST_F(TestBackgroundRenderer, rgb2hsv) {
//
//    test_rgb(255, 0, 0);
//    test_rgb(255, 255, 0);
//    test_rgb(0, 255, 0);
//    test_rgb(0, 255, 255);
//    test_rgb(0, 0, 255);
//    test_rgb(255, 0, 255);
//    test_rgb(255, 255, 255);
//    test_rgb(128, 128, 128);
//    test_rgb(0, 0, 0);
//    test_rgb(255, 128, 0);
//}
//
//TEST_F(TestBackgroundRenderer, hsv2rgb) {
//
//    test_hsv(0, 255, 255);
//    test_hsv(30, 255, 255);
//    test_hsv(60, 255, 255);
//    test_hsv(90, 255, 255);
//    test_hsv(120, 255, 255);
//    test_hsv(150, 255, 255);
//    test_hsv(0, 0, 255);
//    test_hsv(0, 0, 128);
//    test_hsv(0, 0, 0);
//    test_hsv(0, 255, 255);
//}

TEST_F(TestBackgroundRenderer, modifyHue) {

    ASSERT_EQ(true, true);

    this->holdView(10);
}
