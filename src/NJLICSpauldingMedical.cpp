//
//  NJLICSpauldingMedical.cpp
//  Spaulding Medical Test
//
//  Created by James Folk on 11/6/20.
//  Copyright © 2020 NJLIGames Ltd. All rights reserved.
//

#include "NJLICSpauldingMedical.h"

#include "GraphicsPlatform.h"
#include "Ishne.h"
#include "BitmapFont.h"
#include "BackgroundRenderer.h"
#include "MaterialProperty.h"

#include <iostream>

NJLICSpauldingMedical::NJLICSpauldingMedical() :
mIshne(new Ishne) {
    
}

NJLICSpauldingMedical::~NJLICSpauldingMedical() {
    delete mIshne;
}

void NJLICSpauldingMedical::update(double step) {
    
}
void NJLICSpauldingMedical::render() const {
    //    printf("render\n");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    NJLIC::BackgroundRenderer::getInstance()->render(1920, 1080);
}

void NJLICSpauldingMedical::resize(unsigned int w, unsigned int h) {
    
}
void NJLICSpauldingMedical::init(int argc, char *argv[]) {
    NJLIC::MaterialProperty::initReferences();
    
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    BitmapFont::getInstance()->setCurrentFontName("FranklinGothicMedium");
    NJLIC::BackgroundRenderer::getInstance()->init("assets/Medical-abstract-background-Polygon-and-dot-line-graphic-.jpg");
}
void NJLICSpauldingMedical::unInit() {
    NJLIC::BackgroundRenderer::destroyInstance();
    BitmapFont::destroyInstance();
}

void NJLICSpauldingMedical::start() {
    
}

void NJLICSpauldingMedical::stop() {
    
}
void NJLICSpauldingMedical::mouse(int button, int eventType, float x, float y,
                           int clicks) {
}
void NJLICSpauldingMedical::keyDown(const std::string &keycodeName, bool withCapsLock,
                             bool withControl, bool withShift, bool withAlt,
                             bool withGui) {
}
void NJLICSpauldingMedical::keyUp(const std::string &keycodeName, bool withCapsLock,
                           bool withControl, bool withShift, bool withAlt,
                           bool withGui) {
}

void NJLICSpauldingMedical::touch(int touchDevId, int pointerFingerId, int eventType,
                           float x, float y, float dx, float dy,
                           float pressure) {
}

void NJLICSpauldingMedical::vRCameraRotation(float m11, float m12, float m13,
                                      float m21, float m22, float m23,
                                      float m31, float m32, float m33) {}

void NJLICSpauldingMedical::vRCameraRotationYPR(float yaw, float pitch, float roll) {}

void NJLICSpauldingMedical::fileDrop(const std::string &fileName) {
    try {
        mIshne->init(fileName);
    } catch (const std::runtime_error& error) {
        std::cout << error.what() << std::endl;
    }
}
