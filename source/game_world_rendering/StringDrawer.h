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
    enum Alignment { kAlignLeft, kAlignCenter, kAlignRight };
    
    StringDrawer();
    ~StringDrawer();

    bool initialize();
    void paint(const QString & text,
               const glm::mat4 & modelMatrix = glm::mat4(),
               Alignment alignment = kAlignLeft,
               const glm::vec3 color = glm::vec3(1.0f));

protected:
    bool initializeProgram();
    bool initializeTexture();
    
    glm::mat4 alignmentTransform(const QList<CharacterSpecifics *> & list,
                                 Alignment alignment) const;

protected:
    static const float s_textureSize;
    
    glow::ref_ptr<glow::Program> m_program;
    glow::ref_ptr<glow::Texture> m_characterAtlas;

    CharacterDrawable m_drawable;
    StringComposer m_stringComposer;

};
