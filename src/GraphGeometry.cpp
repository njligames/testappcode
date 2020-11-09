//
//  GraphGeometry.cpp
//  Spaulding Medical Test
//
//  Created by James Folk on 11/6/20.
//  Copyright © 2020 NJLIGames Ltd. All rights reserved.
//

#include "GraphGeometry.h"

namespace NJLIC {

    GraphGeometry::GraphGeometry() {
        
    }

    GraphGeometry::~GraphGeometry() {
        
    }

    void GraphGeometry::load(Shader *shader, const std::vector<short> &values, unsigned int numInstances) {
        
        std::copy(values.begin(), values.end(), std::back_inserter(mValues));
        MeshGeometry::load(shader, "", numInstances);
        setDrawMode(GL_LINE_STRIP);
    }

    void GraphGeometry::loadData() {
        auto size = mValues.size();
        
        m_NumberOfIndices = (GLsizei)size;
        m_NumberOfVertices = (GLsizei)size;
        
        struct TCV
        {
            TCV(const size_t &s) : scale(.000237){
                x = 0;
                idx = 0;
                size = s;
                
                vertexData = new TexturedColoredVertex[size];
                indiceData = new GLuint[size];
            }
            
            void operator()(short n) {
                if(idx < size) {

                    TexturedColoredVertex t;
                    t.vertex = glm::vec3(x*scale, n*scale, 0);
                    t.texture = glm::vec2(1.0f, 1.0f);
                    t.normal = glm::vec3(0.0f, 0.0f, -1.0f);
                    t.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                    
                    vertexData[idx] = t;
                    indiceData[idx] = idx;
                    
                    x++;
                    idx++;
                }
            }
            
            float x;
            size_t idx;
            float scale;
            size_t size;
            
            TexturedColoredVertex *vertexData;
            GLuint *indiceData;
        };
        
        TCV tcv = std::for_each(mValues.begin(), mValues.end(), TCV(size));
        
        Geometry::loadData();

        assert(m_VertexData == NULL);
        m_VertexData =
            new TexturedColoredVertex[numberOfVertices() * numberOfInstances()];
        enableVertexArrayBufferChanged(true);

        assert(m_IndiceData == NULL);
        m_IndiceData = new GLuint[numberOfIndices() * numberOfInstances()];

        unsigned long vertexInstanceIndex = 0;
        unsigned long indiceInstanceIndex = 0;
        for (unsigned long meshIndex = 0; meshIndex < numberOfInstances();
             meshIndex++) {
            for (unsigned long verticeIndex = 0;
                 verticeIndex < numberOfVertices(); verticeIndex++) {
                m_VertexData[vertexInstanceIndex] = tcv.vertexData[verticeIndex];
                vertexInstanceIndex++;
            }

            for (unsigned long indiceIndex = 0; indiceIndex < numberOfIndices();
                 indiceIndex++) {
                m_IndiceData[indiceInstanceIndex] = (GLuint)(
                                                             (meshIndex * numberOfVertices()) + tcv.indiceData[indiceIndex]);
                indiceInstanceIndex++;
            }
        }

        TexturedColoredVertex::computeTangentBasis(m_VertexData,
                                                   numberOfVertices());

        delete[] tcv.indiceData;
        delete[] tcv.vertexData;
    }
}

