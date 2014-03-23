#include "SymbolRenderer.h"


const float SymbolRenderer::s_lineHeight = 0.11f;
const float SymbolRenderer::s_textureSize = 1024.0f;

QMap<QString, int> SymbolRenderer::s_idMapping;
bool SymbolRenderer::s_idMapInitialized = false;

SymbolRenderer::SymbolRenderer()
:   SDFRenderer("data/fonts/Symbols.raw", 
                "data/fonts/Symbols.txt", 
                s_lineHeight, 
                s_textureSize)
{
    if (!s_idMapInitialized) {
        s_idMapping.insert(QString("Lightning"), 1);
        s_idMapping.insert(QString("Mammut"), 2);
        s_idMapping.insert(QString("Stones"), 3);
        s_idMapInitialized = true;
    }
}

SymbolRenderer::~SymbolRenderer()
{
    
}

void SymbolRenderer::paint(
    const QString & symbolName,
    const int repeatSymbol,
    const glm::mat4 & modelMatrix,
    Alignment alignment,
    const glm::vec3 color)
{
    QList<CharacterSpecifics *> characterSpecificsList = m_stringComposer.characterSequence(s_idMapping.value(symbolName, -1), repeatSymbol);
    
    SDFRenderer::paint(characterSpecificsList, modelMatrix, alignment, color);
}

