#pragma once

#include <memory>

#include <glow/ref_ptr.h>
#include <glowutils/UnitCube.h>

#include "IDrawable.h"

class CuboidDrawable : public IDrawable
{
public:
    CuboidDrawable();
    ~CuboidDrawable();

    void initialize() override;
    void draw() override;

protected:
    glow::ref_ptr<glowutils::UnitCube> m_cube;

};
