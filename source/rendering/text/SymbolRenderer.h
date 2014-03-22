#pragma once

#include <QMap>

#include "SDFRenderer.h"


class SymbolRenderer : public SDFRenderer
{
public:
    SymbolRenderer();
    virtual ~SymbolRenderer();

    void paint(const QString & symbolName, 
        const int repeatSymbol,
        const glm::mat4 & modelMatrix,
        Alignment alignment = kAlignCenter,
        const glm::vec3 color = glm::vec3(1.0f));

protected:
    static const float s_lineHeight;
    static const float s_textureSize;
    static QMap<QString, int> s_idMapping;
    static bool s_idMapInitialized;
};
