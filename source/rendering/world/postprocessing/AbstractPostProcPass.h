#pragma once

#include <QMap>


class QString;

namespace glow
{
    class Texture;
}

class AbstractPostProcPass
{
public:
    virtual ~AbstractPostProcPass() = default;

    virtual void apply() = 0;

    virtual void setInputTextures(const QMap<QString, glow::Texture *> & input) = 0;
    virtual glow::Texture * outputTexture() = 0;

    virtual void resize(int width, int height);
};
