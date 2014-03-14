#pragma once

#include <QMap>

#include <GL/glew.h>

#include <glow/ref_ptr.h>
#include <glowutils/ScreenAlignedQuad.h>

#include "AbstractPostProcPass.h"


class QString;

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

class SimplePostProcPass : public AbstractPostProcPass
{
public:
    SimplePostProcPass(const QString & vertexShaderSource,
                       const QString & fragmentShaderSource,
                       GLenum outputTextureFormat,
                       GLenum outputTextureFiltering = GL_NEAREST);
    
    SimplePostProcPass(const QString & fragmentShaderSource,
                       GLenum outputTextureFormat,
                       GLenum outputTextureFiltering = GL_NEAREST);
    
    virtual ~SimplePostProcPass();

    void apply() override;
    void resize(int width, int height) override;
    void setInputTextures(const QMap<QString, glow::Texture *> & input) override;
    glow::Texture * outputTexture() override;

    template<typename T>
    void setUniform(const QString & name, const T & value);
    
protected:
    void initializeFbo(GLenum outputTextureFiltering);
    void bindTextures();

protected:
    glow::ref_ptr<glow::FrameBufferObject> m_fbo;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_quad;

    QMap<QString, glow::Texture *> m_inputTextures;
    glow::ref_ptr<glow::Texture> m_outputTexture;

    GLenum m_textureFormat;
};

#include "SimplePostProcPass.hpp"
