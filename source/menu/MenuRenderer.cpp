#include "MenuRenderer.h"

#include <algorithm>
#include <glm/gtx/transform.hpp>
#include "MenuMechanics.h"
#include "MenuItem.h"

MenuRenderer::MenuRenderer(const MenuMechanics & mechanics)
:   m_mechanics(mechanics)
{
}

MenuRenderer::~MenuRenderer()
{
}

void MenuRenderer::initialize()
{
    Renderer::initialize();

    m_stringDrawer.initialize();
}

void MenuRenderer::resize(int width, int height)
{
    m_viewport = glm::ivec2(width, height);
}

void MenuRenderer::render(float devicePixelRatio)
{
    glViewport(0.0f, 0.0f, 
               m_viewport.x * devicePixelRatio, 
               m_viewport.y * devicePixelRatio);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    
    const float aspectRatio = m_viewport.x / std::max(static_cast<float>(m_viewport.y), 1.f);
    const glm::mat4 aspectRatioTransform = glm::scale(1.0f / aspectRatio, 1.0f, 1.0f);

    glm::mat4 translation = glm::scale(1.5f, 1.5f, 0.0f) * glm::translate(0.0f, 0.0f, 0.0f);
    for (auto menuItem : m_mechanics.menuItems())
    {
        glm::vec3 color = m_mechanics.isSelectedMenuItem(menuItem) ? glm::vec3(0, 0.65f, 0.65f) : glm::vec3(1.0f);
        
        translation *= glm::translate(0.0f, -0.13f, 0.0f);

        m_stringDrawer.paint(menuItem->label(), 
                             aspectRatioTransform * translation,
                             StringDrawer::kAlignCenter,
                             color);
    }
    
    glEnable(GL_DEPTH_TEST);
}

