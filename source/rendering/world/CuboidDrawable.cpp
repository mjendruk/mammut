#include "CuboidDrawable.h"

#include <cassert>

CuboidDrawable::CuboidDrawable()
:   m_cube(nullptr)
{
    initialize();
}

CuboidDrawable::~CuboidDrawable()
{
}

void CuboidDrawable::initialize()
{
    m_cube = new glowutils::UnitCube();
}

void CuboidDrawable::draw()
{
    assert(m_cube);

    m_cube->draw();
}