#include "PostprocessingPass.h"

#include <QDebug>

#include <glow/Program.h>
#include <glow/Texture.h>
#include <glow/FrameBufferObject.h>
#include <glowutils/ScreenAlignedQuad.h>

#include "FileAssociatedShader.h"
#include "GameWorldRenderer.h"

PostprocessingPass::PostprocessingPass(const QString name)
:   glow::Referenced()
,   m_name(name)
,   m_program(nullptr)
,   m_output2D()
,   m_inputTextures()
,   m_fragmentShader("")
,   m_vertexShader("data/screenquad.vert")
{
}

PostprocessingPass::~PostprocessingPass()
{
}

const QString PostprocessingPass::name() const
{
    return m_name;
}

void PostprocessingPass::initBeforeDraw(glow::FrameBufferObject & fbo) 
{
    if (!m_output2D.isEmpty())
        fbo.setDrawBuffers(m_output2D.keys().toVector().toStdVector());

    for (GLenum attachment : m_output2D.keys())
        fbo.attachTexture2D(attachment, m_output2D.value(attachment));

    //set input Textures as uniforms
    for (QString uniformName : m_inputTextures.keys()) {
        int textureImageUnit = m_inputTextures.value(uniformName);
        m_program->setUniform(uniformName.toStdString(), textureImageUnit);
    }

    glDisable(GL_DEPTH_TEST);
}


void PostprocessingPass::apply(glow::FrameBufferObject & fbo) 
{
    if (!m_program) {
        initializeProgram();
    }

    initBeforeDraw(fbo);

    fbo.bind();
    m_quad->draw();
    fbo.unbind();
}

void PostprocessingPass::setInputTextures(const QMap<QString, int> & input) 
{
    m_inputTextures = input;
}

void PostprocessingPass::set2DTextureOutput(const QMap<GLenum, glow::Texture*> & output)
{
    m_output2D = output;
}

void PostprocessingPass::setVertexShader(QString vertexShader) 
{
    m_vertexShader = vertexShader;
}

void PostprocessingPass::setFragmentShader(QString fragmentShader) 
{
    m_fragmentShader = fragmentShader;
}

void PostprocessingPass::initializeProgram() 
{
    m_program = new glow::Program();

    // update glow -> use:
    // glowutils::createShaderFromFile(GL_VERTEX_SHADER, m_vertexShader); // glowutils/global.h
    // m_program->attach(vertShader, fragShader);

    glow::Shader* vertShader = FileAssociatedShader::getOrCreate(GL_VERTEX_SHADER, m_vertexShader, *m_program);
    glow::Shader* fragShader = FileAssociatedShader::getOrCreate(GL_FRAGMENT_SHADER, m_fragmentShader, *m_program);

    m_program->link();

    m_quad = new glowutils::ScreenAlignedQuad(m_program);

    CheckGLError();
}


void PostprocessingPass::resize(int width, int height)
{
    //resize local Textures
    //set Viewport Uniforms
    // m_program->setUniform("viewport", glm::vec2(width, height));
}