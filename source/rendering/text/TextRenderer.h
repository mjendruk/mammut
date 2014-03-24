#pragma once

#include "SDFRenderer.h"


class TextRenderer : public SDFRenderer
{
public:    
    TextRenderer();

    void paint(const QString & text,
               const glm::mat4 & modelMatrix,
               Alignment alignment = kAlignLeft,
               const glm::vec3 color = glm::vec3(0.9f));

protected:
    static const float s_lineHeight;
    static const float s_textureSize;
};
