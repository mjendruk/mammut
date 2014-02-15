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
    void setInverseProjectionUniform(const glm::mat4 & invProjection);
    void setNormalMatrixUniform(const glm::mat3 & normalMatrix);

protected:
    void initialize();
    void pointsOnSphere(const unsigned int minN, std::vector<glm::vec3> & points);

protected:
    static const int s_kernelSize;
    static const int s_noiseSize;
    static const float s_radius;
    static const QList<QString> s_requiredSamplers;

    SimplePostProcPass m_ssaoPass;
    SimplePostProcPass m_blurPass;

    glow::ref_ptr<glow::Texture> m_ssaoOutputTexture;
    glow::ref_ptr<glow::Texture> m_noiseTexture;

    std::mt19937 rng;
};
