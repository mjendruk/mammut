#pragma once

#include <glm/glm.hpp>

#include <rendering/Renderer.h>
#include <rendering/text/StringDrawer.h>

class Menu;
class MenuButton;
class MenuLogo;
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

    void setBackground(AbstractBackground * background);
    void setMenu(const Menu * menu);
    
protected:
    glm::ivec2 m_viewport;
    glm::mat4 m_translation;
    StringDrawer m_stringDrawer;

    AbstractBackground * m_background;
    const Menu * m_menu;

};
