
#include "GraphPlot.h"

#include "BitmapFont.h"
#include "GraphGeometry.h"
#include "ListItemNode.h"
#include "Node.h"

namespace NJLIC {
    GraphPlot::GraphPlot()
        : mPlot(nullptr), mGraphNode(nullptr), mIndicatorNode(nullptr),
          mTitleNode(nullptr) {}

    GraphPlot::~GraphPlot() { unInit(); }

    void GraphPlot::init(NJLIC::Shader *shader, NJLIC::Scene *scene,
                         const std::vector<short> &values, const size_t &index,
                         NJLIC::MaterialProperty *materialPropertyGraph,
                         NJLIC::MaterialProperty *materialPropertyIndicator) {

        mGraphGeometry = new NJLIC::GraphGeometry();

        mGraphGeometry->load(shader, values, values.size(), 1);

        mIndicatorGeometry = new NJLIC::GraphGeometry();

        std::vector<short> indicator;

        for (int i = 0; i < 50; i++) {
            indicator.push_back(0);
        }
        for (int i = 0; i < 100; i++) {
            indicator.push_back(300);
        }
        for (int i = 0; i < 50; i++) {
            indicator.push_back(0);
        }
        mIndicatorGeometry->load(shader, indicator, indicator.size(), 1);

        float scale(.000237);
        float y_decrement(scale * 800);

        unInit();

        float graphYOffset(0.07f);

        mPlot = new NJLIC::ListItemNode();
        mPlot->setOrigin(glm::vec3(0.1, 2.2 - (y_decrement * index), 9.0));

        mGraphNode = new NJLIC::Node();

        //        mGraphGeometry->loadDiffuseMatrial(shader,
        //        "assets/graph_line.jpg");
        mGraphGeometry->setDiffuseMaterial(materialPropertyGraph);
        mGraphNode->addGeometry(mGraphGeometry);
        scene->addActiveNode(mGraphNode);
        mGraphNode->setOrigin(glm::vec3(0.0, graphYOffset, 0.02));

        mPlot->addChildNode(mGraphNode);

        mIndicatorNode = new NJLIC::Node();

        //        mIndicatorGeometry->loadDiffuseMatrial(shader,
        //        "assets/indicator_line.jpg");
        mIndicatorGeometry->setDiffuseMaterial(materialPropertyIndicator);
        mIndicatorNode->addGeometry(mIndicatorGeometry);
        scene->addActiveNode(mIndicatorNode);
        mIndicatorNode->setOrigin(glm::vec3(0.0, graphYOffset, 0.01));
        mPlot->addChildNode(mIndicatorNode);

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
        mTitleNode->setOrigin(glm::vec3(0.0, 0.0, 0.0));
        mPlot->addChildNode(mTitleNode);

        scene->getRootNode()->addChildNode(mPlot);
    }

    void GraphPlot::unInit() {
        if (nullptr != mTitleNode)
            delete mTitleNode;
        if (nullptr != mIndicatorNode)
            delete mIndicatorNode;
        if (nullptr != mGraphNode)
            delete mGraphNode;
        if (nullptr != mPlot)
            delete mPlot;
    }

    void GraphPlot::update(float steps) { mPlot->update(steps); }

    void GraphPlot::scrollRight() { mPlot->scrollPrevious(1.f, 15); }

    void GraphPlot::scrollLeft() { mPlot->scrollNext(1.f, 15); }
} // namespace NJLIC
