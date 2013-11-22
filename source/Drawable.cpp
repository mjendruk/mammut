#include "Drawable.h"

#include <cassert>

#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <glow/VertexAttributeBinding.h>

Drawable::Drawable()
:   m_cube(nullptr)
{

}

Drawable::~Drawable()
{
}

void Drawable::initialize()
{
    m_cube = new glow::UnitCube();
}

void Drawable::draw()
{
    assert(m_cube);

    m_cube->draw();
}