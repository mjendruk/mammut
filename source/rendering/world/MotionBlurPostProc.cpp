#include "MotionBlurPostProc.h"

#include <glm/gtx/random.hpp>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glow/Texture.h>
#include <glow/FrameBufferObject.h>

#include "Util.h"

const float MotionBlurPostProc::radius = 20.f;
const int MotionBlurPostProc::numSamples = 15; //must be odd 
const int MotionBlurPostProc::randomBufferSize = 32;
const QList<QString> MotionBlurPostProc::requiredSamplers = { "depth", "color", "velocity" };

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
    for (QString sampler : requiredSamplers)
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

    glow::Array<glm::float_t> randBuffer = glow::Array<glm::float_t>();

    for (int i = 0; i < randomBufferSize * randomBufferSize; ++i) {
        randBuffer << float(glm::linearRand(0.0, 1.0));
    }

    m_randomTexture = Util::create2DTexture();
    m_randomTexture->image2D(0, GL_RED, randomBufferSize, randomBufferSize, 0, GL_RED, GL_FLOAT, &randBuffer[0]);

    //static Uniforms
    m_tmVerticalPass.setUniform("radius", radius);
    m_tmHorizontalPass.setUniform("radius", radius);
    m_neighborMaxPass.setUniform("radius", radius);
    m_blurPass.setUniform("radius", radius);
    m_blurPass.setUniform("numSamples", numSamples);
}

void MotionBlurPostProc::resize(int width, int height)
{
    m_tmVerticalPass.resize(width, height / radius);
    m_tmHorizontalPass.resize(width / radius, height / radius);
    m_neighborMaxPass.resize(width / radius, height / radius);
    m_blurPass.resize(width, height);

    m_tmVerticalPass.setUniform("viewport", glm::vec2(width, height));
    m_tmHorizontalPass.setUniform("viewport", glm::vec2(width, height));
    m_neighborMaxPass.setUniform("viewport", glm::vec2(width, height));
    m_blurPass.setUniform("viewport", glm::vec2(width, height));
}
