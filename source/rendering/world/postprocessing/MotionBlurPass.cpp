#include "MotionBlurPass.h"

#include <vector>

#include <QString>

#include <glm/gtx/random.hpp>

#include <glow/Texture.h>

#include "Util.h"

const float MotionBlurPass::s_radius = 20.f;
const int MotionBlurPass::s_numSamples = 15; //must be odd 
const int MotionBlurPass::s_randomBufferSize = 32;
const QList<QString> MotionBlurPass::s_requiredSamplers = { "depth", "color", "velocity" };

MotionBlurPass::MotionBlurPass()
:   m_tmVerticalPass("data/shaders/motionBlurTM_vertical.frag", GL_RG16F)
,   m_tmHorizontalPass("data/shaders/motionBlurTM_horizontal.frag", GL_RG16F)
,   m_neighborMaxPass("data/shaders/motionBlurNM.frag", GL_RG16F)
,   m_blurPass("data/shaders/motionBlur.frag", GL_RGBA32F)
{
    m_blurPass.outputTexture()->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_blurPass.outputTexture()->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    initialize();
}

MotionBlurPass::~MotionBlurPass()
{
}


void MotionBlurPass::apply()
{
    m_tmVerticalPass.apply();
    m_tmHorizontalPass.apply();

    m_neighborMaxPass.apply();

    m_blurPass.apply();
}

void MotionBlurPass::setInputTextures(const QMap<QString, glow::Texture *> & input)
{
    for (QString sampler : s_requiredSamplers)
        assert(input.contains(sampler));

    //split into 4 Maps for each pass
    QMap<QString, glow::Texture *> tmVerticalInputTextures;
    tmVerticalInputTextures["velocity"] = input.value("velocity");
    m_tmVerticalPass.setInputTextures(tmVerticalInputTextures);

    m_tmHorizontalPass.setInputTextures({ { "tmVertical", m_TMVerticalOutputTexture } });

    m_neighborMaxPass.setInputTextures({ { "maxTile", m_TMOutputTexture } });

    QMap<QString, glow::Texture *> blurInputTextures;
    blurInputTextures["depth"] = input.value("depth");
    blurInputTextures["color"] = input.value("color");
    blurInputTextures["velocity"] = input.value("velocity");
    blurInputTextures["neighborMax"] = m_NMOutputTexture;
    blurInputTextures["randomBuffer"] = m_randomTexture;
    m_blurPass.setInputTextures(blurInputTextures);
}

glow::Texture * MotionBlurPass::outputTexture()
{
    return m_blurPass.outputTexture();
}

void MotionBlurPass::initialize()
{
    //Textures
    m_TMVerticalOutputTexture = m_tmVerticalPass.outputTexture();
    m_TMOutputTexture = m_tmHorizontalPass.outputTexture();
    m_NMOutputTexture = m_neighborMaxPass.outputTexture();

    std::vector<glm::float_t> randBuffer = std::vector<glm::float_t>();

    for (int i = 0; i < s_randomBufferSize * s_randomBufferSize; ++i) {
        randBuffer.push_back(float(glm::linearRand(0.0, 1.0)));
    }

    m_randomTexture = Util::create2DTexture(GL_LINEAR, GL_REPEAT);
    m_randomTexture->image2D(0, GL_RED, s_randomBufferSize, s_randomBufferSize, 0, GL_RED, GL_FLOAT, &randBuffer[0]);

    //static Uniforms
    m_tmVerticalPass.setUniform("radius", s_radius);
    m_tmHorizontalPass.setUniform("radius", s_radius);
    m_neighborMaxPass.setUniform("radius", s_radius);
    m_blurPass.setUniform("radius", s_radius);
    m_blurPass.setUniform("numSamples", s_numSamples);
}

void MotionBlurPass::resize(int width, int height)
{
    m_tmVerticalPass.resize(width, height / s_radius);
    m_tmHorizontalPass.resize(width / s_radius, height / s_radius);
    m_neighborMaxPass.resize(width / s_radius, height / s_radius);
    m_blurPass.resize(width, height);

    m_tmVerticalPass.setUniform("viewport", glm::vec2(width, height));
    m_tmHorizontalPass.setUniform("viewport", glm::vec2(width, height));
    m_neighborMaxPass.setUniform("viewport", glm::vec2(width, height));
    m_blurPass.setUniform("viewport", glm::vec2(width, height));
}

void MotionBlurPass::setFPSUniform(float current_targetFPS)
{
    m_blurPass.setUniform("currentFPS_targetFPS", current_targetFPS);
}
