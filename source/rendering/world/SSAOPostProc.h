#pragma once

#include <glm/glm.hpp>

#include "SimplePostProcPass.h"

class SSAOPostProc : public AbstractPostProc
{
public:
    SSAOPostProc(TextureFormat outputFormat);
    virtual ~SSAOPostProc();

    virtual void apply();
    virtual void resize(int width, int height);
    virtual void setInputTextures(const QMap<QString, glow::Texture*> & input);
    virtual glow::Texture* outputTexture();

    void setProjectionUniform(glm::mat4 projection);
    void setInverseProjectionUniform(glm::mat4 invProjection);
    void setNormalMatrixUniform(glm::mat3 normalMatrix);

protected:
    void initialize();

protected:
    static const int s_kernelSize;
    static const int s_noiseSize;
    static const float s_radius;
    static const QList<QString> s_requiredSamplers;

    SimplePostProcPass m_ssaoPass;
    SimplePostProcPass m_blurPass;

    glow::ref_ptr<glow::Texture> m_ssaoOutputTexture;

    glow::ref_ptr<glow::Texture> m_noiseTexture;
};
