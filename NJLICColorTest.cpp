//
//  NJLICColorTest.cpp
//  SDLSkeleton
//
//  Created by James Folk on 5/16/20.
//

#include "NJLICColorTest.h"

#include "GraphicsPlatform.h"

NJLICColorTest::NJLICColorTest() { printf("NJLICColorTest\n"); }
NJLICColorTest::~NJLICColorTest() { printf("~NJLICColorTest\n"); }

void NJLICColorTest::update(double step) {
    //    printf("update\n");
    float h(std::numeric_limits<float>::denorm_min());
    mColor = Color::modifyHue(mColor, step);
}
void NJLICColorTest::render() const{
    //    printf("render\n");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glClearColor(mColor.red(), mColor.green(), mColor.blue(), 1.0f);
}

void NJLICColorTest::resize(unsigned int w, unsigned int h) {
    printf("resize\n");
}
void NJLICColorTest::init(int argc, char *argv[]) {
    printf("init\n");
    mColor.setRGB("FF0000");
    mColor.setRGB(0xFF00FF);
    mColor.setRGB(glm::vec4(1.0, 0., 0.0, 1.0));

    glClearColor(mColor.red(), mColor.green(), mColor.blue(), 1.0f);
}
void NJLICColorTest::unInit() { printf("unInit\n"); }

void NJLICColorTest::start() { printf("start\n"); }

void NJLICColorTest::stop() { printf("stop\n"); }
void NJLICColorTest::mouse(int button, int eventType, float x, float y,
                           int clicks) {
//    printf("mouse\n");
}
void NJLICColorTest::keyDown(const std::string &keycodeName, bool withCapsLock,
                             bool withControl, bool withShift, bool withAlt,
                             bool withGui) {
//    printf("keyDown\n");
}
void NJLICColorTest::keyUp(const std::string &keycodeName, bool withCapsLock,
                           bool withControl, bool withShift, bool withAlt,
                           bool withGui) {
//    printf("keyUp\n");
}

void NJLICColorTest::touch(int touchDevId, int pointerFingerId, int eventType,
                           float x, float y, float dx, float dy,
                           float pressure) {
    printf("touch\n");
}

void NJLICColorTest::vRCameraRotation(float m11, float m12, float m13,
                                      float m21, float m22, float m23,
                                      float m31, float m32, float m33) {}

void NJLICColorTest::vRCameraRotationYPR(float yaw, float pitch, float roll) {}
