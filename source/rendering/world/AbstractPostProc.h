#pragma once

#include <QString>
#include <QMap>

#include <GL/glew.h>


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

class AbstractPostProc
{
public:
    virtual ~AbstractPostProc(){}

    virtual void apply(glow::FrameBufferObject & frameBuffer) = 0;
    virtual void setInputTextures(const QMap<QString, glow::Texture*> input) = 0;

    virtual void resize(int width, int height){}
};
