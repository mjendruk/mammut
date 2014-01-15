#pragma once

#include "StringDrawer.h"

class Mammut;
class RenderCamera;

class HUD
{
public:
    HUD(const Mammut & mammut,
        const RenderCamera & camera);
    ~HUD();

    void initialize();
    void paint();

protected:
    StringDrawer m_stringDrawer;

    const Mammut & m_mammut;
    const RenderCamera & m_camera;
};
