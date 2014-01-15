#include "CuboidDrawable.h"

#include <cassert>

#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <glow/VertexAttributeBinding.h>

CuboidDrawable::CuboidDrawable()
:   m_cube(nullptr)
{

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