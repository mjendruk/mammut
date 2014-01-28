#pragma once

#include <glm/glm.hpp>

#include <rendering/Renderer.h>
#include <rendering/text/StringDrawer.h>

class Menu;
class MenuButton;

class MenuRenderer : public Renderer
{
public:
    MenuRenderer();
    virtual ~MenuRenderer();

    void initialize() override;
    void resize(int width, int height) override;
    void render(float devicePixelRatio) override;

    void renderButton(const MenuButton * button);

    void setMenu(const Menu * menu);

protected:
    glm::ivec2 m_viewport;
    glm::mat4 m_translation;
    StringDrawer m_stringDrawer;

    const Menu * m_menu;

};
