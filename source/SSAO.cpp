#include "SSAO.h"

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glow/Texture.h>
#include <glow/FrameBufferObject.h>

#include "FileAssociatedShader.h"


SSAO::SSAO()
:   glow::Referenced()
, m_ssaoQuad(nullptr)
, m_blurQuad(nullptr)
{
    initialize();
}

SSAO::~SSAO()
{
}

void SSAO::initialize()
{
    glow::Shader * ssaoFrag = FileAssociatedShader::getOrCreate(
        GL_FRAGMENT_SHADER, "data/ssao.frag", m_ssaoProgram);
    glow::Shader * ssaoVert = FileAssociatedShader::getOrCreate(
        GL_VERTEX_SHADER, "data/ssao.vert", m_ssaoProgram);
    m_ssaoProgram.link();

    glow::Shader * blurFrag = FileAssociatedShader::getOrCreate(
        GL_FRAGMENT_SHADER, "data/blur.frag", m_blurProgram);
    glow::Shader * blurVert = FileAssociatedShader::getOrCreate(
        GL_VERTEX_SHADER, "data/blur.vert", m_blurProgram);
    m_blurProgram.link();

    m_ssaoQuad = new glow::ScreenAlignedQuad(&m_ssaoProgram);
    m_blurQuad = new glow::ScreenAlignedQuad(&m_blurProgram);

    m_ssaoTexture = new glow::Texture(GL_TEXTURE_2D);
    m_ssaoTexture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_ssaoTexture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_ssaoTexture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_ssaoTexture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_ssaoTexture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_fbo = new glow::FrameBufferObject();
    m_fbo->attachTexture2D(GL_COLOR_ATTACHMENT0, m_ssaoTexture);

    m_fbo->setDrawBuffers({ GL_COLOR_ATTACHMENT0 });
}

void SSAO::draw(int normalTexture, int depthTexture, glow::Texture & outTexture)
{
    m_fbo->bind();
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_ssaoProgram.setUniform("normal", 0);
    m_ssaoProgram.setUniform("depth", 2);

    m_ssaoQuad->draw();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    m_fbo->unbind();

    glow::FrameBufferObject tempFBO = glow::FrameBufferObject();
    tempFBO.attachTexture2D(GL_COLOR_ATTACHMENT0, &outTexture);
    tempFBO.setDrawBuffers({ GL_COLOR_ATTACHMENT0 });

    tempFBO.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    m_blurProgram.setUniform("ssao", 3);
    m_ssaoTexture->bind(GL_TEXTURE3);
    
    m_blurQuad->draw();

    m_ssaoTexture->unbind(GL_TEXTURE3);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    tempFBO.unbind();
}

void SSAO::resize(int width, int height)
{
    m_ssaoTexture->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
}

