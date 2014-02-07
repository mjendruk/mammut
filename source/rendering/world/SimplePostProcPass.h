#pragma once

#include <QString>
#include <QMap>

#include <GL/glew.h>

#include <glow/ref_ptr.h>
#include <glow/Referenced.h>

#include "PostProcInterface.h"


namespace glow
{
    class Program;
    class FrameBufferObject;
    class Texture;
}

namespace glowutils
{
    class ScreenAlignedQuad;
}

class SimplePostProcPass : public PostProcInterface
{
public:
    SimplePostProcPass();
    virtual ~SimplePostProcPass();

    virtual void apply(glow::FrameBufferObject & frameBuffer);
    virtual void resize(int width, int height);
    virtual void setInputTextures(const QMap<QString, int> input);
    virtual void set2DTextureOutput(const QMap<GLenum, glow::Texture*> output);
    
    void setVertexShader(const QString output);
    void setFragmentShader(const QString output);

    template<typename T>
    void setUniform(const QString name, const T& value);

protected:
    void initializeProgram();
    void initBeforeDraw(glow::FrameBufferObject & frameBuffer);

protected:
    QString const m_name;
    glow::ref_ptr<glow::Program> m_program;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_quad;
    QMap<QString, int> m_inputTextures;
    QMap<GLenum, glow::Texture*> m_output2D;
    QString m_fragmentShader;
    QString m_vertexShader;

    bool m_output2DInvalidated;
};

template<typename T>
void SimplePostProcPass::setUniform(const QString name, const T& value) {
    if (!m_program) {
        initializeProgram();
        m_quad = new glowutils::ScreenAlignedQuad(m_program);
    }
    m_program->setUniform(name.toStdString(), value);
}
