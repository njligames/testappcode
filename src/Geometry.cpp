//
//  Geometry.cpp
//  Spaulding Medical Test
//
//  Created by James Folk on 11/6/20.
//  Copyright © 2020 NJLIGames Ltd. All rights reserved.
//

#include "Geometry.h"

#include <assert.h>
#include <iostream>

#include "Camera.h"
#include "Node.h"
#include "Shader.h"
#include "Util.h"
// https://en.wikipedia.org/wiki/Wavefront_.obj_file#Material_template_library

namespace NJLIC {

    const unsigned int HALF_FLOAT_MIN_BIASED_EXP_AS_SINGLE_FP_EXP = 0x38000000;
    const unsigned int HALF_FLOAT_MAX_BIASED_EXP_AS_SINGLE_FP_EXP = 0x47800000;
    const unsigned int FLOAT_MAX_BIASED_EXP = (0xFF << 23);
    const unsigned int HALF_FLOAT_MAX_BIASED_EXP = (0x1F << 10);

    static hfloat convertFloatToHFloat(float *f) {
        unsigned int x = *(unsigned int *)f;
        unsigned int sign = (unsigned short)(x >> 31);
        unsigned int mantissa;
        unsigned int exp;
        hfloat hf;

        // get mantissa
        mantissa = x & ((1 << 23) - 1);
        // get exponent bits
        exp = x & FLOAT_MAX_BIASED_EXP;
        if (exp >= HALF_FLOAT_MAX_BIASED_EXP_AS_SINGLE_FP_EXP) {
            // check if the original single precision float number is a NaN
            if (mantissa && (exp == FLOAT_MAX_BIASED_EXP)) {
                // we have  a single precision NaN
                mantissa = (1 << 23) - 1;
            } else {
                // 16-bit half-float representation stores number as Inf;
                mantissa = 0;
            }
            hf = (((hfloat)sign) << 15) | (hfloat)(HALF_FLOAT_MAX_BIASED_EXP) |
                 (hfloat)(mantissa >> 13);
        }
        // check if exponent is <= -15
        else if (exp <= HALF_FLOAT_MIN_BIASED_EXP_AS_SINGLE_FP_EXP) {
            // store a denorm half-float value or zero
            exp = (HALF_FLOAT_MIN_BIASED_EXP_AS_SINGLE_FP_EXP - exp) >> 23;
            mantissa >>= (14 + exp);

            hf = (((hfloat)sign) << 15) | (hfloat)(mantissa);
        } else {
            hf = (((hfloat)sign) << 15) |
                 (hfloat)((exp - HALF_FLOAT_MIN_BIASED_EXP_AS_SINGLE_FP_EXP) >>
                          13) |
                 (hfloat)(mantissa >> 13);
        }
        return hf;
    }

    static float convertHFloatToFloat(hfloat hf) {
        unsigned int sign = (unsigned int)(hf >> 15);
        unsigned int mantissa = (unsigned int)(hf & ((1 << 10) - 1));
        unsigned int exp = (unsigned int)(hf & HALF_FLOAT_MAX_BIASED_EXP);
        unsigned int f;

        if (exp == HALF_FLOAT_MAX_BIASED_EXP) {
            /*
             we have a half-float NaN or Inf
             half-float NaNas will be converted to a single precision NaN
             half-float Infs will be converted to a single precision Inf
             */
            exp = FLOAT_MAX_BIASED_EXP;
            if (mantissa)
                mantissa = (1 << 23) - 1;
        } else if (exp == 0x0) {
            // convert half-float zero/denorm to single precision value
            if (mantissa) {
                mantissa <<= 1;
                exp = HALF_FLOAT_MIN_BIASED_EXP_AS_SINGLE_FP_EXP;
                // check for leading 1 in denorm mantissa
                while ((mantissa & (1 << 10)) == 0) {
                    /*
                     for every leading 0, decrement single precision exponent by
                     1 and shift half-float mantissa value to the left
                     */
                    mantissa <<= 1;
                    exp -= (1 << 23);
                }
                // clamp the mantissa to 10-bits
                mantissa &= ((1 << 10) - 1);
                // shift left to generate single-precision mantissa of 23-bits
                mantissa <<= 13;
            }
        } else {
            // shift left to generate single-precision mantissa of 23-bits
            mantissa <<= 13;
            // generate single precision biased exponent value
            exp = (exp << 13) + HALF_FLOAT_MIN_BIASED_EXP_AS_SINGLE_FP_EXP;
        }
        f = (sign << 31) | exp | mantissa;
        return *((float *)&f);
    }

