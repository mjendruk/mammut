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

MotionBlurPostProc::MotionBlurPostProc()
{
    initialize();
}

MotionBlurPostProc::~MotionBlurPostProc()
{
}


void MotionBlurPostProc::apply(glow::FrameBufferObject & fbo)
{
    //fist tileMax pass
    m_tmVerticalPass.apply(*m_TMTempFBO);
    
    //second tileMax pass
    m_TMTempTexture->bind(GL_TEXTURE0 + TIU_BufferCount);
    m_tmHorizontalPass.apply(*m_TMFBO);
    m_TMTempTexture->unbind(GL_TEXTURE0 + TIU_BufferCount);

    //neighborMax pass
    m_TMTexture->bind(GL_TEXTURE0 + TIU_BufferCount);
    m_neighborMaxPass.apply(*m_NMFBO);
    m_TMTexture->unbind(GL_TEXTURE0 + TIU_BufferCount);

    //motion blur pass
    m_NMTexture->bind(GL_TEXTURE0 + TIU_BufferCount);
    m_randomTexture->bind(GL_TEXTURE0 + TIU_BufferCount + 1);
    m_blurPass.apply(fbo);
    m_randomTexture->unbind(GL_TEXTURE0 + TIU_BufferCount + 1);
    m_NMTexture->unbind(GL_TEXTURE0 + TIU_BufferCount);
}

void MotionBlurPostProc::setInputTextures(const QMap<QString, int> input)
{
    //split into 4 Maps for each pass
    QMap<QString, int> tmVerticalInputTextures;
    tmVerticalInputTextures["velocity"] = input.value("velocity", TIU_Velocity);
    m_tmVerticalPass.setInputTextures(tmVerticalInputTextures);

    m_tmHorizontalPass.setInputTextures({ { "tmVertical", TIU_BufferCount } });

    m_neighborMaxPass.setInputTextures({ { "maxTile", TIU_BufferCount } });

    QMap<QString, int> blurInputTextures;
    blurInputTextures["depth"] = input.value("depth", TIU_Depth);
    blurInputTextures["color"] = input.value("color", TIU_Color);
    blurInputTextures["velocity"] = input.value("velocity", TIU_Velocity);
    blurInputTextures["neighborMax"] = TIU_BufferCount;
    blurInputTextures["randomBuffer"] = TIU_BufferCount + 1;
    m_blurPass.setInputTextures(blurInputTextures);
}


void MotionBlurPostProc::set2DTextureOutput(const QMap<GLenum, glow::Texture*> output)
{
    m_tmVerticalPass.set2DTextureOutput({}); //own FBOs know where to draw
    m_tmHorizontalPass.set2DTextureOutput({});
    m_neighborMaxPass.set2DTextureOutput({});
    m_blurPass.set2DTextureOutput(output);
}

void MotionBlurPostProc::initialize()
{
    m_tmVerticalPass.setFragmentShader("data/shaders/motionBlurTM_vertical.frag");

    m_tmHorizontalPass.setFragmentShader("data/shaders/motionBlurTM_horizontal.frag");

    m_neighborMaxPass.setFragmentShader("data/shaders/motionBlurNM.frag");

    m_blurPass.setFragmentShader("data/shaders/motionBlur.frag");

    //Textures
    m_TMTempTexture = Util::create2DTexture();
    m_TMTexture = Util::create2DTexture();
    m_NMTexture = Util::create2DTexture();
    m_randomTexture = Util::create2DTexture();

    //static Uniforms
    const int N = 32;
    glow::Array<glm::float_t> randBuffer = glow::Array<glm::float_t>();

    for (int i = 0; i < N * N; ++i) {
        randBuffer << float(glm::linearRand(0.0, 1.0));
    }

    m_randomTexture->image2D(0, GL_RED, N, N, 0, GL_RED, GL_FLOAT, &randBuffer[0]);

    m_tmVerticalPass.setUniform("radius", radius);
    m_tmHorizontalPass.setUniform("radius", radius);
    m_neighborMaxPass.setUniform("radius", radius);
    m_blurPass.setUniform("radius", radius);
    m_blurPass.setUniform("numSamples", numSamples);

    //FBOs
    m_TMTempFBO = new glow::FrameBufferObject();
    m_TMTempFBO->attachTexture2D(GL_COLOR_ATTACHMENT0, m_TMTempTexture);
    m_TMTempFBO->setDrawBuffers({ GL_COLOR_ATTACHMENT0 });

    m_TMFBO = new glow::FrameBufferObject();
    m_TMFBO->attachTexture2D(GL_COLOR_ATTACHMENT0, m_TMTexture);
    m_TMFBO->setDrawBuffers({ GL_COLOR_ATTACHMENT0 });

    m_NMFBO = new glow::FrameBufferObject();
    m_NMFBO->attachTexture2D(GL_COLOR_ATTACHMENT0, m_NMTexture);
    m_NMFBO->setDrawBuffers({ GL_COLOR_ATTACHMENT0 });
}

void MotionBlurPostProc::resize(int width, int height)
{
    m_TMTempTexture->image2D(0, GL_RG16F, width, height / radius, 0, GL_RG, GL_FLOAT, nullptr);
    m_TMTexture->image2D(0, GL_RG16F, width / radius, height / radius, 0, GL_RG, GL_FLOAT, nullptr);
    m_NMTexture->image2D(0, GL_RG16F, width / radius, height / radius, 0, GL_RG, GL_FLOAT, nullptr);

    m_tmVerticalPass.setUniform("viewport", glm::vec2(width, height));
    m_tmHorizontalPass.setUniform("viewport", glm::vec2(width, height));
    m_neighborMaxPass.setUniform("viewport", glm::vec2(width, height));
    m_blurPass.setUniform("viewport", glm::vec2(width, height));

    m_tmVerticalPass.resize(width, height);
    m_tmHorizontalPass.resize(width, height);
    m_neighborMaxPass.resize(width, height);
    m_blurPass.resize(width, height);
}
