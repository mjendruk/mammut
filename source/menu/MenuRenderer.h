#pragma once

#include <glm/glm.hpp>

#include <Renderer.h>
#include <game_world_rendering/StringDrawer.h>

class MenuMechanics;

class MenuRenderer : public Renderer
{
public:
    MenuRenderer(const MenuMechanics & mechanics);
    virtual ~MenuRenderer();

    void initialize() override;
    void resize(int width, int height) override;
    void render(float devicePixelRatio) override;

protected:
    glm::ivec2 m_viewport;
    StringDrawer m_stringDrawer;

    const MenuMechanics & m_mechanics;

};
