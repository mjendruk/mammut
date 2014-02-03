#include "PostprocessingPass.h"

#include <QDebug>

#include <glow/Program.h>
#include <glow/Texture.h>
#include <glow/FrameBufferObject.h>
#include <glowutils/ScreenAlignedQuad.h>
#include <glowutils/File.h>


PostprocessingPass::PostprocessingPass(const QString name)
:   glow::Referenced()
,   m_name(name)
,   m_vertexShader("data/screenquad.vert")
,   m_output2DInvalidated(true)
,   m_inputTextures(new QMap<QString, int>)
,   m_output2D(new QMap<GLenum, glow::Texture*>)
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
    //set input Textures as uniforms
    for (QString uniformName : m_inputTextures->keys()) {
        int textureImageUnit = (*m_inputTextures)[uniformName];
        m_program->setUniform(uniformName.toStdString(), textureImageUnit);
    }

    if (m_output2DInvalidated){
        if (!m_output2D->isEmpty())
            fbo.setDrawBuffers(m_output2D->keys().toVector().toStdVector());

        for (GLenum attachment : m_output2D->keys()){
            if (attachment > TIU_Depth)
                fbo.attachTexture2D(attachment, (*m_output2D)[attachment]);
        }

        m_output2DInvalidated = false;
    }
}

void PostprocessingPass::apply(glow::FrameBufferObject & fbo) 
{
    if (!m_program) {
        m_program = initializeProgram();
        m_quad = new glowutils::ScreenAlignedQuad(m_program);
    }

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

void PostprocessingPass::setInputTextures(const QMap<QString, int> & input) 
{
    m_inputTextures->clear();
    for (QString key : input.keys())
        m_inputTextures->insert(key, input.value(key));
}

void PostprocessingPass::set2DTextureOutput(const QMap<GLenum, glow::Texture*> & output)
{
    m_output2D->clear();
    for (GLenum key : output.keys())
        m_output2D->insert(key, output.value(key));

    m_output2DInvalidated = true;
}

void PostprocessingPass::setVertexShader(QString vertexShader) 
{
    m_vertexShader = vertexShader;
}

void PostprocessingPass::setFragmentShader(QString fragmentShader) 
{
    m_fragmentShader = fragmentShader;
}

glow::ref_ptr<glow::Program> PostprocessingPass::initializeProgram()
{
    glow::ref_ptr<glow::Program> program = new glow::Program();

    glow::Shader* vertShader = glowutils::createShaderFromFile(GL_VERTEX_SHADER, m_vertexShader.toStdString());
    glow::Shader* fragShader = glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, m_fragmentShader.toStdString());
    program->attach(vertShader, fragShader);

    program->link();

    CheckGLError();

    return program;
}


void PostprocessingPass::resize(int width, int height)
{
    // resize local Textures
    // set Viewport Uniforms:
    // m_program->setUniform("viewport", glm::vec2(width, height));
}