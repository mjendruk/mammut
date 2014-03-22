#pragma once

#include <QString>
#include <QVector>

#include <glm/glm.hpp>
#include <glow/ref_ptr.h>

#include <QMap>

#include "CharacterDrawable.h"
#include "StringComposer.h"

namespace glow
{
    class Program;
    class Texture;
}

class SymbolRenderer
{
public:
    enum Alignment { kAlignLeft, kAlignCenter, kAlignRight };
    
    static const float s_lineHeight;
    
    SymbolRenderer();
    ~SymbolRenderer();

    void paint(const QString & symbolName, 
        const int repeatSymbol,
        const glm::mat4 & modelMatrix,
        Alignment alignment = kAlignCenter,
        const glm::vec3 color = glm::vec3(1.0f));

    bool isValid() const;

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
    static QMap<QString, int> s_idMapping;
    static bool s_idMapInitialized;
    
    glow::ref_ptr<glow::Program> m_program;
    glow::ref_ptr<glow::Texture> m_characterAtlas;
    glow::ref_ptr<glow::Texture> m_lightingTexture;

    CharacterDrawable m_drawable;
    StringComposer m_stringComposer;

    bool m_valid;

};
