#pragma once

#include <rendering/text/TextRenderer.h>
#include <rendering/text/SymbolRenderer.h>

class RenderCamera;
class GameWorldRenderer;

class HUD
{
public:
    HUD(const RenderCamera & camera,
        const GameWorldRenderer & renderer);
    ~HUD();

    void paint(int velocity, int score, int collectedBoosts);

protected:
    TextRenderer m_textRenderer;
    SymbolRenderer m_symbolRenderer;

    const RenderCamera & m_camera;
    const GameWorldRenderer & m_gameWorldRenderer;
};
