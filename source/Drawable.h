
#pragma once

#include <memory>

#include <glow/ref_ptr.h>
#include <glow/Array.h>
#include <glowutils/UnitCube.h>

class Drawable
{
public:
    Drawable();
    ~Drawable();

    void initialize();
    void draw();

protected:
    glow::ref_ptr<glow::UnitCube> m_cube;

};
