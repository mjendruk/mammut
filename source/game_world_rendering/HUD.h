#pragma once

#include "StringDrawer.h"

class Mammut;
class RenderCamera;
class GameWorldRenderer;

class HUD
{
public:
    HUD(const Mammut & mammut,
        const RenderCamera & camera,
        const GameWorldRenderer & renderer);
    ~HUD();

    void initialize();
    void paint();

protected:
    StringDrawer m_stringDrawer;

    const Mammut & m_mammut;
    const RenderCamera & m_camera;
    const GameWorldRenderer & m_gameWorldRenderer;
};
