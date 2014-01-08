#pragma once

#include <QString>
#include <glm/glm.hpp>
#include <glow/ref_ptr.h>

#include "CharacterDrawable.h"
#include "StringComposer.h"

namespace glow
{
    class Program;
    class Texture;
}

class StringDrawer
{
public:
    StringDrawer();
    ~StringDrawer();

    bool initialize();
    void paint(const QString & string, const glm::mat4 & modelMatrix = glm::mat4());

protected:
    bool initializeProgram();
    bool initializeTexture();

protected:
    static const float s_textureSize;
    
    glow::ref_ptr<glow::Program> m_program;
    glow::ref_ptr<glow::Texture> m_characterAtlas;

    CharacterDrawable m_drawable;
    StringComposer m_stringComposer;

};
