#pragma once

#include <QString>
#include <QMap>

#include <GL/glew.h>

#include <glow/ref_ptr.h>
#include <glow/Program.h>
#include <glow/Referenced.h>

#include <glowutils/ScreenAlignedQuad.h>

#include "AbstractPostProc.h"


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

struct TextureFormat
{
    GLint internalFormat;
    GLenum format;
    GLenum type;
};

class SimplePostProcPass : public AbstractPostProc
{
public:
    SimplePostProcPass(TextureFormat outputFormat, const QString fragmentShader, const QString vertexShader = QString("data/shaders/screenquad.vert"));
    virtual ~SimplePostProcPass();

    virtual void apply();
    virtual void resize(int width, int height);
    virtual void setInputTextures(const QMap<QString, glow::Texture*> & input);
    virtual glow::Texture* outputTexture();

    template<typename T>
    void setUniform(const QString name, const T& value);

protected:
    void initialize();
    void initializeProgram();
    void bindTextures();
    void unbindTextures();


protected:
    glow::ref_ptr<glow::FrameBufferObject> m_fbo;
    glow::ref_ptr<glow::Program> m_program;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_quad;

    QMap<QString, glow::Texture*> m_inputTextures;
    glow::ref_ptr<glow::Texture> m_outputTexture;

    QString m_fragmentShader;
    QString m_vertexShader;

    TextureFormat m_textureFormat;
};

template<typename T>
void SimplePostProcPass::setUniform(const QString name, const T & value) {
    m_program->setUniform(name.toStdString(), value);
}
