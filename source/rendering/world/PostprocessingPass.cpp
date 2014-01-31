#include "PostprocessingPass.h"

#include <glow/Program.h>
#include <glow/FrameBufferObject.h>
#include <glowutils/ScreenAlignedQuad.h>

#include "FileAssociatedShader.h"

PostprocessingPass::PostprocessingPass(const QString name)
:   glow::Referenced()
,   m_name(name)
,   m_program(nullptr)
,   m_output()
,   m_inputMapping()
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

void PostprocessingPass::initBeforeDraw(glow::FrameBufferObject & fbo) {
    fbo.setDrawBuffers( m_output.toStdVector() );
    int i = 0;
    for (QString key : m_inputMapping.keys()) {
        glActiveTexture(GL_TEXTURE0 + i);
        m_program->setUniform(key.toStdString(), i);
        fbo.bind(GL_TEXTURE0 + m_inputMapping.value(key));
        i++;
    }
    glDisable(GL_DEPTH_TEST);
    fbo.bind();
}


void PostprocessingPass::apply(glow::FrameBufferObject & fbo) {
    if (!m_program) {
        initializeProgram();
    }

    initBeforeDraw(fbo);

    m_program->use();
    m_quad->draw();
    m_program->release();
}

void PostprocessingPass::setInputMapping(const QMap<QString, int> & inputMapping) 
{
    m_inputMapping = inputMapping;
}

void PostprocessingPass::setOutput(const QVector<GLenum>& output) 
{
    m_output = output;
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

    glow::Shader* vertShader = FileAssociatedShader::getOrCreate(GL_FRAGMENT_SHADER, m_vertexShader, *m_program);
    glow::Shader* fragShader = FileAssociatedShader::getOrCreate(GL_FRAGMENT_SHADER, m_fragmentShader, *m_program);

    m_program->link();

    m_quad = new glowutils::ScreenAlignedQuad(m_program);

    CheckGLError();
}


void PostprocessingPass::resizeTextures(int width, int height)
{
    m_program->setUniform("viewport", glm::vec2(width, height));
    //resize local Textures
}