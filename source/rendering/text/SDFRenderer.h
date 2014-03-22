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

class SDFRenderer
{
public:
    enum Alignment { kAlignLeft, kAlignCenter, kAlignRight };
    
    SDFRenderer(QString rawFile, QString specificsFile, float lineHeight, float textureSize);
    virtual ~SDFRenderer();

    void paint(const QList<CharacterSpecifics *> & characterSpecificsList,
               const glm::mat4 & modelMatrix,
               Alignment alignment = kAlignLeft,
               const glm::vec3 color = glm::vec3(1.0f));

    bool isValid() const;
    float lineHeight() const;

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
    glow::ref_ptr<glow::Program> m_program;
    glow::ref_ptr<glow::Texture> m_characterAtlas;
    glow::ref_ptr<glow::Texture> m_lightingTexture;

    CharacterDrawable m_drawable;
    StringComposer m_stringComposer;

    bool m_valid;

    QString m_rawFile;
    QString m_specificsFile;
    float m_lineHeight;
    float m_textureSize;
};
