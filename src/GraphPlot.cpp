
#include "GraphPlot.h"

#include "BitmapFont.h"
#include "GraphGeometry.h"
#include "ListItemNode.h"
#include "Node.h"

namespace NJLIC {
    GraphPlot::GraphPlot()
        : mPlot(nullptr), mGraphNode(nullptr), mIndicatorNode(nullptr),
          mTitleNode(nullptr), mGraphGeometry(nullptr),
          mIndicatorGeometry(nullptr) {}

    GraphPlot::~GraphPlot() { unInit(); }

    NJLIC::Node *
    GraphPlot::init(NJLIC::Shader *shader, NJLIC::Scene *scene,
                    const std::vector<short> &values, const size_t &index,
                    NJLIC::MaterialProperty *materialPropertyGraph,
                    NJLIC::MaterialProperty *materialPropertyIndicator) {

        float scale(Geometry::sScale3DTo2D);
        float y_decrement(scale * 800);

        unInit();

        float graphYOffset(0.07f);

        mPlot = new NJLIC::Node();
        mPlot->setOrigin(glm::vec3(0.1, 2.2 - (y_decrement * index), 9.0));

        {
            mGraphGeometry = new NJLIC::GraphGeometry();
            mGraphGeometry->load(shader, values, values.size(), 1);

            mGraphNode = new NJLIC::Node();
            mGraphGeometry->setDiffuseMaterial(materialPropertyGraph);
            mGraphNode->addGeometry(mGraphGeometry);
            scene->addActiveNode(mGraphNode);
            mGraphNode->setOrigin(glm::vec3(0.0, graphYOffset, 0.0));
            mPlot->addChildNode(mGraphNode);
        }

        {
            std::vector<short> indicator(100);
            fill_n(indicator.begin() + 1, 100 - 2, 300);

            mIndicatorGeometry = new NJLIC::GraphGeometry();
            mIndicatorGeometry->load(shader, indicator, indicator.size(), 1);
            mIndicatorNode = new NJLIC::Node();
            mIndicatorGeometry->setDiffuseMaterial(materialPropertyIndicator);
            mIndicatorNode->addGeometry(mIndicatorGeometry);
            scene->addActiveNode(mIndicatorNode);
            mIndicatorNode->setOrigin(glm::vec3(
                0.1, 2.2 - (y_decrement * index) + graphYOffset, 9.0));
        }

        std::string title;
        switch (index) {
        case 0:
            title = "I";
            break;
        case 1:
            title = "II";
            break;
        case 2:
            title = "III";
            break;
        case 3:
            title = "aVR";
            break;
        case 4:
            title = "aVL";
            break;
        case 5:
            title = "aVF";
            break;
        case 6:
            title = "V1";
            break;
        case 7:
            title = "V2";
            break;
        case 8:
            title = "V3";
            break;
        case 9:
            title = "V4";
            break;
        case 10:
            title = "V5";
            break;
        case 11:
            title = "V6";
            break;
        default:
            title = "<PLACEHOLDER>";
            break;
        }
        mTitleNode =
            BitmapFont::getInstance()->printf(scene, "%s", title.c_str());
        scene->addActiveNode(mTitleNode);
        mTitleNode->setOrigin(glm::vec3(0.1, 2.2 - (y_decrement * index), 9.0));
        mPlot->addChildNode(mTitleNode);

        scene->getRootNode()->addChildNode(mIndicatorNode);
        scene->getRootNode()->addChildNode(mTitleNode);

        return mPlot;
    }

    void GraphPlot::unInit() {
        if (nullptr != mGraphGeometry)
            delete mGraphGeometry;
        if (nullptr != mIndicatorGeometry)
            delete mIndicatorGeometry;
        if (nullptr != mTitleNode)
            delete mTitleNode;
        if (nullptr != mIndicatorNode)
            delete mIndicatorNode;
        if (nullptr != mGraphNode)
            delete mGraphNode;
        if (nullptr != mPlot)
            delete mPlot;
    }

    void GraphPlot::update(float steps) {}

    void GraphPlot::scrollRight() {}

    void GraphPlot::scrollLeft() {}
} // namespace NJLIC
