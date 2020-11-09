//
//  MeshGeometry.h
//  Spaulding Medical Test
//
//  Created by James Folk on 11/6/20.
//  Copyright © 2020 NJLIGames Ltd. All rights reserved.
//

#ifndef MeshGeometry_hpp
#define MeshGeometry_hpp

#include "Geometry.h"

namespace NJLIC {
    class MeshGeometry : public Geometry {
      public:
        /* members */
        MeshGeometry();
        MeshGeometry(const MeshGeometry &rhs);
        const MeshGeometry &operator=(const MeshGeometry &rhs);
        ~MeshGeometry();

        virtual void load(Shader *shader, const std::string &filecontent = "",
                          unsigned int numInstances = 1);

        glm::vec3 getVertexPosition(const GLsizei instanceIdx,
                                    const GLsizei verticeIdx) const;
        glm::vec4 getVertexColor(const GLsizei instanceIdx,
                                 const GLsizei verticeIdx) const;
        glm::vec2 getVertexTexture(const GLsizei instanceIdx,
                                   const GLsizei verticeIdx) const;
        glm::vec3 getVertexNormal(const GLsizei instanceIdx,
                                  const GLsizei verticeIdx) const;
        glm::vec3 getVertexTangent(const GLsizei instanceIdx,
                                   const GLsizei verticeIdx) const;
        glm::vec3 getVertexBitangent(const GLsizei instanceIdx,
                                     const GLsizei verticeIdx) const;

        void setVertexPosition(const glm::vec3 &v, const GLsizei instanceIdx,
                               const GLsizei verticeIdx);
        void setVertexColor(const glm::vec4 &v, const GLsizei instanceIdx,
                            const GLsizei verticeIdx);
        void setVertexTexture(const glm::vec2 &v, const GLsizei instanceIdx,
                              const GLsizei verticeIdx);
        void setVertexNormal(const glm::vec3 &v, const GLsizei instanceIdx,
                             const GLsizei verticeIdx);
        void setVertexTangent(const glm::vec3 &v, const GLsizei instanceIdx,
                              const GLsizei verticeIdx);
        void setVertexBitangent(const glm::vec3 &v, const GLsizei instanceIdx,
                                const GLsizei verticeIdx);

        virtual TexturedColoredVertex getVertex(const GLsizei instanceIdx,
                                                const GLsizei verticeIdx) const;
        virtual void setVertex(const TexturedColoredVertex &tcv,
                               const GLsizei instanceIdx,
                               const GLsizei verticeIdx);

      protected:
        virtual void loadData();
        virtual void unLoadData();

        virtual const void *getVertexArrayBufferPtr() const;
        virtual GLsizeiptr getVertexArrayBufferSize() const;

        virtual const void *getElementArrayBufferPtr() const;
        virtual GLsizeiptr getElementArrayBufferSize() const;

        virtual GLenum getElementIndexType() const;

        virtual void setOpacity(Node *node);
        virtual void setHidden(Node *node);
        virtual void setColorBase(Node *node);

        virtual GLsizei numberOfVertices() const;
        virtual GLsizei numberOfIndices() const;

        GLsizei m_NumberOfVertices;
        GLsizei m_NumberOfIndices;
        TexturedColoredVertex *m_VertexData;
        GLuint *m_IndiceData;
      private:
        

        std::string m_Filedata;
        

        TexturedColoredVertex *m_triangleBuffer;
    };
} // namespace NJLIC

#endif /* MeshGeometry_hpp */
