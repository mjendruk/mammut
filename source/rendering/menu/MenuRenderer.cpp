#include "MenuRenderer.h"

#include <cassert>
#include <algorithm>

#include <glm/gtx/transform.hpp>

#include <glow/FrameBufferObject.h>

#include <logic/menu/Menu.h>
#include <logic/menu/items/MenuButton.h>
#include <logic/menu/items/MenuLogo.h>
#include <logic/menu/items/HighscoreListItem.h>
#include <logic/menu/items/CreditsListItem.h>
#include <logic/menu/items/MenuInput.h>
#include <logic/menu/items/MenuText.h>

#include <logic/highscore/HighscoreEntry.h>

#include "AbstractBackground.h"

const glm::vec3 MenuRenderer::s_titleColor = glm::vec3(0.6f, 0.13f, 0.02f);
const glm::vec3 MenuRenderer::s_focusColor = glm::vec3(0, 0.65f, 0.65f);
const glm::vec3 MenuRenderer::s_textColor = glm::vec3(1.f);

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
    const glm::vec3 color = m_menu->isFocusedItem(button) ? s_focusColor : s_textColor;
    
    m_textRenderer.paint(button->label(), 
                         m_translation,
                         TextRenderer::kAlignCenter,
                         color);

    moveTranslationDown(1.5f);
}

void MenuRenderer::render(const MenuLogo * logo)
{
    const float scale = 2.0f;
    
    m_textRenderer.paint("Mammut",
                         m_translation * glm::scale(glm::vec3(scale)),
                         TextRenderer::kAlignCenter,
                         s_titleColor);
    
    moveTranslationDown(scale * 1.5f);
}

void MenuRenderer::render(const HighscoreListItem * item)
{
    const float namePosition = -0.6f;
    const float scorePosition = 0.3f;
    const glm::mat4 entryScale = glm::scale(glm::vec3(0.7f));
    
    m_textRenderer.paint("Name",
                         m_translation * glm::translate(namePosition, 0.0f, 0.0f),
                         TextRenderer::kAlignLeft,
                         s_titleColor);
    
    m_textRenderer.paint("Score",
                         m_translation * glm::translate(scorePosition, 0.0f, 0.0f),
                         TextRenderer::kAlignLeft,
                         s_titleColor);
    
    moveTranslationDown(1.1f);
    
    for (const HighscoreEntry & entry : item->scores())
    {
        m_textRenderer.paint(entry.name(), m_translation * glm::translate(namePosition, 0.0f, 0.0f) * entryScale);
        m_textRenderer.paint(QString::number(entry.score()), m_translation * glm::translate(scorePosition, 0.0f, 0.0f) * entryScale);
        
        moveTranslationDown(0.8f);
    }
    
    moveTranslationDown(0.2f);
}

void MenuRenderer::render(const CreditsListItem * item)
{
    const glm::vec3 nameStartPosition = glm::vec3(-0.6f, 0.0f, 0.f);
    const glm::vec3 namePositionOffset = -nameStartPosition;
    const glm::vec3 thanksToStartPosition = glm::vec3(-0.5f, 0.0f, 0.f);
    const glm::vec3 thanksToPositionOffset = -thanksToStartPosition;

    glm::vec3 currentRowPosition = nameStartPosition;
    const glm::vec3 thanksHeadingPosition = glm::vec3(0.f);
    const glm::vec3 thanksToPosition = glm::vec3(0.3f, 0.f, 0.f);

    const glm::mat4 forenameScale = glm::scale(glm::vec3(0.9f));
    const glm::mat4 surnameScale = glm::scale(glm::vec3(0.6f));
    const glm::mat4 thanksHeadingScale = glm::scale(glm::vec3(0.8f));
    const glm::mat4 thanksToScale = glm::scale(glm::vec3(0.5f));

    const int maxIndexPerRow = 2;

    moveTranslationDown(0.5f);

    for (const QString & name : item->creditsForenames())
    {
        m_textRenderer.paint(name, m_translation * glm::translate(currentRowPosition) * surnameScale, TextRenderer::kAlignCenter, s_textColor);
        currentRowPosition += namePositionOffset;
    }

    moveTranslationDown(0.85f);
    currentRowPosition = nameStartPosition;

    for (const QString & name : item->creditsNicknames())
    {
        m_textRenderer.paint(name, m_translation * glm::translate(currentRowPosition) * forenameScale, TextRenderer::kAlignCenter, s_focusColor);
        currentRowPosition += namePositionOffset;
    }

     moveTranslationDown(0.95f);
     currentRowPosition = nameStartPosition;

    for (const QString & name : item->creditsSurnames())
    {
        m_textRenderer.paint(name, m_translation * glm::translate(currentRowPosition) * surnameScale, TextRenderer::kAlignCenter, s_textColor);
        currentRowPosition += namePositionOffset;
    }

    moveTranslationDown(1.6f);

    m_textRenderer.paint("Thanks to",
        m_translation * glm::translate(thanksHeadingPosition) * thanksHeadingScale,
        TextRenderer::kAlignCenter,
        s_titleColor);

    moveTranslationDown(1.1f);

    int index = 0;
    currentRowPosition = thanksToStartPosition;

    for (const QString & name : item->thanks())
    {
        if (index == maxIndexPerRow + 1) {
            currentRowPosition = thanksToStartPosition;
            moveTranslationDown(0.7f);
            index = 0;
        }

        m_textRenderer.paint(name, m_translation * glm::translate(currentRowPosition) * thanksToScale, TextRenderer::kAlignCenter);
        currentRowPosition += thanksToPositionOffset;
        ++index;
    }

    moveTranslationDown(1.5f);
}

void MenuRenderer::render(const MenuInput * input)
{
    const glm::vec3 color = m_menu->isFocusedItem(input) ? s_focusColor : s_textColor;
    
    m_textRenderer.paint(input->label() + ": ",
                         m_translation,
                         TextRenderer::kAlignRight,
                         color);
    
    m_textRenderer.paint(input->text() + "_",
                         m_translation,
                         TextRenderer::kAlignLeft);
    
    moveTranslationDown(1.5f);
}

void MenuRenderer::render(const MenuText * text)
{
    
    m_textRenderer.paint(text->text(), 
                         m_translation,
                         TextRenderer::kAlignCenter,
                         s_titleColor);

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
    m_translation *= glm::translate(0.0f, - TextRenderer::s_lineHeight * lineHeightScale, 0.0f);
}
