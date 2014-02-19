#include "SSAOPass.h"

#include <array>

#include <glm/gtx/random.hpp>

#include <glow/Texture.h>
#include <glowutils/Icosahedron.h>

#include "Util.h"

const int SSAOPass::s_kernelSize = 24;
const int SSAOPass::s_noiseSize = 7;
const QList<QString> SSAOPass::s_requiredSamplers = { "normal_depth", "color" };

SSAOPass::SSAOPass()
:   m_ssaoPass("data/shaders/ssao.vert", "data/shaders/ssao.frag", GL_R8)
,   m_horizontalBlurPass("data/shaders/horizontal_blur.frag", GL_R8)
,   m_verticalBlurPass("data/shaders/vertical_blur.frag", GL_RGB8)
{
    initialize();
}

SSAOPass::~SSAOPass()
{
}

void SSAOPass::apply()
{
    // first SSAO Pass
    m_ssaoPass.apply();
    
    //second SSAO Pass (blur)
    m_horizontalBlurPass.apply();
    m_verticalBlurPass.apply();
}

void SSAOPass::setInputTextures(const QMap<QString, glow::Texture*> & input)
{
    for (QString sampler : s_requiredSamplers)
        assert(input.contains(sampler));

    //split into 2 Maps for each pass (ssao, blur)
    QMap<QString, glow::Texture *> ssaoInputTextures;
    ssaoInputTextures["normal_depth"] = input.value("normal_depth");
    ssaoInputTextures["noise"] = m_noiseTexture;

    m_ssaoPass.setInputTextures(ssaoInputTextures);

    QMap<QString, glow::Texture *> horizontalBlurInputTextures;
    horizontalBlurInputTextures["ssao"] = m_ssaoTexture;
    horizontalBlurInputTextures["normal_depth"] = input.value("normal_depth");
    m_horizontalBlurPass.setInputTextures(horizontalBlurInputTextures);
    
    QMap<QString, glow::Texture *> verticalBlurInputTextures;
    verticalBlurInputTextures["horizontalBlur"] = m_horizontalBlurTexture;
    verticalBlurInputTextures["normal_depth"] = input.value("normal_depth");
    verticalBlurInputTextures["color"] = input.value("color");
    m_verticalBlurPass.setInputTextures(verticalBlurInputTextures);
}

glow::Texture* SSAOPass::outputTexture()
{
    return m_verticalBlurPass.outputTexture();
}

void SSAOPass::initialize()
{
    m_ssaoTexture = m_ssaoPass.outputTexture();
    m_horizontalBlurTexture = m_horizontalBlurPass.outputTexture();
    m_verticalBlurTexture = m_verticalBlurPass.outputTexture();


    initializeKernel();
    initializeNoise();
}

void SSAOPass::initializeKernel()
{
    std::vector<glm::vec3> kernel;
    
    for (int i = 0; i < s_kernelSize; ++i) {
        glm::vec3 sample = glm::normalize(glm::vec3(glm::linearRand(-1.0f, 1.0f),
                                                    glm::linearRand(-1.0f, 1.0f),
                                                    glm::linearRand(0.2f, 1.0f)));
        
        float scale = float(i) / float(s_kernelSize);
        scale = glm::mix(0.1f, 1.0f, scale * scale);
        sample *= scale;
        
        kernel.push_back(sample);
    }

    m_ssaoPass.setUniform("kernelSize", s_kernelSize);
    m_ssaoPass.setUniform("kernel", kernel);
}

void SSAOPass::initializeNoise()
{
    std::vector<glm::vec3> noise;
    const int noiseBufferSize = s_noiseSize * s_noiseSize;

    for (int i = 0; i < noiseBufferSize; ++i) {
        glm::vec3 sample = glm::normalize(glm::vec3(glm::linearRand(-1.0f, 1.0f),
                                                    glm::linearRand(-1.0f, 1.0f),
                                                    0.0f));

        noise.push_back(sample);
    }

    m_noiseTexture = Util::create2DTexture(GL_LINEAR, GL_REPEAT);
    m_noiseTexture->image2D(0, GL_RGB32F, s_noiseSize, s_noiseSize, 0, GL_RGB, GL_FLOAT, &noise[0]);
}

void SSAOPass::resize(int width, int height)
{
    m_ssaoPass.setUniform("noiseScale", glm::vec2(width, height) / glm::vec2(s_noiseSize));
    
    m_horizontalBlurPass.setUniform("texelSize", 1.0f / glm::vec2(width, height));
    m_verticalBlurPass.setUniform("texelSize", 1.0f / glm::vec2(width, height));

    m_ssaoPass.resize(width, height);
    m_horizontalBlurPass.resize(width, height);
    m_verticalBlurPass.resize(width, height);
}

void SSAOPass::setProjectionUniform(const glm::mat4 & projection)
{
    m_ssaoPass.setUniform("projection", projection);
}

void SSAOPass::setInverseProjectionUniform(const glm::mat4 & inverseProjection)
{
    m_ssaoPass.setUniform("inverseProjection", inverseProjection);
}

void SSAOPass::setFarPlaneUniform(float farPlane)
{
    m_ssaoPass.setUniform("farPlane", farPlane);
}
