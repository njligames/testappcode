//
//  NJLICColorTest.h
//  SDLSkeleton
//
//  Created by James Folk on 5/16/20.
//

#ifndef NJLICColorTest_h
#define NJLICColorTest_h

#include "Color.h"
#include "NJLICGame.h"

class NJLICColorTest : public NJLICGame {
    Color mColor;

  public:
    NJLICColorTest();
    virtual ~NJLICColorTest();

    virtual void update(double step) override;
    virtual void render()const override;
    virtual void resize(unsigned int w, unsigned int h)const override;
    virtual void init(int argc, char *argv[]) override;
    virtual void unInit() override;
    virtual void start()const override;
    virtual void stop()const override;

    virtual void mouse(int button, int eventType, float x, float y,
                       int clicks)const override;
    virtual void keyDown(const std::string &keycodeName, bool withCapsLock,
                         bool withControl, bool withShift, bool withAlt,
                         bool withGui)const override;
    virtual void keyUp(const std::string &keycodeName, bool withCapsLock,
                       bool withControl, bool withShift, bool withAlt,
                       bool withGui)const override;
    virtual void touch(int touchDevId, int pointerFingerId, int eventType,
                       float x, float y, float dx, float dy,
                       float pressure)const override;
    virtual void vRCameraRotation(float m11, float m12, float m13, float m21,
                                  float m22, float m23, float m31, float m32,
                                  float m33)const override;

    virtual void vRCameraRotationYPR(float yaw, float pitch,
                                     float roll)const override;
};

#endif /* NJLICColorTest_h */
