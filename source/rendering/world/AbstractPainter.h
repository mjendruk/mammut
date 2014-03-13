#pragma once

#include <glm/glm.hpp>

class DrawableInterface;


class AbstractPainter
{
public:
    AbstractPainter();
    virtual ~AbstractPainter();

    virtual void paint(DrawableInterface & drawable, 
        const glm::mat4 & modelMatrix, 
        const glm::mat4 & prevModelMatrix,
        bool containsBoost) = 0;

protected:
    virtual void initialize() = 0;
};
