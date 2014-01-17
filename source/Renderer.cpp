#include "Renderer.h"

Renderer::Renderer()
:   m_initialized(false)
{
}

Renderer::~Renderer()
{
}

void Renderer::initialize()
{
    m_initialized = true;
}

bool Renderer::initialized() const
{
    return m_initialized;
}
