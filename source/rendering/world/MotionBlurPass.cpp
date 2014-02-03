#include "MotionBlurPass.h"

#include <QDebug>
#include <glm/gtx/random.hpp>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glow/Texture.h>
#include <glow/FrameBufferObject.h>

#include "FileAssociatedShader.h"

MotionBlurPass::MotionBlurPass()
:   glow::Referenced()
, m_motionBlurQuad(nullptr)
{
    initialize();
}

MotionBlurPass::~MotionBlurPass()
{
}

void MotionBlurPass::initialize()
{
    glow::Shader * motionBlurFrag = FileAssociatedShader::getOrCreate(
        GL_FRAGMENT_SHADER, "data/motionBlur.frag", m_motionBlurProgram);
    glow::Shader * motionBlurVert = FileAssociatedShader::getOrCreate(
        GL_VERTEX_SHADER, "data/motionBlur.vert", m_motionBlurProgram);
    m_motionBlurProgram.link();

    m_motionBlurQuad = new glowutils::ScreenAlignedQuad(&m_motionBlurProgram);

    m_fbo = new glow::FrameBufferObject();
}

void MotionBlurPass::draw(int colorTexture, int depthTexture, int ssaoTexture,
    const glm::mat4 & currentViewProjectionInverted, const glm::mat4 previousViewProjection,
    int currentFPS, glow::Texture & outTexture)
{
    glow::FrameBufferObject tempFBO;
    tempFBO.attachTexture2D(GL_COLOR_ATTACHMENT0, &outTexture);
    tempFBO.setDrawBuffers({ GL_COLOR_ATTACHMENT0 });

    tempFBO.bind();
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glClear(GL_COLOR_BUFFER_BIT);

    m_motionBlurProgram.setUniform("color", colorTexture);
    m_motionBlurProgram.setUniform("depth", depthTexture);
    m_motionBlurProgram.setUniform("ssao", ssaoTexture);
    m_motionBlurProgram.setUniform("currentViewProjectionInverted", currentViewProjectionInverted);
    m_motionBlurProgram.setUniform("previousViewProjection", previousViewProjection);
    m_motionBlurProgram.setUniform("targetFPS", 60);
    m_motionBlurProgram.setUniform("currentFPS", currentFPS);

    m_motionBlurQuad->draw();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    tempFBO.unbind();
}

