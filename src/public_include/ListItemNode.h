//
//  ListItemNode.h
//  Spaulding Medical Test
//
//  Created by James Folk on 11/6/20.
//  Copyright © 2020 NJLIGames Ltd. All rights reserved.
//

#ifndef ListItemNode_h
#define ListItemNode_h

#include "Node.h"
#include "PubSub.h"

namespace NJLIC {
class ListItemNode : public NJLIC::Node, public Publisher {
    glm::vec3 mNextPosition;
    glm::vec3 mPrevPosition;
    glm::vec3 mDirection;
    glm::vec3 mTargetPosition;
    float mTotalTime;
    float mCurrentTime;
    int mAlgorithm;
    float mCurrentDistance;

  public:
    ListItemNode();
    virtual ~ListItemNode();

    void setNextPosition(const glm::vec3 &next);
    void setPreviousPosition(const glm::vec3 &previousPosition);
    void scrollNext(float seconds = 1.0, int algorithm = 0);
    void scrollPrevious(float seconds = 1.0, int algorithm = 0);

    virtual void update(float timestep) override;

  protected:
    // Robert Penner's easing functions in GLSL
    // https://github.com/stackgl/glsl-easings
    // http://easings.net
    static float linear(float t);
    static float exponentialIn(float t);
    static float exponentialOut(float t);
    static float exponentialInOut(float t);
    static float sineIn(float t);
    static float sineOut(float t);
    static float sineInOut(float t);
    static float qinticIn(float t);
    static float qinticOut(float t);
    static float qinticInOut(float t);
    static float quarticIn(float t);
    static float quarticOut(float t);
    static float quarticInOut(float t);
    static float quadraticInOut(float t);
    static float quadraticIn(float t);
    static float quadraticOut(float t);
    static float cubicIn(float t);
    static float cubicOut(float t);
    static float cubicInOut(float t);
    static float elasticIn(float t);
    static float elasticOut(float t);
    static float elasticInOut(float t);
    static float circularIn(float t);
    static float circularOut(float t);
    static float circularInOut(float t);
    static float bounceOut(float t);
    static float bounceIn(float t);
    static float bounceInOut(float t);
    static float backIn(float t);
    static float backOut(float t);
    static float backInOut(float t);
};
}
#endif /* ListItemNode_h */
