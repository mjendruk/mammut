#pragma once

#include <QString>
#include <QVector>
#include <QMap>

#include <GL/glew.h>

#include <glow/ref_ptr.h>
#include <glow/Referenced.h>


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

class PostprocessingPass : public glow::Referenced
{
public:
    PostprocessingPass(const QString name);
    ~PostprocessingPass();

    template<typename T>
    void setUniform(const QString name, const T& value);

public:
    const QString name() const;

    void apply(glow::FrameBufferObject & frameBuffer);

    void setInputTextures(const QMap<QString, int> & input);
    void setOutput(const QVector<GLenum> & output);
    void setVertexShader(const QString output);
    void setFragmentShader(const QString output);

    void resizeTextures(int width, int height);

protected:
    void initializeProgram();

    void initBeforeDraw(glow::FrameBufferObject & frameBuffer);

protected:
    QString const m_name;
    glow::ref_ptr<glow::Program> m_program;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_quad;
    QMap<QString, int> m_inputTextures;
    QVector<GLenum> m_output;
    QString m_fragmentShader;
    QString m_vertexShader;
};

template<typename T>
void PostprocessingPass::setUniform(const QString name, const T& value) {
    if (!m_program) {
        initializeProgram();
    }
    m_program->setUniform(name.toStdString(), value);
}