    static const GLfptype TRANSFORM_IDENTITY_MATRIX[] = {
        1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
    };

    static const GLfptype COLOR_IDENTITY_MATRIX[] = {
        1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    };

    const float Geometry::sScale3DTo2D = 0.000237f;

    Geometry::Geometry()
        : m_MatrixBuffer(new GLfptype[16]),
          m_MatrixBufferFullSize(new float[16]),
          m_ModelViewTransformData(nullptr),
          m_NormalMatrixTransformData(nullptr), m_VertexArray(0),
          m_ModelViewBuffer(0), m_NormalMatrixTransformBuffer(0),
          m_VerticesBuffer(0), m_IndexBuffer(0), m_NumberInstances(1),
          m_Shader(nullptr), m_OpacityModifyRGB(false),
          m_VertexBufferChanged(true), m_NormalMatrixBufferChanged(true),
          m_ModelViewBufferChanged(true), m_ShaderChanged(true),
          mMaterialProperty(nullptr), mFormat(GL_RGBA),
          mDrawMode(GL_TRIANGLES) {
        assert(m_MatrixBuffer);
        assert(m_MatrixBufferFullSize);

        m_References.resize(m_NumberInstances);
    }

    Geometry::~Geometry() {
        if (m_NormalMatrixTransformData)
            delete[] m_NormalMatrixTransformData;
        m_NormalMatrixTransformData = nullptr;

        if (m_ModelViewTransformData)
            delete[] m_ModelViewTransformData;
        m_ModelViewTransformData = nullptr;

        delete[] m_MatrixBufferFullSize;
        m_MatrixBufferFullSize = nullptr;

        delete[] m_MatrixBuffer;
        m_MatrixBuffer = nullptr;
    }

