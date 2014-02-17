#pragma once

#include <random>
#include <vector>

#include <QString>

#include <glm/glm.hpp>

#include "SimplePostProcPass.h"


class SSAOPass : public AbstractPostProcPass
{
public:
    SSAOPass();
    virtual ~SSAOPass();

    void apply() override;
    void resize(int width, int height) override;
    
    void setInputTextures(const QMap<QString, glow::Texture *> & input) override;
    glow::Texture * outputTexture() override;

    void setProjectionUniform(const glm::mat4 & projection);
    void setInverseProjectionUniform(const glm::mat4 & inverseProjection);
    void setFarPlaneUniform(float farPlane);

protected:
    void initialize();
    void initializeKernel();
    void initializeNoise();

protected:
    static const int s_kernelSize;
    static const int s_noiseSize;
    static const QList<QString> s_requiredSamplers;

    SimplePostProcPass m_ssaoPass;
    SimplePostProcPass m_horizontalBlurPass;
    SimplePostProcPass m_verticalBlurPass;

    glow::ref_ptr<glow::Texture> m_ssaoTexture;
    glow::ref_ptr<glow::Texture> m_noiseTexture;
    glow::ref_ptr<glow::Texture> m_horizontalBlurTexture;
    glow::ref_ptr<glow::Texture> m_verticalBlurTexture;

    std::mt19937 rng;
};
