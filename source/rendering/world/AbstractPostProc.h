#pragma once

#include <QString>
#include <QMap>

#include <GL/glew.h>


namespace glow
{
    class FrameBufferObject;
    class Texture;
}

class AbstractPostProc
{
public:
    virtual ~AbstractPostProc() = 0;

    virtual void apply() = 0;

    virtual void setInputTextures(const QMap<QString, glow::Texture*> & input) = 0;
    virtual glow::Texture* outputTexture() = 0;

    virtual void resize(int width, int height){}
};
