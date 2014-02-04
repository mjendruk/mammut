#include "MotionBlurPass.h"

#include <QDebug>
#include <glm/gtx/random.hpp>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glow/Texture.h>
#include <glow/FrameBufferObject.h>

#include "FileAssociatedShader.h"

const float MotionBlurPass::radius = 20.f;
const float MotionBlurPass::numSamples = 15.f;

MotionBlurPass::MotionBlurPass(const QString name)
:   PostprocessingPass(name)
,   m_TMTempInputTextures(new QMap<QString, int>)
,   m_TMInputTextures(new QMap<QString, int>)
,   m_NMInputTextures(new QMap<QString, int>)
,   m_blurInputTextures(new QMap<QString, int>)
{
    initialize();
}

MotionBlurPass::~MotionBlurPass()
{
}


void MotionBlurPass::apply(glow::FrameBufferObject & fbo)
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //fist tileMax pass
    m_program = m_TMProgram;
    m_inputTextures = m_TMTempInputTextures;
    auto save2DOutput = *m_output2D;
    set2DTextureOutput({}); //local FBOs know where to draw

    initBeforeDraw(*m_TMTempFBO);

    m_TMTempFBO->bind();
    m_TMQuad->draw();
    m_TMTempFBO->unbind();

    //second tileMax pass
    m_inputTextures = m_TMTempInputTextures;

    initBeforeDraw(*m_TMFBO);

    m_TMTempTexture->bind(GL_TEXTURE0 + TIU_BufferCount);
    m_TMFBO->bind();
    m_TMQuad->draw();
    m_TMFBO->unbind();
    m_TMTempTexture->unbind(GL_TEXTURE0 + TIU_BufferCount);

    //neighborMax pass
    m_program = m_NMProgram;
    m_inputTextures = m_NMInputTextures;

    initBeforeDraw(*m_NMFBO);

    m_TMTexture->bind(GL_TEXTURE0 + TIU_BufferCount);
    m_NMFBO->bind();
    m_NMQuad->draw();
    m_NMFBO->unbind();
    m_TMTexture->unbind(GL_TEXTURE0 + TIU_BufferCount);

    //motion blur pass
    m_program = m_blurProgram;
    m_inputTextures = m_blurInputTextures;
    set2DTextureOutput(save2DOutput);

    initBeforeDraw(fbo);
    
    m_TMTexture->bind(GL_TEXTURE0 + TIU_BufferCount);
    m_randomTexture->bind(GL_TEXTURE0 + TIU_BufferCount + 1);
    fbo.bind();
    m_blurQuad->draw();
    fbo.unbind();
    m_randomTexture->unbind(GL_TEXTURE0 + TIU_BufferCount + 1);
    m_TMTexture->unbind(GL_TEXTURE0 + TIU_BufferCount);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}

void MotionBlurPass::setInputTextures(const QMap<QString, int> & input)
{
    //split into 4 Maps for each pass
    m_TMTempInputTextures->clear();
    m_TMTempInputTextures->insert("velocity", input.value("velocity"));

    m_TMInputTextures->clear();
    m_TMInputTextures->insert("velocity", TIU_BufferCount);

    m_NMInputTextures->clear();
    m_NMInputTextures->insert("maxTile", TIU_BufferCount);

    m_blurInputTextures->clear();
    m_blurInputTextures->insert("depth", input.value("depth"));
    m_blurInputTextures->insert("color", input.value("color"));
    m_blurInputTextures->insert("velocity", input.value("velocity"));
    m_blurInputTextures->insert("neighborMax", TIU_BufferCount);
    m_blurInputTextures->insert("randomBuffer", TIU_BufferCount+1);
}

void MotionBlurPass::initialize()
{
    initializePrograms();

    //Textures
    m_TMTempTexture = new glow::Texture(GL_TEXTURE_2D);
    m_TMTempTexture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_TMTempTexture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_TMTempTexture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_TMTempTexture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_TMTempTexture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_TMTexture = new glow::Texture(GL_TEXTURE_2D);
    m_TMTexture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_TMTexture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_TMTexture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_TMTexture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_TMTexture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    m_NMTexture = new glow::Texture(GL_TEXTURE_2D);
    m_NMTexture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_NMTexture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_NMTexture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_NMTexture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_NMTexture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_randomTexture = new glow::Texture(GL_TEXTURE_2D);
    m_NMTexture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_NMTexture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_NMTexture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_NMTexture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_NMTexture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //static Uniforms
    const int N = 32;
    glow::Array<glm::float_t> randBuffer = glow::Array<glm::float_t>();

    for (int i = 0; i < N * N; ++i) {
        randBuffer << glm::linearRand(0.0, 255.0);
    }

    //    glow::ref_ptr<glow::Texture> m_motionBlurNoise;
    m_blurProgram->setUniform("randomBuffer", randBuffer);

    m_TMProgram->setUniform("radius", radius);
    m_NMProgram->setUniform("radius", radius);
    m_blurProgram->setUniform("radius", radius);
    m_blurProgram->setUniform("numSamples", numSamples); 

    //FBOS
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

void MotionBlurPass::initializePrograms()
{
    setFragmentShader("data/motionBlurTM.frag");
    m_TMProgram = initializeProgram();
    m_TMQuad = new glowutils::ScreenAlignedQuad(m_TMProgram);

    setFragmentShader("data/motionBlurNM.frag");
    m_NMProgram = initializeProgram();
    m_NMQuad = new glowutils::ScreenAlignedQuad(m_NMProgram);

    setFragmentShader("data/motionBlur.frag");
    m_blurProgram = initializeProgram();
    m_blurQuad = new glowutils::ScreenAlignedQuad(m_blurProgram);

    m_program = m_blurProgram;
}

void MotionBlurPass::resize(int width, int height)
{
    m_TMTempTexture->image2D(0, GL_RG16F, height, width / radius, 0, GL_RG, GL_FLOAT, nullptr);
    m_TMTexture->image2D(0, GL_RG16F, width / radius, height / radius, 0, GL_RG, GL_FLOAT, nullptr);
    m_NMTexture->image2D(0, GL_RG16F, width / radius, height / radius, 0, GL_RG, GL_FLOAT, nullptr);

    m_TMProgram->setUniform("viewport", glm::vec2(width, height));
    m_NMProgram->setUniform("viewport", glm::vec2(width, height));
    m_blurProgram->setUniform("viewport", glm::vec2(width, height));
}
