//
//  GraphGeometry.h
//  Spaulding Medical Test
//
//  Created by James Folk on 11/6/20.
//  Copyright © 2020 NJLIGames Ltd. All rights reserved.
//

#ifndef GraphGeometry_h
#define GraphGeometry_h

#include "MeshGeometry.h"

namespace NJLIC {
    class GraphGeometry : public MeshGeometry {
        
        GraphGeometry(const GraphGeometry &) = delete;
        const GraphGeometry &operator=(const GraphGeometry &) = delete;
        
        std::vector<short> mValues;

      public:
        GraphGeometry();
        virtual ~GraphGeometry();

        virtual void load(Shader *shader, const std::vector<short> &values, unsigned int numInstances = 1);

      protected:
        
        virtual void loadData()override;

        virtual void load(Shader *shader, const std::string &filecontent,
                          unsigned int numInstances = 1) override {
            MeshGeometry::load(shader, filecontent, numInstances);
        }
    };
} // namespace NJLIC

#endif /* GraphGeometry_hpp */
