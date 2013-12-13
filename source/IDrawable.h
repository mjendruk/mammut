#pragma once

#include <memory>

#include <glow/ref_ptr.h>
#include <glowutils/UnitCube.h>

class IDrawable
{
public:
    virtual ~IDrawable(){}

    virtual void initialize() = 0;
    virtual void draw() = 0;

};
