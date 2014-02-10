#include "SimplePostProcPass.h"

#include <QDebug>

#include <glow/Program.h>
#include <glow/Texture.h>
#include <glow/FrameBufferObject.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/File.h>


SimplePostProcPass::SimplePostProcPass()
:   m_vertexShader("data/shaders/screenquad.vert")
,   m_output2DInvalidated(true)
,   m_inputTextures(QMap<QString, int>())
{
}

SimplePostProcPass::~SimplePostProcPass()
{
}

void SimplePostProcPass::initBeforeDraw(glow::FrameBufferObject & fbo)
{
    //set input Textures as uniforms    
    for (QString uniformName : m_inputTextures.uniqueKeys()) {
        int numberOfTextureImageUnit = m_inputTextures.value(uniformName);
        m_program->setUniform(uniformName.toStdString(), numberOfTextureImageUnit);
    }

    if (m_output2DInvalidated){
        if (!m_output2D.isEmpty())
            fbo.setDrawBuffers(m_output2D.keys().toVector().toStdVector());

        for (GLenum attachment : m_output2D.keys()){
            if (attachment > TIU_Velocity)
                fbo.attachTexture2D(attachment, m_output2D[attachment]);
        }

        m_output2DInvalidated = false;
    }
}

void SimplePostProcPass::apply(glow::FrameBufferObject & fbo)
{
    if (!m_program)
       initializeProgram();

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    initBeforeDraw(fbo);

    fbo.bind();
    m_quad->draw();
    fbo.unbind();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}

void SimplePostProcPass::setInputTextures(const QMap<QString, int> input)
{
    m_inputTextures = QMap<QString, int>(input);
}

void SimplePostProcPass::set2DTextureOutput(const QMap<GLenum, glow::Texture*> output)
{
    m_output2D = QMap<GLenum, glow::Texture*>(output);

    m_output2DInvalidated = true;
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
    // resize local Textures
    // set Viewport Uniforms:
    // m_program->setUniform("viewport", glm::vec2(width, height));
}