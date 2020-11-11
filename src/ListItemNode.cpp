//
//  ListItemNode.cpp
//  Spaulding Medical Test
//
//  Created by James Folk on 11/6/20.
//  Copyright © 2020 NJLIGames Ltd. All rights reserved.
//

#include "ListItemNode.h"

namespace NJLIC {
    ListItemNode::ListItemNode()
        : mNextPosition(glm::vec3(0, 0, 0)), mPrevPosition(glm::vec3(0, 0, 0)),
          mDirection(glm::vec3(0, 0, 0)), mTargetPosition(glm::vec3(0, 0, 0)),
          mTotalTime(0), mCurrentTime(0), mAlgorithm(0), mCurrentDistance(0) {}

    ListItemNode::~ListItemNode() {}

    void ListItemNode::setNextPosition(const glm::vec3 &next) {
        mNextPosition = next;
    }

    void ListItemNode::setPreviousPosition(const glm::vec3 &previousPosition) {
        mPrevPosition = previousPosition;
    }

    void ListItemNode::scrollNext(float seconds, int algorithm) {
        if (seconds <= 0) {
            mDirection = glm::vec3(0, 0, 0);
            setOrigin(mNextPosition);
            mCurrentTime = 0.0;

            notify();
        } else {
            if (glm::length(mDirection) <= 0) {
                mDirection = (mNextPosition - getOrigin());
                mCurrentDistance = glm::length(mDirection);

                mTargetPosition = mNextPosition;

                mTotalTime =
                    std::max(seconds,
                             std::numeric_limits<float>::denorm_min()) *
                    17;
                mCurrentTime = 0.0;

                mAlgorithm = algorithm;
                // random number from 0 to 30, inclusively
                if (algorithm < 0)
                    mAlgorithm = 0;
                if (algorithm > 30)
                    mAlgorithm = 30;
            }
        }
    }

    void ListItemNode::scrollPrevious(float seconds, int algorithm) {
        if (seconds <= 0) {
            mDirection = glm::vec3(0, 0, 0);
            setOrigin(mPrevPosition);
            mCurrentTime = 0.0;

            notify();
        } else {
            if (glm::length(mDirection) <= 0) {
                mDirection = (mPrevPosition - getOrigin());
                mCurrentDistance = glm::length(mDirection);

                mTargetPosition = mPrevPosition;

                mTotalTime =
                    std::max(seconds,
                             std::numeric_limits<float>::denorm_min()) *
                    17;
                mCurrentTime = 0.0;

                mAlgorithm = algorithm;
                // random number from 0 to 30, inclusively
                if (algorithm < 0)
                    mAlgorithm = 0;
                if (algorithm > 30)
                    mAlgorithm = 30;
            }
        }
    }

    void ListItemNode::update(float timestep) {
        float length = glm::length(mDirection);

        if (length > 0) {

            mCurrentTime += timestep;

            float time(0.0);

            switch (mAlgorithm) {
            case 0:
                time = linear(mCurrentTime / mTotalTime);
                break;
            case 1:
                time = exponentialIn(mCurrentTime / mTotalTime);
                break;
            case 2:
                time = exponentialOut(mCurrentTime / mTotalTime);
                break;
            case 3:
                time = exponentialInOut(mCurrentTime / mTotalTime);
                break;
            case 4:
                time = sineIn(mCurrentTime / mTotalTime);
                break;
            case 5:
                time = sineOut(mCurrentTime / mTotalTime);
                break;
            case 6:
                time = sineInOut(mCurrentTime / mTotalTime);
                break;
            case 7:
                time = qinticIn(mCurrentTime / mTotalTime);
                break;
            case 8:
                time = qinticOut(mCurrentTime / mTotalTime);
                break;
            case 9:
                time = qinticInOut(mCurrentTime / mTotalTime);
                break;
            case 10:
                time = quarticIn(mCurrentTime / mTotalTime);
                break;
            case 11:
                time = quarticOut(mCurrentTime / mTotalTime);
                break;
            case 12:
                time = quarticInOut(mCurrentTime / mTotalTime);
                break;
            case 13:
                time = quadraticInOut(mCurrentTime / mTotalTime);
                break;
            case 14:
                time = quadraticIn(mCurrentTime / mTotalTime);
                break;
            case 15:
                time = quadraticOut(mCurrentTime / mTotalTime);
                break;
            case 16:
                time = cubicIn(mCurrentTime / mTotalTime);
                break;
            case 17:
                time = cubicOut(mCurrentTime / mTotalTime);
                break;
            case 18:
                time = cubicInOut(mCurrentTime / mTotalTime);
                break;
            case 19:
                time = elasticIn(mCurrentTime / mTotalTime);
                break;
            case 20:
                time = elasticOut(mCurrentTime / mTotalTime);
                break;
            case 21:
                time = elasticInOut(mCurrentTime / mTotalTime);
                break;
            case 22:
                time = circularIn(mCurrentTime / mTotalTime);
                break;
            case 23:
                time = circularOut(mCurrentTime / mTotalTime);
                break;
            case 24:
                time = circularInOut(mCurrentTime / mTotalTime);
                break;
            case 25:
                time = bounceOut(mCurrentTime / mTotalTime);
                break;
            case 26:
                time = bounceIn(mCurrentTime / mTotalTime);
                break;
            case 27:
                time = bounceInOut(mCurrentTime / mTotalTime);
                break;
            case 28:
                time = backIn(mCurrentTime / mTotalTime);
                break;
            case 29:
                time = backOut(mCurrentTime / mTotalTime);
                break;
            case 30:
                time = backInOut(mCurrentTime / mTotalTime);
                break;

            default:
                break;
            }

            glm::vec3 origin(getOrigin());

            glm::vec3 distance(mDirection * time);

            mCurrentDistance -= glm::length(distance);

            origin += distance;

            setOrigin(origin);

            if (mCurrentDistance <= 0) {
                mDirection = glm::vec3(0, 0, 0);
                setOrigin(mTargetPosition);
                mCurrentTime = 0.0;

                notify();
            }
        }
    }

