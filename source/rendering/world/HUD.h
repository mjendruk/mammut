#pragma once

#include <rendering/text/TextRenderer.h>
#include <rendering/text/SymbolRenderer.h>

class RenderCamera;
class GameWorldRenderer;

class HUD
{
public:
    HUD(const RenderCamera & camera);

    void paint(int velocity, int score, int collectedBoosts, int fps);

protected:
    TextRenderer m_textRenderer;
    SymbolRenderer m_symbolRenderer;

    const RenderCamera & m_camera;
    
protected:
    static const bool s_showFps;
    
};
