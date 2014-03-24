#include "TextRenderer.h"


const float TextRenderer::s_lineHeight = 0.11f;
const float TextRenderer::s_textureSize = 1024.0f;

TextRenderer::TextRenderer()
:   SDFRenderer("data/fonts/P22UndergroundPro-Medium.1024.1024.r.ub.raw", 
                "data/fonts/P22UndergroundPro-Medium.txt",
                s_lineHeight, s_textureSize)
{
}

void TextRenderer::paint(
    const QString & text,
    const glm::mat4 & modelMatrix,
    Alignment alignment, 
    const glm::vec3 color)
{
    QList<CharacterSpecifics *> characterSpecificsList = m_stringComposer.characterSequence(text);

    SDFRenderer::paint(characterSpecificsList, modelMatrix, alignment, color);
}
