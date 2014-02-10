#pragma once

#include <QString>
#include <QMap>

#include <GL/glew.h>

#include <glow/ref_ptr.h>
#include <glow/Referenced.h>


namespace glow
{
    class FrameBufferObject;
    class Texture;
}

enum TextureImageUnits {
    TIU_Normal = 0,
    TIU_Color,
    TIU_Depth,
    TIU_Velocity,
    TIU_SSAO,
    TIU_MotionBlur,
    TIU_BufferCount // should always be last member
};

class PostProcInterface
{
public:
    virtual ~PostProcInterface(){}

    virtual void apply(glow::FrameBufferObject & frameBuffer) = 0;
    virtual void resize(int width, int height) = 0;

    virtual void setInputTextures(const QMap<QString, int> input) = 0;
    virtual void set2DTextureOutput(const QMap<GLenum, glow::Texture*> output) = 0;
};
