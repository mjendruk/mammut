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
    
    for (MenuItem * menuItem : m_menu->menuItems())
        menuItem->accept(this);
    
    glEnable(GL_DEPTH_TEST);
    
    fbo->unbind();
}

void MenuRenderer::render(const MenuButton * button)
{
    const glm::vec3 color = m_menu->isFocusedItem(button) ? glm::vec3(0, 0.65f, 0.65f) : glm::vec3(1.0f);
    
    m_textRenderer.paint(button->label(), 
                         m_translation,
                         SDFRenderer::kAlignCenter,
                         color);

    moveTranslationDown(1.5f);
}

void MenuRenderer::render(const MenuLogo * logo)
{
    const glm::vec3 color(0.6f, 0.13f, 0.02f);
    const float scale = 2.0f;

    const glm::mat4 mammutMatrix = m_translation * glm::translate(-0.3f, -0.95f, 0.f) * glm::scale(glm::vec3(0.55f));
    const glm::mat4 stonesMatrix = m_translation * glm::translate(-0.90f, -0.9f, 0.f) * glm::scale(glm::vec3(0.55f));
    
    m_textRenderer.paint("Mammut",
                         m_translation * glm::scale(glm::vec3(scale)),
                         SDFRenderer::kAlignCenter,
                         color);
    
    m_symbolRenderer.paint("Mammut", 1, mammutMatrix, SDFRenderer::kAlignCenter, color);
    m_symbolRenderer.paint("Stones", 1, stonesMatrix, SDFRenderer::kAlignCenter, color);
    
    moveTranslationDown(scale * 1.5f);
}

void MenuRenderer::render(const HighscoreListItem * item)
{
    const float namePosition = -0.6f;
    const float scorePosition = 0.3f;
    const glm::vec3 titleColor(0.6f, 0.13f, 0.02f);
    const glm::mat4 entryScale = glm::scale(glm::vec3(0.7f));
    
    m_textRenderer.paint("Name",
                         m_translation * glm::translate(namePosition, 0.0f, 0.0f),
                         SDFRenderer::kAlignLeft,
                         titleColor);
    
    m_textRenderer.paint("Score",
                         m_translation * glm::translate(scorePosition, 0.0f, 0.0f),
                         SDFRenderer::kAlignLeft,
                         titleColor);
    
    moveTranslationDown(1.1f);
    
    for (const HighscoreEntry & entry : item->scores())
    {
        m_textRenderer.paint(entry.name(), m_translation * glm::translate(namePosition, 0.0f, 0.0f) * entryScale);
        m_textRenderer.paint(QString::number(entry.score()), m_translation * glm::translate(scorePosition, 0.0f, 0.0f) * entryScale);
        
        moveTranslationDown(0.8f);
    }
    
    moveTranslationDown(0.2f);
}

void MenuRenderer::render(const MenuInput * input)
{
    const glm::vec3 color = m_menu->isFocusedItem(input) ? glm::vec3(0, 0.65f, 0.65f) : glm::vec3(1.0f);
    
    m_textRenderer.paint(input->label() + ": ",
                         m_translation,
                         SDFRenderer::kAlignRight,
                         color);
    
    m_textRenderer.paint(input->text() + "_",
                         m_translation,
                         SDFRenderer::kAlignLeft);
    
    moveTranslationDown(1.5f);
}

void MenuRenderer::render(const MenuText * text)
{
    const glm::vec3 color(0.6f, 0.13f, 0.02f);
    
    m_textRenderer.paint(text->text(), 
                         m_translation,
                         SDFRenderer::kAlignCenter,
                         color);

    moveTranslationDown(1.5f);
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

void MenuRenderer::moveTranslationDown(float lineHeightScale)
{
    m_translation *= glm::translate(0.0f, -m_textRenderer.lineHeight() * lineHeightScale, 0.0f);
}