    void Geometry::load(Shader *shader, const std::string &filecontent,
                        unsigned int numInstances) {
        assert(shader);

        setShader(shader);

        m_NumberInstances = numInstances;
        m_References.resize(m_NumberInstances);

        loadData();

        assert(m_VertexArray == 0);
        glGenVertexArrays_NJLIC(1, &m_VertexArray);
        glBindVertexArray_NJLIC(m_VertexArray);
        {
            {
                assert(m_ModelViewBuffer == 0);
                glGenBuffers(1, &m_ModelViewBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, m_ModelViewBuffer);
                glBufferData(
                    GL_ARRAY_BUFFER, getModelViewTransformArrayBufferSize(),
                    getModelViewTransformArrayBufferPtr(), GL_STREAM_DRAW);
                int inTransformAttrib =
                    getShader()->getAttributeLocation("inTransform");
                glEnableVertexAttribArray(inTransformAttrib + 0);
                glEnableVertexAttribArray(inTransformAttrib + 1);
                glEnableVertexAttribArray(inTransformAttrib + 2);
                glEnableVertexAttribArray(inTransformAttrib + 3);
                glVertexAttribPointer(inTransformAttrib + 0, 4, GL_INDEX_TYPE,
                                      0, sizeof(GLfptype) * 16, (GLvoid *)0);
                glVertexAttribPointer(inTransformAttrib + 1, 4, GL_INDEX_TYPE,
                                      0, sizeof(GLfptype) * 16, (GLvoid *)16);
                glVertexAttribPointer(inTransformAttrib + 2, 4, GL_INDEX_TYPE,
                                      0, sizeof(GLfptype) * 16, (GLvoid *)32);
                glVertexAttribPointer(inTransformAttrib + 3, 4, GL_INDEX_TYPE,
                                      0, sizeof(GLfptype) * 16, (GLvoid *)48);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }

            {
                assert(m_NormalMatrixTransformBuffer == 0);
                glGenBuffers(1, &m_NormalMatrixTransformBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, m_NormalMatrixTransformBuffer);
                glBufferData(
                    GL_ARRAY_BUFFER, getNormalMatrixTransformArrayBufferSize(),
                    getNormalMatrixTransformArrayBufferPtr(), GL_STREAM_DRAW);
                int inNormalMatrixAttrib =
                    getShader()->getAttributeLocation("inNormalMatrix");
                glEnableVertexAttribArray(inNormalMatrixAttrib + 0);
                glEnableVertexAttribArray(inNormalMatrixAttrib + 1);
                glEnableVertexAttribArray(inNormalMatrixAttrib + 2);
                glEnableVertexAttribArray(inNormalMatrixAttrib + 3);
                glVertexAttribPointer(inNormalMatrixAttrib + 0, 4,
                                      GL_INDEX_TYPE, 0, sizeof(GLfptype) * 16,
                                      (GLvoid *)0);
                glVertexAttribPointer(inNormalMatrixAttrib + 1, 4,
                                      GL_INDEX_TYPE, 0, sizeof(GLfptype) * 16,
                                      (GLvoid *)16);
                glVertexAttribPointer(inNormalMatrixAttrib + 2, 4,
                                      GL_INDEX_TYPE, 0, sizeof(GLfptype) * 16,
                                      (GLvoid *)32);
                glVertexAttribPointer(inNormalMatrixAttrib + 3, 4,
                                      GL_INDEX_TYPE, 0, sizeof(GLfptype) * 16,
                                      (GLvoid *)48);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }
            {
                assert(m_VerticesBuffer == 0);
                glGenBuffers(1, &m_VerticesBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, m_VerticesBuffer);
                glBufferData(GL_ARRAY_BUFFER, getVertexArrayBufferSize(),
                             getVertexArrayBufferPtr(), GL_STREAM_DRAW);
                int inPositionAttrib =
                    getShader()->getAttributeLocation("inPosition");
                int inColorAttrib =
                    getShader()->getAttributeLocation("inColor");
                int inNormalAttrib =
                    getShader()->getAttributeLocation("inNormal");

                int inTexCoordAttrib =
                    getShader()->getAttributeLocation("inTexCoord");

                int inTangentAttrib =
                    getShader()->getAttributeLocation("inTangent");
                int inBiTangentAttrib =
                    getShader()->getAttributeLocation("inBiTangent");

                glEnableVertexAttribArray(inPositionAttrib);
                glVertexAttribPointer(
                    inPositionAttrib, 3, GL_INDEX_TYPE, GL_FALSE,
                    sizeof(TexturedColoredVertex),
                    (const GLvoid *)offsetof(TexturedColoredVertex, vertex));

                glEnableVertexAttribArray(inTexCoordAttrib);
                glVertexAttribPointer(
                    inTexCoordAttrib, 2, GL_INDEX_TYPE, GL_FALSE,
                    sizeof(TexturedColoredVertex),
                    (const GLvoid *)offsetof(TexturedColoredVertex, texture));

                glEnableVertexAttribArray(inNormalAttrib);
                glVertexAttribPointer(
                    inNormalAttrib, 3, GL_INDEX_TYPE, GL_FALSE,
                    sizeof(TexturedColoredVertex),
                    (const GLvoid *)offsetof(TexturedColoredVertex, normal));

                glEnableVertexAttribArray(inColorAttrib);
                glVertexAttribPointer(
                    inColorAttrib, 4, GL_INDEX_TYPE, GL_FALSE,
                    sizeof(TexturedColoredVertex),
                    (const GLvoid *)offsetof(TexturedColoredVertex, color));

                glEnableVertexAttribArray(inTangentAttrib);
                glVertexAttribPointer(
                    inTangentAttrib, 3, GL_INDEX_TYPE, GL_FALSE,
                    sizeof(TexturedColoredVertex),
                    (const GLvoid *)offsetof(TexturedColoredVertex, tangent));

                glEnableVertexAttribArray(inBiTangentAttrib);
                glVertexAttribPointer(
                    inBiTangentAttrib, 3, GL_INDEX_TYPE, GL_FALSE,
                    sizeof(TexturedColoredVertex),
                    (const GLvoid *)offsetof(TexturedColoredVertex, bitangent));

                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }

            {
                glGenBuffers(1, &m_IndexBuffer);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                             getElementArrayBufferSize(),
                             getElementArrayBufferPtr(), GL_STATIC_DRAW);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }
        }
        glBindVertexArray_NJLIC(0);
    }

    void Geometry::unLoad() {
        if (m_IndexBuffer)
            glDeleteBuffers(1, &m_IndexBuffer);
        m_IndexBuffer = 0;

        if (m_VerticesBuffer)
            glDeleteBuffers(1, &m_VerticesBuffer);
        m_VerticesBuffer = 0;

        if (m_NormalMatrixTransformBuffer)
            glDeleteBuffers(1, &m_NormalMatrixTransformBuffer);
        m_NormalMatrixTransformBuffer = 0;

        if (m_ModelViewBuffer)
            glDeleteBuffers(1, &m_ModelViewBuffer);
        m_ModelViewBuffer = 0;

        if (m_VertexArray)
            glDeleteVertexArrays_NJLIC(1, &m_VertexArray);
        m_VertexArray = 0;
    }

    bool Geometry::isLoaded() const { return false; }

    Shader *const Geometry::getShader() { return m_Shader; }

    void Geometry::setShader(Shader *const shader) {
        m_Shader = shader;
        m_ShaderChanged = true;
    }

    void Geometry::render(Camera *camera) {
        Shader *shader = getShader();
        if (shader && camera) {
            shader->use();

            if (mMaterialProperty) {
                mMaterialProperty->render();
                shader->setUniformValue("tDiffuseColor",
                                        mMaterialProperty->getTextureIndex());
            }

            camera->render(shader, m_ShaderChanged);

            struct LightSourceParameters {
                glm::vec4 ambient;
                glm::vec4 diffuse;
                glm::vec4 specular;
                glm::vec4 position;
                glm::vec4 halfVector;
                glm::vec3 spotDirection;
                float spotExponent;
                float spotCutoff;
                float spotCosCutoff;
                float constantAttenuation;
                float linearAttenuation;
                float quadraticAttenuation;
            };

            struct MaterialParameters {
                glm::vec4 emission;
                glm::vec4 ambient;
                glm::vec4 diffuse;
                glm::vec4 specular;
                float shininess;
            };

            shader->setUniformValue("RimLightColor",
                                    glm::vec3(1.0f, 1.0f, 1.0f));
            shader->setUniformValue("RimLightStart", 0.0f);
            shader->setUniformValue("RimLightEnd", 1.0f);
            shader->setUniformValue("RimLightCoefficient", 0.6f);

            shader->setUniformValue("LightSourceAmbientColor",
                                    glm::vec3(1.0f, 1.0f, 1.0f));
            shader->setUniformValue("LightSourceDiffuseColor",
                                    glm::vec3(1.0f, 1.0f, 1.0f));
            shader->setUniformValue("LightSourceSpecularColor",
                                    glm::vec3(1.0f, 1.0f, 1.0f));

            shader->setUniformValue("LightSourcePosition_worldspace",
                                    glm::vec4(-3.0f, -1.5f, -1.0f, 1.0));

            shader->setUniformValue("LightSourceSpotDirection",
                                    glm::vec3(0.0f, 0.0f, 1.0f));
            shader->setUniformValue("LightSourceSpotExponent", 100.0f);
            // set LightSourceSpotCutoff != 180 for SpotLight
            shader->setUniformValue("LightSourceSpotCutoff", 180.0f);
            shader->setUniformValue("LightSourceSpotCosCutoff", 30.0f);
            // http://www.ogre3d.org/tikiwiki/-Point+Light+Attenuation

            shader->setUniformValue("LightSourceConstantAttenuation", 1.0f);
            shader->setUniformValue("LightSourceLinearAttenuation", 0.045f);
            shader->setUniformValue("LightSourceQuadraticAttenuation", 0.0075f);

            shader->setUniformValue("LightAmbientColor",
                                    glm::vec3(1.0f, 1.0f, 1.0f));

            shader->setUniformValue("MaterialShininess", 1000.0f);

            shader->setUniformValue("FogMaxDistance", 10.0f);
            shader->setUniformValue("FogMinDistance", 0.1f);
            shader->setUniformValue("FogColor", glm::vec3(0.7f, 0.7f, 0.7f));
            shader->setUniformValue("FogDensity", 0.000000001f);

            m_ShaderChanged = false;

            glBindVertexArray_NJLIC(m_VertexArray);

            if (isModelViewBufferChanged()) {
                glBindBuffer(GL_ARRAY_BUFFER, m_ModelViewBuffer);
                glBufferSubData(GL_ARRAY_BUFFER, 0,
                                getModelViewTransformArrayBufferSize(),
                                getModelViewTransformArrayBufferPtr());
                enableModelViewBufferChanged(false);
            }

            if (isNormalMatrixBufferChanged()) {
                glBindBuffer(GL_ARRAY_BUFFER, m_NormalMatrixTransformBuffer);
                glBufferSubData(GL_ARRAY_BUFFER, 0,
                                getNormalMatrixTransformArrayBufferSize(),
                                getNormalMatrixTransformArrayBufferPtr());
                enableNormalMatrixBufferChanged(false);
            }

            if (isVertexArrayBufferChanged()) {
                glBindBuffer(GL_ARRAY_BUFFER, m_VerticesBuffer);
                glBufferSubData(GL_ARRAY_BUFFER, 0, getVertexArrayBufferSize(),
                                getVertexArrayBufferPtr());
                enableVertexArrayBufferChanged(false);
            }

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

            glDrawElements(mDrawMode, numberOfInstances() * numberOfIndices(),
                           getElementIndexType(), (const GLvoid *)0);
        }
    }

    GLsizei Geometry::numberOfInstances() const { return m_NumberInstances; }

    unsigned long Geometry::getGeometryIndex(Node *const node) const {
        return node->getGeometryIndex();
    }

    void Geometry::loadDiffuseMatrial(Shader *shader,
                                      const unsigned char *diffuseFileData,
                                      int width, int height,
                                      int channels_in_file) {

        if (nullptr != mMaterialProperty) {
            mMaterialProperty->load(shader, diffuseFileData, width, height,
                                    channels_in_file);
        }
    }

    bool Geometry::loadDiffuseMatrial(Shader *shader,
                                      const std::string &diffuseFile) {

        int width, height, num_components;

        unsigned char *buffer = (unsigned char *)Util::loadImage(
            diffuseFile, &width, &height, &num_components);

        if (nullptr != buffer) {

            Geometry::loadDiffuseMatrial(shader, buffer, width, height,
                                         num_components);

            free(buffer);
            return true;
        }
        return false;
    }

    void Geometry::reloadDiffuseMatrial(Shader *shader,
                                        const unsigned char *diffuseFileData,
                                        int width, int height,
                                        int channels_in_file) {

        if (nullptr != mMaterialProperty) {
            if (nullptr != diffuseFileData &&
                width == mMaterialProperty->getWidth() &&
                height == mMaterialProperty->getHeight()) {
                mMaterialProperty->reload(shader, diffuseFileData, width,
                                          height, channels_in_file);
            }
        }
    }

    void
    Geometry::setDiffuseMaterial(NJLIC::MaterialProperty *materialProperty) {
        mMaterialProperty = materialProperty;
    }

    const void *Geometry::getModelViewTransformArrayBufferPtr() const {
        assert(m_ModelViewTransformData);

        return m_ModelViewTransformData;
    }

    GLsizeiptr Geometry::getModelViewTransformArrayBufferSize() const {
        GLsizeiptr size =
            sizeof(GLfptype) * numberOfInstances() * numberOfVertices() * 16;
        return size;
    }

    bool Geometry::isModelViewBufferChanged() const {
        return m_ModelViewBufferChanged;
    }

    void Geometry::enableModelViewBufferChanged(bool changed) {
        m_ModelViewBufferChanged = changed;
    }

    const void *Geometry::getNormalMatrixTransformArrayBufferPtr() const {
        assert(m_NormalMatrixTransformData);

        return m_NormalMatrixTransformData;
    }

    GLsizeiptr Geometry::getNormalMatrixTransformArrayBufferSize() const {
        GLsizeiptr size =
            sizeof(GLfptype) * numberOfInstances() * numberOfVertices() * 16;
        return size;
    }

    bool Geometry::isNormalMatrixBufferChanged() const {
        return m_NormalMatrixBufferChanged;
    }

    void Geometry::enableNormalMatrixBufferChanged(bool changed) {
        m_NormalMatrixBufferChanged = changed;
    }

    void Geometry::loadData() {
        unLoadData();

        m_ModelViewTransformData =
            new GLfptype[numberOfInstances() * numberOfVertices() * 16];

        m_NormalMatrixTransformData =
            new GLfptype[numberOfInstances() * numberOfVertices() * 16];
        enableNormalMatrixBufferChanged(true);

        assert(m_ModelViewTransformData);
        //        assert(m_ColorTransformData);
        assert(m_NormalMatrixTransformData);

        unsigned long i;

        for (i = 0; i < (numberOfInstances() * numberOfVertices() * 16);
             i += 16)
            memcpy(m_ModelViewTransformData + i, TRANSFORM_IDENTITY_MATRIX,
                   sizeof(TRANSFORM_IDENTITY_MATRIX));

        for (i = 0; i < (numberOfInstances() * numberOfVertices() * 16);
             i += 16)
            memcpy(m_NormalMatrixTransformData + i, TRANSFORM_IDENTITY_MATRIX,
                   sizeof(TRANSFORM_IDENTITY_MATRIX));

        enableModelViewBufferChanged(true);
        enableNormalMatrixBufferChanged(true);
    }

    void Geometry::unLoadData() {
        if (m_NormalMatrixTransformData)
            delete[] m_NormalMatrixTransformData;
        m_NormalMatrixTransformData = nullptr;

        if (m_ModelViewTransformData)
            delete[] m_ModelViewTransformData;
        m_ModelViewTransformData = nullptr;
    }

    bool Geometry::isVertexArrayBufferChanged() const {
        return m_VertexBufferChanged;
    }

    void Geometry::enableVertexArrayBufferChanged(bool changed) {
        m_VertexBufferChanged = changed;
    }

    void Geometry::addReference(Node *node) {
        for (unsigned long i = 0; i < m_References.size(); ++i) {
            if (!m_References[i]) {
                m_References[i] = true;
                node->setGeometryIndex(i);
                return;
            }
        }
    }

    void Geometry::removeReference(Node *node) {
        unsigned long index = getGeometryIndex(node);

        if (index < m_References.size()) {
            m_References[index] = false;

            setHidden(node);
        }
    }

    void Geometry::setTransform(const unsigned long index,
                                const glm::mat4x4 &transform) {
        if (index < numberOfInstances()) {
            const unsigned long STRIDE = 16 * numberOfVertices();

#ifdef USE_HALF_FLOAT
            transform.getOpenGLMatrix(m_MatrixBufferFullSize);
            for (unsigned long i = 0; i < 16; i++)
                m_MatrixBuffer[i] =
                    convertFloatToHFloat(&m_MatrixBufferFullSize[i]);
#else
            memcpy(m_MatrixBuffer, &transform[0], sizeof(glm::mat4x4));
#endif

            for (int currentVertex = 0; currentVertex < numberOfVertices();
                 currentVertex++) {
                unsigned long p = ((index * STRIDE) + (16 * currentVertex));
                int cmp = memcmp(m_ModelViewTransformData + p, m_MatrixBuffer,
                                 sizeof(GLfptype) * 16);

                if (0 != cmp) {
                    memcpy(m_ModelViewTransformData + p, m_MatrixBuffer,
                           sizeof(GLfptype) * 16);
                }
            }
            enableModelViewBufferChanged(true);
        }
    }

    glm::mat4x4 Geometry::getTransform(const unsigned long index) {
        glm::mat4x4 transform(1.0);
        if (index < numberOfInstances()) {
            const unsigned long STRIDE = 16 * numberOfVertices();

            for (int currentVertex = 0; currentVertex < numberOfVertices();
                 currentVertex++) {
                unsigned long p = ((index * STRIDE) + (16 * currentVertex));
                memcpy(m_MatrixBuffer, m_ModelViewTransformData + p,
                       sizeof(GLfptype) * 16);
            }

#ifdef USE_HALF_FLOAT
            for (unsigned long i = 0; i < 16; i++)
                m_MatrixBufferFullSize[i] =
                    convertHFloatToFloat(m_MatrixBuffer[i]);
            transform.setFromOpenGLMatrix(m_MatrixBufferFullSize);
#else
            memcpy(&transform[0], m_MatrixBuffer, sizeof(glm::mat4x4));
#endif
        }
        return transform;
    }

    void Geometry::setNormalMatrixTransform(const unsigned long index,
                                            const glm::mat4x4 &transform) {
        if (index < numberOfInstances()) {
            const unsigned long STRIDE = 16 * numberOfVertices();

#ifdef USE_HALF_FLOAT
            transform.getOpenGLMatrix(m_MatrixBufferFullSize);
            for (unsigned long i = 0; i < 16; i++)
                m_MatrixBuffer[i] =
                    convertFloatToHFloat(&m_MatrixBufferFullSize[i]);
#else
            memcpy(m_MatrixBuffer, &transform[0], sizeof(glm::mat4x4));
#endif

            for (int currentVertex = 0; currentVertex < numberOfVertices();
                 currentVertex++) {
                unsigned long p = ((index * STRIDE) + (16 * currentVertex));

                int cmp = memcmp(m_NormalMatrixTransformData + p,
                                 m_MatrixBuffer, sizeof(GLfptype) * 16);

                if (0 != cmp) {
                    memcpy(m_NormalMatrixTransformData + p, m_MatrixBuffer,
                           sizeof(GLfptype) * 16);
                }
            }
            enableNormalMatrixBufferChanged(true);
        }
    }

    glm::mat4x4 Geometry::getNormalMatrixTransform(const unsigned long index) {
        glm::mat4x4 transform(1);
        if (index < numberOfInstances()) {
            const unsigned long STRIDE = 16 * numberOfVertices();

            for (int currentVertex = 0; currentVertex < numberOfVertices();
                 currentVertex++) {
                unsigned long p = ((index * STRIDE) + (16 * currentVertex));
                memcpy(m_MatrixBuffer, m_NormalMatrixTransformData + p,
                       sizeof(GLfptype) * 16);
            }

#ifdef USE_HALF_FLOAT
            for (unsigned long i = 0; i < 16; i++)
                m_MatrixBufferFullSize[i] =
                    convertHFloatToFloat(m_MatrixBuffer[i]);
            transform.setFromOpenGLMatrix(m_MatrixBufferFullSize);
#else
            memcpy(&transform[0], m_MatrixBuffer, sizeof(glm::mat4x4));
#endif
        }
        return transform;
    }
} // namespace NJLIC
