//
//  NJLICSpauldingMedical.cpp
//  Spaulding Medical Test
//
//  Created by James Folk on 11/6/20.
//  Copyright © 2020 NJLIGames Ltd. All rights reserved.
//

#include "NJLICSpauldingMedical.h"

#include "BackgroundRenderer.h"
#include "BitmapFont.h"
#include "DebugDrawer.h"
#include "GraphGeometry.h"
#include "GraphicsPlatform.h"
#include "MaterialProperty.h"

#include <iostream>

void NJLICSpauldingMedical::loadView(const Ishne &ishne) {
    //    for(int i = 0; i < ishne.numberOfLeads(); i++) {
    //        NJLIC::GraphGeometry *g=new NJLIC::GraphGeometry();
    //        g->load(mShader, ishne.getValues(i));
    //        mGeometries.push_back(g);
    //    }

    NJLIC::Node *mGraphNode = new NJLIC::Node();
    NJLIC::GraphGeometry *mGraphGeometry = new NJLIC::GraphGeometry();

    mGraphGeometry->load(mShader, ishne.getValues(0), ishne.numberOfLeads());
    mGraphNode->addGeometry(mGraphGeometry);
    mScene->addActiveNode(mGraphNode);
    mGraphNode->setOrigin(glm::vec3(0.0, 0.0, 0.0));
    mScene->getRootNode()->addChildNode(mGraphNode);
}

NJLICSpauldingMedical::NJLICSpauldingMedical()
    : mDebugDrawer(new DebugDrawer), mShader(new NJLIC::Shader()),
      mCamera(new NJLIC::Camera()), mCameraNode(new NJLIC::Node()),
      mScene(new NJLIC::Scene()) {}

NJLICSpauldingMedical::~NJLICSpauldingMedical() {
    delete mScene;
    delete mCameraNode;
    delete mCamera;
    delete mShader;
    delete mDebugDrawer;
}

void NJLICSpauldingMedical::update(double step) {

    mScene->update(step);

    //    glm::vec3 from(0, 0, 0);
    //    glm::vec3 to(1, 1, 0);
    //    glm::vec3 color(1, 0, 0);

    //    static bool drew=false;
    //    if(!drew) {
    //        mDebugDrawer->drawLine(from, to, color);
    ////        drew=true;
    //
    //    }
}

void NJLICSpauldingMedical::render() const {
    //    printf("render\n");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    NJLIC::BackgroundRenderer::getInstance()->render(1920, 1080);
    mDebugDrawer->draw();

    glEnable(GL_DEPTH_TEST);
    mScene->render();
}

void NJLICSpauldingMedical::resize(unsigned int w, unsigned int h) {}
void NJLICSpauldingMedical::init(int argc, char *argv[]) {
    NJLIC::MaterialProperty::initReferences();

    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    BitmapFont::getInstance()->setCurrentFontName("FranklinGothicMedium");
    NJLIC::BackgroundRenderer::getInstance()->init(
        "assets/Medical-abstract-background-Polygon-and-dot-line-graphic-.jpg");

    mDebugDrawer->init();

    mCameraNode->addCamera(mCamera);
    mCameraNode->setOrigin(glm::vec3(0.0f, 0.0f, 0.0f));

    mScene->addActiveNode(mCameraNode);
    mScene->addActiveCamera(mCamera);
    mScene->getRootNode()->setOrigin(glm::vec3(0.0f, 0.0f, 1.0f));

    bool loaded = false;

    const char *vertShader =
        Util::loadFile("assets/shaders/StandardShader2.vert");
    const char *fragShader =
        Util::loadFile("assets/shaders/StandardShader2.frag");

    if (nullptr != vertShader && nullptr != fragShader) {
        const std::string &vertexSource(vertShader);
        const std::string &fragmentSource(fragShader);

        if (mShader->load(vertexSource, fragmentSource)) {
            //            const char *objData =
            //            Util::loadFile("assets/models/sprite.obj");
        }
    }

    NJLIC::Node *mTitleNode =
        BitmapFont::getInstance()->printf(mScene, "%s", "A");
    mScene->addActiveNode(mTitleNode);
    mTitleNode->setOrigin(glm::vec3(0.0, 0.0, 0.0));
    mScene->getRootNode()->addChildNode(mTitleNode);
}

void NJLICSpauldingMedical::unInit() {
    mDebugDrawer->unInit();

    NJLIC::BackgroundRenderer::destroyInstance();
    BitmapFont::destroyInstance();

    while (!mGeometries.empty()) {
        auto g = mGeometries.back();
        delete g;
        mGeometries.pop_back();
    }
}

void NJLICSpauldingMedical::start() {}

void NJLICSpauldingMedical::stop() {}
void NJLICSpauldingMedical::mouse(int button, int eventType, float x, float y,
                                  int clicks) {}
void NJLICSpauldingMedical::keyDown(const std::string &keycodeName,
                                    bool withCapsLock, bool withControl,
                                    bool withShift, bool withAlt,
                                    bool withGui) {}
void NJLICSpauldingMedical::keyUp(const std::string &keycodeName,
                                  bool withCapsLock, bool withControl,
                                  bool withShift, bool withAlt, bool withGui) {}

void NJLICSpauldingMedical::touch(int touchDevId, int pointerFingerId,
                                  int eventType, float x, float y, float dx,
                                  float dy, float pressure) {}

void NJLICSpauldingMedical::vRCameraRotation(float m11, float m12, float m13,
                                             float m21, float m22, float m23,
                                             float m31, float m32, float m33) {}

void NJLICSpauldingMedical::vRCameraRotationYPR(float yaw, float pitch,
                                                float roll) {}

void NJLICSpauldingMedical::fileDrop(const std::string &fileName) {
    Ishne *ishne = new Ishne();

    try {
        ishne->init(fileName);
    } catch (const std::runtime_error &error) {
        std::cout << error.what() << std::endl;
    }

    loadView(*ishne);
    delete ishne;
}
