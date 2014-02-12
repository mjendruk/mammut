#pragma once

#include <rendering/text/TextRenderer.h>

class Mammut;
class RenderCamera;
class GameWorldRenderer;

class HUD
{
public:
    HUD(const RenderCamera & camera,
        const GameWorldRenderer & renderer);
    ~HUD();

    void initialize();
    void paint(const Mammut & mammut);

protected:
    TextRenderer m_textRenderer;

    const RenderCamera & m_camera;
    const GameWorldRenderer & m_gameWorldRenderer;
};
