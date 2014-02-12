#pragma once

#include <memory>

#include <glow/ref_ptr.h>
#include <glowutils/UnitCube.h>

#include "DrawableInterface.h"

class CuboidDrawable : public DrawableInterface
{
public:
    CuboidDrawable();
    ~CuboidDrawable();

    void draw() override;

protected:
    void initialize() override;
    glow::ref_ptr<glowutils::UnitCube> m_cube;

};