    // Robert Penner's easing functions in GLSL
    // https://github.com/stackgl/glsl-easings
    float ListItemNode::linear(float t) {
        assert(t >= 0 && t <= 1.0);
        return t;
    }

    float ListItemNode::exponentialIn(float t) {
        assert(t >= 0 && t <= 1.0);
        return t == 0.0 ? t : pow(2.0, 10.0 * (t - 1.0));
    }

    float ListItemNode::exponentialOut(float t) {
        assert(t >= 0 && t <= 1.0);
        return t == 1.0 ? t : 1.0 - pow(2.0, -10.0 * t);
    }

    float ListItemNode::exponentialInOut(float t) {
        assert(t >= 0 && t <= 1.0);
        return t == 0.0 || t == 1.0
                   ? t
                   : t < 0.5 ? +0.5 * pow(2.0, (20.0 * t) - 10.0)
                             : -0.5 * pow(2.0, 10.0 - (t * 20.0)) + 1.0;
    }

    float ListItemNode::sineIn(float t) {
        assert(t >= 0 && t <= 1.0);
        return sin((t - 1.0) * M_PI_2) + 1.0;
    }

    float ListItemNode::sineOut(float t) {
        assert(t >= 0 && t <= 1.0);
        return sin(t * M_PI_2);
    }

    float ListItemNode::sineInOut(float t) {
        assert(t >= 0 && t <= 1.0);
        return -0.5 * (cos(M_PI * t) - 1.0);
    }

    float ListItemNode::qinticIn(float t) {
        assert(t >= 0 && t <= 1.0);
        return pow(t, 5.0);
    }

    float ListItemNode::qinticOut(float t) {
        assert(t >= 0 && t <= 1.0);
        return 1.0 - (pow(t - 1.0, 5.0));
    }

    float ListItemNode::qinticInOut(float t) {
        assert(t >= 0 && t <= 1.0);
        return t < 0.5 ? +16.0 * pow(t, 5.0)
                       : -0.5 * pow(2.0 * t - 2.0, 5.0) + 1.0;
    }

    float ListItemNode::quarticIn(float t) {
        assert(t >= 0 && t <= 1.0);
        return pow(t, 4.0);
    }

    float ListItemNode::quarticOut(float t) {
        assert(t >= 0 && t <= 1.0);
        return pow(t - 1.0, 3.0) * (1.0 - t) + 1.0;
    }

    float ListItemNode::quarticInOut(float t) {
        assert(t >= 0 && t <= 1.0);
        return t < 0.5 ? +8.0 * pow(t, 4.0) : -8.0 * pow(t - 1.0, 4.0) + 1.0;
    }

    float ListItemNode::quadraticInOut(float t) {
        assert(t >= 0 && t <= 1.0);
        float p = 2.0 * t * t;
        return t < 0.5 ? p : -p + (4.0 * t) - 1.0;
    }

