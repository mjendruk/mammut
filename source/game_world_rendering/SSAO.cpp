#include "SSAO.h"

#include <QDebug>
#include <glm/gtx/random.hpp>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glow/Texture.h>
#include <glow/FrameBufferObject.h>

#include "FileAssociatedShader.h"

const int SSAO::m_kernelSize = 32;
const int SSAO::m_noiseSize = 4;

SSAO::SSAO()
:   glow::Referenced()
,   m_ssaoQuad(nullptr)
,   m_blurQuad(nullptr)
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

    m_ssaoQuad = new glowutils::ScreenAlignedQuad(&m_ssaoProgram);
    m_blurQuad = new glowutils::ScreenAlignedQuad(&m_blurProgram);

    m_ssaoTexture = new glow::Texture(GL_TEXTURE_2D);
    m_ssaoTexture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_ssaoTexture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_ssaoTexture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_ssaoTexture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_ssaoTexture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_fbo = new glow::FrameBufferObject();
    m_fbo->attachTexture2D(GL_COLOR_ATTACHMENT0, m_ssaoTexture);

    m_fbo->setDrawBuffers({ GL_COLOR_ATTACHMENT0 });

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
    m_ssaoProgram.setUniform("noiseTexSize", m_noiseSize);

    m_noiseTexture->image2D(0, GL_RGB32F, m_noiseSize, m_noiseSize, 0, GL_RGB, GL_FLOAT, &noise[0]);
    m_ssaoProgram.setUniform("kernel", kernel);

    m_ssaoProgram.setUniform("kernelSize", m_kernelSize);
    m_ssaoProgram.setUniform("radius", 25.0f);
}

void SSAO::draw(int normalTexture, int depthTexture, const glm::mat3 & normal, const glm::mat4 & projection, glow::Texture & outTexture)
{
    m_fbo->bind();
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_ssaoProgram.setUniform("projection", projection);
    m_ssaoProgram.setUniform("invProjection", glm::inverse(projection));
    m_ssaoProgram.setUniform("normalMatrix", normal);
    m_ssaoProgram.setUniform("normal", 0);
    m_ssaoProgram.setUniform("depth", 2);
    m_ssaoProgram.setUniform("noise", 3);

    m_noiseTexture->bind(GL_TEXTURE3);

    m_ssaoQuad->draw();

    m_noiseTexture->unbind(GL_TEXTURE3);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    m_fbo->unbind();



    glow::FrameBufferObject tempFBO;
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
    m_ssaoProgram.setUniform("viewport", glm::vec2(width, height));
}

