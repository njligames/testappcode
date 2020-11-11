//
//  NJLICSpauldingMedical.h
//  SDLSkeleton
//
//  Created by James Folk on 11/6/20.
//

#ifndef NJLICSpauldingMedical_h
#define NJLICSpauldingMedical_h

#include "Color.h"
#include "NJLICGame.h"
#include "Ishne.h"
#include "GraphGeometry.h"
#include "Shader.h"
#include "Camera.h"
#include "Node.h"
#include "Scene.h"
#include "GraphPlot.h"
#include "PubSub.h"

#include <vector>

class DebugDrawer;



class NJLICSpauldingMedical : public NJLICGame, public Subscriber {
    DebugDrawer *mDebugDrawer;
    std::vector<NJLIC::GraphGeometry*> mGeometries;
    
    NJLIC::Shader *mShader;
    NJLIC::Camera *mCamera;
    NJLIC::Node *mCameraNode;
    NJLIC::Scene *mScene;
    
    NJLIC::ListItemNode *mListNodeItem;
    bool mIsScrolling;
    
    void loadView(const Ishne &ishne);

  public:
    NJLICSpauldingMedical();
    virtual ~NJLICSpauldingMedical();

    virtual void update(double step) override;
    virtual void render()const override;
    virtual void resize(unsigned int w, unsigned int h) override;
    virtual void init(int argc, char *argv[]) override;
    virtual void unInit() override;
    virtual void start() override;
    virtual void stop() override;

    virtual void mouse(int button, int eventType, float x, float y,
                       int clicks) override;
    virtual void keyDown(const std::string &keycodeName, bool withCapsLock,
                         bool withControl, bool withShift, bool withAlt,
                         bool withGui) override;
    virtual void keyUp(const std::string &keycodeName, bool withCapsLock,
                       bool withControl, bool withShift, bool withAlt,
                       bool withGui) override;
    virtual void touch(int touchDevId, int pointerFingerId, int eventType,
                       float x, float y, float dx, float dy,
                       float pressure) override;
    virtual void vRCameraRotation(float m11, float m12, float m13, float m21,
                                  float m22, float m23, float m31, float m32,
                                  float m33) override;

    virtual void vRCameraRotationYPR(float yaw, float pitch,
                                     float roll) override;
    
    virtual void fileDrop(const std::string &fileName)override;
    
    virtual void update(Publisher *who, void *userdata = 0) override;
    
};

#endif /* NJLICSpauldingMedical_h */
