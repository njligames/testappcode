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
#include "GraphPlot.h"
#include "GraphicsPlatform.h"
#include "ListItemNode.h"
#include "MaterialProperty.h"

#include <iostream>

void NJLICSpauldingMedical::loadView(const Ishne &ishne) {

    NJLIC::MaterialProperty *mMaterialPropertyGraph =
        new NJLIC::MaterialProperty();
    NJLIC::MaterialProperty *mMaterialPropertyIndicator =
        new NJLIC::MaterialProperty();

    int width, height, num_components;

    unsigned char *buffer = (unsigned char *)Util::loadImage(
        "assets/graph_line.jpg", &width, &height, &num_components);

    if (nullptr != buffer) {

        mMaterialPropertyGraph->load(mShader, buffer, width, height,
                                     num_components);

        free(buffer);
    }

    buffer = (unsigned char *)Util::loadImage("assets/indicator_line.jpg",
                                              &width, &height, &num_components);

    if (nullptr != buffer) {

        mMaterialPropertyIndicator->load(mShader, buffer, width, height,
                                         num_components);

        free(buffer);
    }

    for (int i = 0; i < ishne.numberOfLeads(); i++) {

        NJLIC::GraphPlot *graphPlot = new NJLIC::GraphPlot();
        NJLIC::Node *node =
            graphPlot->init(mShader, mScene, ishne.getValues(i), i,
                            mMaterialPropertyGraph, mMaterialPropertyIndicator);
        mListNodeItem->addChildNode(node);
    }
    mScene->addActiveNode(mListNodeItem);
    mScene->getRootNode()->addChildNode(mListNodeItem);

    glm::vec3 pos(mListNodeItem->getOrigin());
    pos.x -= NJLIC::Geometry::sScale3DTo2D * 200;
    mListNodeItem->setNextPosition(pos);

    mListNodeItem->subscribe(this);
}

NJLICSpauldingMedical::NJLICSpauldingMedical()
    : mDebugDrawer(new DebugDrawer), mShader(new NJLIC::Shader()),
      mCamera(new NJLIC::Camera()), mCameraNode(new NJLIC::Node()),
      mScene(new NJLIC::Scene()), mListNodeItem(new NJLIC::ListItemNode()) {}

NJLICSpauldingMedical::~NJLICSpauldingMedical() {
    delete mScene;
    delete mCameraNode;
    delete mCamera;
    delete mShader;
    delete mDebugDrawer;
}

void NJLICSpauldingMedical::update(double step) { mScene->update(step); }

void NJLICSpauldingMedical::render() const {
    static size_t sWidth(4000);
    static size_t sHeight(1080);
    //    printf("render\n");
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    NJLIC::BackgroundRenderer::getInstance()->render(sWidth, sHeight);
    mDebugDrawer->draw();

    glViewport(-sWidth, -sWidth, sWidth << 1, sWidth << 1);
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
    mScene->getRootNode()->setOrigin(glm::vec3(0.0f, 0.0f, 3.0f));

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
                                  bool withShift, bool withAlt, bool withGui) {

    if ("Right" == keycodeName) {
        mListNodeItem->scrollNext(1, 0);
    } else if ("Left" == keycodeName) {
        mListNodeItem->scrollPrevious(1, 0);
    }
}

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

void NJLICSpauldingMedical::update(Publisher *who, void *userdata) {
    glm::vec3 posNext(mListNodeItem->getOrigin());
    glm::vec3 posPrev(mListNodeItem->getOrigin());

    posNext.x -= NJLIC::Geometry::sScale3DTo2D * 200;
    mListNodeItem->setNextPosition(posNext);

    posPrev.x += NJLIC::Geometry::sScale3DTo2D * 200;
    mListNodeItem->setPreviousPosition(posPrev);
}
