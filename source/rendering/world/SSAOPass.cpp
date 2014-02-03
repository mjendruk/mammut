#include "SSAOPass.h"

#include <glm/gtx/random.hpp>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glow/Texture.h>
#include <glow/FrameBufferObject.h>

#include "FileAssociatedShader.h"

const int SSAOPass::m_kernelSize = 32;
const int SSAOPass::m_noiseSize = 4;
const float SSAOPass::m_radius = 25.0f;

SSAOPass::SSAOPass(QString name)
:   PostprocessingPass(name)
,   m_ssaoInputTextures(new QMap<QString, int>)
,   m_blurInputTextures(new QMap<QString, int>)
{
    initialize();
}

SSAOPass::~SSAOPass()
{
}

void SSAOPass::apply(glow::FrameBufferObject & fbo)
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // first SSAO Pass
    m_program = m_ssaoProgram;
    m_inputTextures = m_ssaoInputTextures;
    auto save2DOutput = *m_output2D;
    set2DTextureOutput({});

    initBeforeDraw(*m_fbo);

    m_noiseTexture->bind(GL_TEXTURE0 + TIU_BufferCount);
    m_fbo->bind();
    m_ssaoQuad->draw();
    m_fbo->unbind();
    m_noiseTexture->unbind(GL_TEXTURE0 + TIU_BufferCount);
    
    //second SSAO Pass (blur)
    m_program = m_blurProgram;
    m_inputTextures = m_blurInputTextures;
    set2DTextureOutput(save2DOutput);

    initBeforeDraw(fbo);

    m_ssaoTexture->bind(GL_TEXTURE0 + TIU_BufferCount);
    fbo.bind();
    m_blurQuad->draw();
    fbo.unbind();
    m_ssaoTexture->unbind(GL_TEXTURE0 + TIU_BufferCount);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    m_program = m_ssaoProgram;
}

void SSAOPass::setInputTextures(const QMap<QString, int> & input)
{
    //split into 2 Maps for each pass (ssao, blur)
    m_ssaoInputTextures->clear();
    m_ssaoInputTextures->insert("normal", input.value("normal"));
    m_ssaoInputTextures->insert("depth",input.value("depth"));
    m_ssaoInputTextures->insert("noise",TIU_BufferCount);

    m_blurInputTextures->clear();
    m_blurInputTextures->insert("color", input.value("color"));
    m_blurInputTextures->insert("ssao", TIU_BufferCount);

    m_inputTextures = m_ssaoInputTextures;
}

void SSAOPass::initialize()
{
    initializeSSAOPrograms();

    m_ssaoTexture = new glow::Texture(GL_TEXTURE_2D);
    m_ssaoTexture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_ssaoTexture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_ssaoTexture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_ssaoTexture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_ssaoTexture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_noiseTexture = new glow::Texture(GL_TEXTURE_2D);
    m_noiseTexture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_noiseTexture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_noiseTexture->setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    m_noiseTexture->setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    m_noiseTexture->setParameter(GL_TEXTURE_WRAP_R, GL_REPEAT);


    glow::Array<glm::vec3> kernel = glow::Array<glm::vec3>();
    for (int i = 0; i < m_kernelSize; ++i) {
        kernel << glm::normalize(glm::vec3(
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(0.1f, 1.0f))
            );

        float scale = glm::linearRand(0.0f, 1.0f);
        scale = glm::mix(0.1f, 1.0f, scale * scale);
        kernel[i] *= scale;
    }

    const int noiseBufferSize = m_noiseSize*m_noiseSize;
    glow::Array<glm::vec3> noise = glow::Array<glm::vec3>();
    for (int i = 0; i < noiseBufferSize; ++i) {
        noise << glm::normalize(glm::vec3(
            glm::linearRand(-1.0f, 1.0f),
            glm::linearRand(-1.0f, 1.0f),
            0.0f)
            );
    }

    m_noiseTexture->image2D(0, GL_RGB32F, m_noiseSize, m_noiseSize, 0, GL_RGB, GL_FLOAT, &noise[0]);

    m_ssaoProgram->setUniform("noiseTexSize", m_noiseSize);
    m_ssaoProgram->setUniform("kernelSize", m_kernelSize);
    m_ssaoProgram->setUniform("kernel", kernel);
    m_ssaoProgram->setUniform("radius", m_radius);

    m_fbo = new glow::FrameBufferObject();
    m_fbo->attachTexture2D(GL_COLOR_ATTACHMENT0, m_ssaoTexture);
    m_fbo->setDrawBuffers({ GL_COLOR_ATTACHMENT0 });
}

void SSAOPass::initializeSSAOPrograms()
{
    setVertexShader("data/blur.vert");
    setFragmentShader("data/blur.frag");
    m_blurProgram = initializeProgram(); 
    m_blurQuad = new glowutils::ScreenAlignedQuad(m_blurProgram);

    setVertexShader("data/ssao.vert");
    setFragmentShader("data/ssao.frag");
    m_ssaoProgram = initializeProgram();
    m_ssaoQuad = new glowutils::ScreenAlignedQuad(m_ssaoProgram);
}

void SSAOPass::resize(int width, int height)
{
    m_ssaoTexture->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    m_ssaoProgram->setUniform("viewport", glm::vec2(width, height));
}

