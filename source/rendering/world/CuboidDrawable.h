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

    void initialize() override;
    void draw() override;

protected:
    glow::ref_ptr<glowutils::UnitCube> m_cube;

};
