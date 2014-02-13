#include "SimplePostProcPass.h"

#include <QVector>

#include <glow/Program.h>
#include <glow/Texture.h>
#include <glow/FrameBufferObject.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/File.h>

#include <Util.h>


SimplePostProcPass::SimplePostProcPass(TextureFormat format)
:   m_vertexShader("data/shaders/screenquad.vert")
,   m_inputTextures(QMap<QString, glow::Texture*>())
,   m_textureFormat(TextureFormat(format))
{
    initialize();
}

SimplePostProcPass::~SimplePostProcPass()
{
}

void SimplePostProcPass::initialize()
{
    m_outputTexture = Util::create2DTexture();
    m_fbo = new glow::FrameBufferObject();
    m_fbo->setDrawBuffer(GL_COLOR_ATTACHMENT0);
    m_fbo->attachTexture2D(GL_COLOR_ATTACHMENT0, m_outputTexture); 
}

void SimplePostProcPass::bindTextures()
{
    //set input Textures as uniforms    
    int indexOfTextureImageUnit = 0;
    for (QString uniformName : m_inputTextures.uniqueKeys()) {
        glow::Texture * texture = m_inputTextures.value(uniformName);
        texture->bind(GL_TEXTURE0 + indexOfTextureImageUnit);
        m_program->setUniform(uniformName.toStdString(), indexOfTextureImageUnit);

        ++indexOfTextureImageUnit;
    }
}

void SimplePostProcPass::unbindTextures()
{
    for (glow::Texture * texture : m_inputTextures.values())
        texture->unbind();
}

void SimplePostProcPass::apply()
{
    if (!m_program)
       initializeProgram();

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    bindTextures();

    m_fbo->bind();
    m_quad->draw();
    m_fbo->unbind();

    unbindTextures();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}

void SimplePostProcPass::setInputTextures(const QMap<QString, glow::Texture*> input)
{
    m_inputTextures = QMap<QString, glow::Texture*>(input);
}

glow::Texture* SimplePostProcPass::outputTexture()
{
    return m_outputTexture;
}

void SimplePostProcPass::setVertexShader(QString vertexShader)
{
    m_vertexShader = vertexShader;
}

void SimplePostProcPass::setFragmentShader(QString fragmentShader)
{
    m_fragmentShader = fragmentShader;
}

void SimplePostProcPass::initializeProgram()
{
    m_program = new glow::Program();

    glow::Shader* vertShader = glowutils::createShaderFromFile(GL_VERTEX_SHADER, m_vertexShader.toStdString());
    glow::Shader* fragShader = glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, m_fragmentShader.toStdString());
    
    m_program->attach(vertShader, fragShader);
    m_program->link();

    m_quad = new glowutils::ScreenAlignedQuad(m_program);

    CheckGLError();
}


void SimplePostProcPass::resize(int width, int height)
{
    m_outputTexture->image2D(0, m_textureFormat.internalFormat, width, height, 0, m_textureFormat.format, m_textureFormat.type, nullptr);
}
