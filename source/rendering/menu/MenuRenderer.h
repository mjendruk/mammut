#pragma once

#include <glm/glm.hpp>

#include <rendering/Renderer.h>
#include <rendering/text/TextRenderer.h>

class Menu;
class MenuButton;
class MenuLogo;
class HighscoreListItem;
class CreditsItem;
class HowtoListItem;
class MenuInput;
class MenuText;
class AbstractBackground;

class MenuRenderer : public Renderer
{
public:
    MenuRenderer();
    virtual ~MenuRenderer();

    void resize(int width, int height) override;
    void render(glow::FrameBufferObject * fbo, float devicePixelRatio) override;

    void render(const MenuButton * button);
    void render(const MenuLogo * logo);
    void render(const HighscoreListItem * item);
    void render(const CreditsItem * item);
    void render(const HowtoListItem * item);
    void render(const MenuInput * input);
    void render(const MenuText * text);

    void setBackground(AbstractBackground * background);
    void setMenu(const Menu * menu);
    
protected:
    void moveTranslationDown(float lineHeightScale);

protected:
    static const glm::vec3 s_titleColor;
    static const glm::vec3 s_focusColor;
    static const glm::vec3 s_textColor;

    glm::ivec2 m_viewport;
    glm::mat4 m_translation;
    TextRenderer m_textRenderer;

    AbstractBackground * m_background;
    const Menu * m_menu;

};
