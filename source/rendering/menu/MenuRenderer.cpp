#include "MenuRenderer.h"

#include <cassert>
#include <algorithm>

#include <glm/gtx/transform.hpp>

#include <glow/FrameBufferObject.h>

#include <logic/menu/MenuButton.h>
#include <logic/menu/MenuLogo.h>
#include <logic/menu/HighscoreItem.h>
#include <logic/menu/MenuInput.h>
#include <logic/menu/MenuText.h>
#include <logic/highscore/HighscoreEntry.h>
#include <logic/menu/Menu.h>

#include "AbstractBackground.h"

MenuRenderer::MenuRenderer()
:   m_background(nullptr)
,   m_menu(nullptr)
{
}

MenuRenderer::~MenuRenderer()
{
}

void MenuRenderer::resize(int width, int height)
{
    assert(m_background != nullptr);
    
    m_viewport = glm::ivec2(width, height);
    m_background->resize(width, height);
}

void MenuRenderer::render(glow::FrameBufferObject * fbo, float devicePixelRatio)
{
    assert(m_menu != nullptr);
    assert(m_background != nullptr);
    
    fbo->bind();
    
    glViewport(0.0f, 0.0f, 
               m_viewport.x * devicePixelRatio, 
               m_viewport.y * devicePixelRatio);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_background->render(devicePixelRatio);
    
    glDisable(GL_DEPTH_TEST);
    
    const float aspectRatio = m_viewport.x / std::max(static_cast<float>(m_viewport.y), 1.f);
    const glm::mat4 aspectRatioTransform = glm::scale(1.0f / aspectRatio, 1.0f, 1.0f);

    m_translation = glm::translate(0.0f, 0.85f, 0.0f) * aspectRatioTransform * glm::scale(1.5f, 1.5f, 1.0f);
    
    for (auto menuItem : m_menu->menuItems())
        menuItem->accept(this);
    
    glEnable(GL_DEPTH_TEST);
    
    fbo->unbind();
}

void MenuRenderer::render(const MenuButton * button)
{
    const glm::vec3 color = m_menu->isFocusedItem(button) ? glm::vec3(0, 0.65f, 0.65f) : glm::vec3(1.0f);
    
    m_stringDrawer.paint(button->label(), 
                         m_translation,
                         StringDrawer::kAlignCenter,
                         color);

    m_translation *= glm::translate(0.0f, - StringDrawer::s_lineHeight * 1.5f, 0.0f);
}

void MenuRenderer::render(const MenuLogo * logo)
{
    const glm::vec3 color(0.6f, 0.13f, 0.02f);
    const glm::mat4 scale = glm::scale(glm::vec3(2.0f));
    
    m_stringDrawer.paint("Mammut",
                         m_translation * scale,
                         StringDrawer::kAlignCenter,
                         color);
    
    m_translation *= glm::translate(0.0f, - StringDrawer::s_lineHeight * 2.0f * 1.5f, 0.0f);
}

void MenuRenderer::render(const HighscoreItem * list)
{
    const float namePosition = -0.6f;
    const float scorePosition = 0.3f;
    const glm::vec3 titleColor(0.6f, 0.13f, 0.02f);
    const glm::mat4 entryScale = glm::scale(glm::vec3(0.7f));
    
    m_stringDrawer.paint("Name",
                         m_translation * glm::translate(namePosition, 0.0f, 0.0f),
                         StringDrawer::kAlignLeft,
                         titleColor);
    
    m_stringDrawer.paint("Score",
                         m_translation * glm::translate(scorePosition, 0.0f, 0.0f),
                         StringDrawer::kAlignLeft,
                         titleColor);
    
    m_translation *= glm::translate(0.0f, - StringDrawer::s_lineHeight * 1.1f, 0.0f);
    
    for (const HighscoreEntry & entry : list->scores())
    {
        m_stringDrawer.paint(entry.name(), m_translation * glm::translate(namePosition, 0.0f, 0.0f) * entryScale);
        m_stringDrawer.paint(QString::number(entry.score()), m_translation * glm::translate(scorePosition, 0.0f, 0.0f) * entryScale);
        
        m_translation *= glm::translate(0.0f, - StringDrawer::s_lineHeight * 0.8f, 0.0f);
    }
    
    m_translation *= glm::translate(0.0f, - StringDrawer::s_lineHeight * 0.2f, 0.0f);
}

void MenuRenderer::render(const MenuInput * input)
{
    const glm::vec3 color = m_menu->isFocusedItem(input) ? glm::vec3(0, 0.65f, 0.65f) : glm::vec3(1.0f);
    
    m_stringDrawer.paint(input->label() + ": ",
                         m_translation,
                         StringDrawer::kAlignRight,
                         color);
    
    m_stringDrawer.paint(input->text() + "_",
                         m_translation,
                         StringDrawer::kAlignLeft);
    
    m_translation *= glm::translate(0.0f, - StringDrawer::s_lineHeight * 1.5f, 0.0f);
}

void MenuRenderer::render(const MenuText * text)
{
    const glm::vec3 color(0.6f, 0.13f, 0.02f);
    
    m_stringDrawer.paint(text->text(), 
                         m_translation,
                         StringDrawer::kAlignCenter,
                         color);

    m_translation *= glm::translate(0.0f, - StringDrawer::s_lineHeight * 1.5f, 0.0f);
}

void MenuRenderer::setBackground(AbstractBackground * background)
{
    assert(background != nullptr);
    m_background = background;
}

void MenuRenderer::setMenu(const Menu * menu)
{
    assert(menu != nullptr);
    m_menu = menu;
}
