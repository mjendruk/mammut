#include "MenuRenderer.h"

#include <cassert>
#include <algorithm>
#include <glm/gtx/transform.hpp>
#include "MenuButton.h"
#include "Menu.h"

MenuRenderer::MenuRenderer(const Menu * menu)
:   m_menu(menu)
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

    m_translation = aspectRatioTransform * glm::scale(1.5f, 1.5f, 0.0f) * glm::translate(0.0f, 0.0f, 0.0f);
    
    for (auto menuItem : m_menu->menuItems())
        menuItem->accept(this);
    
    glEnable(GL_DEPTH_TEST);
}

void MenuRenderer::renderButton(const MenuButton * button)
{
    glm::vec3 color = m_menu->isFocusedItem(button) ? glm::vec3(0, 0.65f, 0.65f) : glm::vec3(1.0f);

    m_stringDrawer.paint(button->label(), 
                         m_translation,
                         StringDrawer::kAlignCenter,
                         color);

    m_translation *= glm::translate(0.0f, -0.13f, 0.0f);
}

void MenuRenderer::setMenu(const Menu * menu)
{
    assert(menu != nullptr);
    m_menu = menu;
}