    float ListItemNode::quadraticIn(float t) {
        assert(t >= 0 && t <= 1.0);
        return t * t;
    }

    float ListItemNode::quadraticOut(float t) {
        assert(t >= 0 && t <= 1.0);
        return -t * (t - 2.0);
    }

    float ListItemNode::cubicIn(float t) {
        assert(t >= 0 && t <= 1.0);
        return t * t * t;
    }

    float ListItemNode::cubicOut(float t) {
        assert(t >= 0 && t <= 1.0);
        float f = t - 1.0;
        return f * f * f + 1.0;
    }

    float ListItemNode::cubicInOut(float t) {
        assert(t >= 0 && t <= 1.0);
        return t < 0.5 ? 4.0 * t * t * t : 0.5 * pow(2.0 * t - 2.0, 3.0) + 1.0;
    }

    float ListItemNode::elasticIn(float t) {
        assert(t >= 0 && t <= 1.0);
        return sin(13.0 * t * M_PI_2) * pow(2.0, 10.0 * (t - 1.0));
    }

    float ListItemNode::elasticOut(float t) {
        assert(t >= 0 && t <= 1.0);
        return sin(-13.0 * (t + 1.0) * M_PI_2) * pow(2.0, -10.0 * t) + 1.0;
    }

    float ListItemNode::elasticInOut(float t) {
        assert(t >= 0 && t <= 1.0);
        return t < 0.5 ? 0.5 * sin(+13.0 * M_PI_2 * 2.0 * t) *
                             pow(2.0, 10.0 * (2.0 * t - 1.0))
                       : 0.5 * sin(-13.0 * M_PI_2 * ((2.0 * t - 1.0) + 1.0)) *
                                 pow(2.0, -10.0 * (2.0 * t - 1.0)) +
                             1.0;
    }

    float ListItemNode::circularIn(float t) {
        assert(t >= 0 && t <= 1.0);
        return 1.0 - sqrt(1.0 - t * t);
    }

    float ListItemNode::circularOut(float t) {
        assert(t >= 0 && t <= 1.0);
        return sqrt((2.0 - t) * t);
    }

    float ListItemNode::circularInOut(float t) {
        assert(t >= 0 && t <= 1.0);
        return t < 0.5 ? 0.5 * (1.0 - sqrt(1.0 - 4.0 * t * t))
                       : 0.5 * (sqrt((3.0 - 2.0 * t) * (2.0 * t - 1.0)) + 1.0);
    }

    float ListItemNode::bounceOut(float t) {
        assert(t >= 0 && t <= 1.0);
        const float a = 4.0 / 11.0;
        const float b = 8.0 / 11.0;
        const float c = 9.0 / 10.0;

        const float ca = 4356.0 / 361.0;
        const float cb = 35442.0 / 1805.0;
        const float cc = 16061.0 / 1805.0;

        float t2 = t * t;

        return t < a ? 7.5625 * t2
                     : t < b ? 9.075 * t2 - 9.9 * t + 3.4
                             : t < c ? ca * t2 - cb * t + cc
                                     : 10.8 * t * t - 20.52 * t + 10.72;
    }

    float ListItemNode::bounceIn(float t) {
        assert(t >= 0 && t <= 1.0);
        return 1.0 - bounceOut(1.0 - t);
    }

    float ListItemNode::bounceInOut(float t) {
        assert(t >= 0 && t <= 1.0);
        return t < 0.5 ? 0.5 * (1.0 - bounceOut(1.0 - t * 2.0))
                       : 0.5 * bounceOut(t * 2.0 - 1.0) + 0.5;
    }

    float ListItemNode::backIn(float t) {
        assert(t >= 0 && t <= 1.0);
        return pow(t, 3.0) - t * sin(t * M_PI);
    }

    float ListItemNode::backOut(float t) {
        assert(t >= 0 && t <= 1.0);
        float f = 1.0 - t;
        return 1.0 - (pow(f, 3.0) - f * sin(f * M_PI));
    }

    float ListItemNode::backInOut(float t) {
        assert(t >= 0 && t <= 1.0);
        float f = t < 0.5 ? 2.0 * t : 1.0 - (2.0 * t - 1.0);

        float g = pow(f, 3.0) - f * sin(f * M_PI);

        return t < 0.5 ? 0.5 * g : 0.5 * (1.0 - g) + 0.5;
    }
} // namespace NJLIC
