#include "MotionBlurPostProc.h"

#include <glm/gtx/random.hpp>

#include <glow/Texture.h>

#include "Util.h"

const float MotionBlurPostProc::s_radius = 20.f;
const int MotionBlurPostProc::s_numSamples = 15; //must be odd 
const int MotionBlurPostProc::s_randomBufferSize = 32;
const QList<QString> MotionBlurPostProc::s_requiredSamplers = { "depth", "color", "velocity" };

MotionBlurPostProc::MotionBlurPostProc(TextureFormat outputFormat)
:   m_tmVerticalPass(SimplePostProcPass({ GL_RG16F, GL_RG, GL_FLOAT }, "data/shaders/motionBlurTM_vertical.frag"))
,   m_tmHorizontalPass(SimplePostProcPass({ GL_RG16F, GL_RG, GL_FLOAT }, "data/shaders/motionBlurTM_horizontal.frag"))
,   m_neighborMaxPass(SimplePostProcPass({ GL_RG16F, GL_RG, GL_FLOAT }, "data/shaders/motionBlurNM.frag"))
,   m_blurPass(SimplePostProcPass(outputFormat, "data/shaders/motionBlur.frag"))
{
    initialize();
}

MotionBlurPostProc::~MotionBlurPostProc()
{
}


void MotionBlurPostProc::apply()
{
    m_tmVerticalPass.apply();
    m_tmHorizontalPass.apply();

    m_neighborMaxPass.apply();

    m_blurPass.apply();
}

void MotionBlurPostProc::setInputTextures(const QMap<QString, glow::Texture*> & input)
{
    for (QString sampler : s_requiredSamplers)
        assert(input.contains(sampler));

    //split into 4 Maps for each pass
    QMap<QString, glow::Texture*> tmVerticalInputTextures;
    tmVerticalInputTextures["velocity"] = input.value("velocity");
    m_tmVerticalPass.setInputTextures(tmVerticalInputTextures);

    m_tmHorizontalPass.setInputTextures({ { "tmVertical", m_TMVerticalOutputTexture } });

    m_neighborMaxPass.setInputTextures({ { "maxTile", m_TMOutputTexture } });

    QMap<QString, glow::Texture*> blurInputTextures;
    blurInputTextures["depth"] = input.value("depth");
    blurInputTextures["color"] = input.value("color");
    blurInputTextures["velocity"] = input.value("velocity");
    blurInputTextures["neighborMax"] = m_NMOutputTexture;
    blurInputTextures["randomBuffer"] = m_randomTexture;
    m_blurPass.setInputTextures(blurInputTextures);
}

glow::Texture* MotionBlurPostProc::outputTexture()
{
    return m_blurPass.outputTexture();
}

void MotionBlurPostProc::initialize()
{
    //Textures
    m_TMVerticalOutputTexture = m_tmVerticalPass.outputTexture();
    m_TMOutputTexture = m_tmHorizontalPass.outputTexture();
    m_NMOutputTexture = m_neighborMaxPass.outputTexture();

    std::vector<glm::float_t> randBuffer = std::vector<glm::float_t>();

    for (int i = 0; i < s_randomBufferSize * s_randomBufferSize; ++i) {
        randBuffer.push_back(float(glm::linearRand(0.0, 1.0)));
    }

    m_randomTexture = Util::create2DTexture();
    m_randomTexture->image2D(0, GL_RED, s_randomBufferSize, s_randomBufferSize, 0, GL_RED, GL_FLOAT, &randBuffer[0]);

    //static Uniforms
    m_tmVerticalPass.setUniform("radius", s_radius);
    m_tmHorizontalPass.setUniform("radius", s_radius);
    m_neighborMaxPass.setUniform("radius", s_radius);
    m_blurPass.setUniform("radius", s_radius);
    m_blurPass.setUniform("numSamples", s_numSamples);
}

void MotionBlurPostProc::resize(int width, int height)
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


void MotionBlurPostProc::setFPSUniform(float current_targetFPS)
{
    m_blurPass.setUniform("currentFPS_targetFPS", current_targetFPS);
}
