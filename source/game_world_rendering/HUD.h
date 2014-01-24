#pragma once

#include "StringDrawer.h"

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
    StringDrawer m_stringDrawer;

    const RenderCamera & m_camera;
    const GameWorldRenderer & m_gameWorldRenderer;
};
