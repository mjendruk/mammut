#pragma once

#include <QString>
#include <QVector>

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

    void paint(const QString & text,
               const glm::mat4 & modelMatrix,
               Alignment alignment = kAlignLeft,
               const glm::vec3 color = glm::vec3(1.0f));

protected:
    bool initialize();
    bool initializeProgram();
    bool initializeTexture();
    
    static void prepareTransforms(const QList<CharacterSpecifics *> characterSpecificsList,
                                  const glm::mat4 & modelMatrix,
                                  Alignment alignment,
                                  QVector<glm::mat4> & vertexTransforms,
                                  QVector<glm::mat4> & textureCoordTransforms);
    
    static glm::mat4 alignmentTransform(const QList<CharacterSpecifics *> & list,
                                        Alignment alignment);

protected:
    static const float s_textureSize;
    
    glow::ref_ptr<glow::Program> m_program;
    glow::ref_ptr<glow::Texture> m_characterAtlas;

    CharacterDrawable m_drawable;
    StringComposer m_stringComposer;

};
