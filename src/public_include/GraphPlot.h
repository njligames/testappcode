//
//  GraphPlot.h
//  Spaulding Medical Test
//
//  Created by James Folk on 11/6/20.
//  Copyright © 2020 NJLIGames Ltd. All rights reserved.
//

#ifndef GraphPlot_h
#define GraphPlot_h

#include "MeshGeometry.h"

namespace NJLIC {
class Shader;
class Scene;
class GraphGeometry;
class ListItemNode;

    class GraphPlot : public MeshGeometry {
        
        GraphPlot(const GraphPlot &) = delete;
        const GraphPlot &operator=(const GraphPlot &) = delete;
        
        NJLIC::Node *mPlot;
        NJLIC::Node *mGraphNode;
        NJLIC::Node *mIndicatorNode;
        NJLIC::Node *mTitleNode;
        
        NJLIC::GraphGeometry *mGraphGeometry;
        NJLIC::GraphGeometry *mIndicatorGeometry;

      public:
        GraphPlot();
        virtual ~GraphPlot();
        
        NJLIC::Node * init(NJLIC::Shader *shader, NJLIC::Scene *scene, const std::vector<short> &values, const size_t &index, NJLIC::MaterialProperty *materialPropertyGraph, NJLIC::MaterialProperty *materialPropertyIndicator);
        void unInit();
        
        void update(float steps);
        
        void scrollRight();
        void scrollLeft();

      protected:
    };
} // namespace NJLIC

#endif /* GraphPlot_hpp */
