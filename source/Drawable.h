#pragma once

#include <memory>

#include <glow/ref_ptr.h>
#include <glowutils/UnitCube.h>

class Drawable
{
public:
    Drawable();
    ~Drawable();

    void initialize();
    void draw();

protected:
    glow::ref_ptr<glowutils::UnitCube> m_cube;

};
