#include "MenuRenderer.h"

#include <cassert>
#include <algorithm>

#include <glm/gtx/transform.hpp>

#include <glow/FrameBufferObject.h>

#include <logic/menu/Menu.h>
#include <logic/menu/items/MenuButton.h>
#include <logic/menu/items/MenuLogo.h>
#include <logic/menu/items/HighscoreListItem.h>
#include <logic/menu/items/MenuInput.h>
#include <logic/menu/items/MenuText.h>

#include <logic/highscore/HighscoreEntry.h>

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
    
    m_textRenderer.paint(button->label(), 
                         m_translation,
                         TextRenderer::kAlignCenter,
                         color);

    m_translation *= glm::translate(0.0f, - TextRenderer::s_lineHeight * 1.5f, 0.0f);
}

void MenuRenderer::render(const MenuLogo * logo)
{
    const glm::vec3 color(0.6f, 0.13f, 0.02f);
    const glm::mat4 scale = glm::scale(glm::vec3(2.0f));
    
    m_textRenderer.paint("Mammut",
                         m_translation * scale,
                         TextRenderer::kAlignCenter,
                         color);
    
    m_translation *= glm::translate(0.0f, - TextRenderer::s_lineHeight * 2.0f * 1.5f, 0.0f);
}

void MenuRenderer::render(const HighscoreListItem * item)
{
    const float namePosition = -0.6f;
    const float scorePosition = 0.3f;
    const glm::vec3 titleColor(0.6f, 0.13f, 0.02f);
    const glm::mat4 entryScale = glm::scale(glm::vec3(0.7f));
    
    m_textRenderer.paint("Name",
                         m_translation * glm::translate(namePosition, 0.0f, 0.0f),
                         TextRenderer::kAlignLeft,
                         titleColor);
    
    m_textRenderer.paint("Score",
                         m_translation * glm::translate(scorePosition, 0.0f, 0.0f),
                         TextRenderer::kAlignLeft,
                         titleColor);
    
    m_translation *= glm::translate(0.0f, - TextRenderer::s_lineHeight * 1.1f, 0.0f);
    
    for (const HighscoreEntry & entry : item->scores())
    {
        m_textRenderer.paint(entry.name(), m_translation * glm::translate(namePosition, 0.0f, 0.0f) * entryScale);
        m_textRenderer.paint(QString::number(entry.score()), m_translation * glm::translate(scorePosition, 0.0f, 0.0f) * entryScale);
        
        m_translation *= glm::translate(0.0f, - TextRenderer::s_lineHeight * 0.8f, 0.0f);
    }
    
    m_translation *= glm::translate(0.0f, - TextRenderer::s_lineHeight * 0.2f, 0.0f);
}

void MenuRenderer::render(const MenuInput * input)
{
    const glm::vec3 color = m_menu->isFocusedItem(input) ? glm::vec3(0, 0.65f, 0.65f) : glm::vec3(1.0f);
    
    m_textRenderer.paint(input->label() + ": ",
                         m_translation,
                         TextRenderer::kAlignRight,
                         color);
    
    m_textRenderer.paint(input->text() + "_",
                         m_translation,
                         TextRenderer::kAlignLeft);
    
    m_translation *= glm::translate(0.0f, - TextRenderer::s_lineHeight * 1.5f, 0.0f);
}

void MenuRenderer::render(const MenuText * text)
{
    const glm::vec3 color(0.6f, 0.13f, 0.02f);
    
    m_textRenderer.paint(text->text(), 
                         m_translation,
                         TextRenderer::kAlignCenter,
                         color);

    m_translation *= glm::translate(0.0f, - TextRenderer::s_lineHeight * 1.5f, 0.0f);
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
