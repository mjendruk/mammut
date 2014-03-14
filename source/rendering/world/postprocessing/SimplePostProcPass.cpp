#include "SimplePostProcPass.h"

#include <cassert>

#include <QString>
#include <QVector>

#include <glow/Program.h>
#include <glow/Texture.h>
#include <glow/FrameBufferObject.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/global.h>

#include <Util.h>


SimplePostProcPass::SimplePostProcPass(
    const QString & vertexShaderSource,
    const QString & fragmentShaderSource,
    GLenum outputTextureFormat,
    GLenum outputTextureFiltering)
:   m_textureFormat(outputTextureFormat)
{
    initializeFbo(outputTextureFiltering);

    glow::ref_ptr<glow::Program> program = new glow::Program();
    
    program->attach(glowutils::createShaderFromFile(GL_VERTEX_SHADER, vertexShaderSource.toStdString()),
                    glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, fragmentShaderSource.toStdString()));
    
    m_quad = new glowutils::ScreenAlignedQuad(program);
}

SimplePostProcPass::SimplePostProcPass(
    const QString & fragmentShaderSource,
    GLenum outputTextureFormat,
    GLenum outputTextureFiltering)
:   m_textureFormat(outputTextureFormat)
{
    initializeFbo(outputTextureFiltering);

    m_quad = new glowutils::ScreenAlignedQuad(
        glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, fragmentShaderSource.toStdString()));
}

SimplePostProcPass::~SimplePostProcPass()
{
}

void SimplePostProcPass::initializeFbo(GLenum outputTextureFiltering)
{
    m_outputTexture = Util::create2DTexture(outputTextureFiltering, GL_CLAMP_TO_EDGE);
    
    m_fbo = new glow::FrameBufferObject();
    m_fbo->setDrawBuffer(GL_COLOR_ATTACHMENT0);
    m_fbo->attachTexture2D(GL_COLOR_ATTACHMENT0, m_outputTexture);
}

void SimplePostProcPass::bindTextures()
{
    int indexOfTextureImageUnit = 0;
    for (glow::Texture * texture : m_inputTextures.values()) {
        texture->bindActive(GL_TEXTURE0 + indexOfTextureImageUnit);
        ++indexOfTextureImageUnit;
    }
}

void SimplePostProcPass::apply()
{
    bindTextures();

    m_fbo->bind();
    m_quad->draw();
    m_fbo->unbind();
}

void SimplePostProcPass::setInputTextures(const QMap<QString, glow::Texture *> & input)
{
    m_inputTextures = input;

    int indexOfTextureImageUnit = 0;
    for (QString uniformName : m_inputTextures.uniqueKeys()) {
        setUniform(uniformName, indexOfTextureImageUnit);
        ++indexOfTextureImageUnit;
    }
}

glow::Texture * SimplePostProcPass::outputTexture()
{
    return m_outputTexture;
}

void SimplePostProcPass::resize(int width, int height)
{
    m_outputTexture->image2D(0, m_textureFormat, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
}
