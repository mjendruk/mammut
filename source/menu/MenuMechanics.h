#pragma once

#include <Mechanics.h>
#include "MenuRenderer.h"

class QKeyEvent;
class Menu;

class MenuMechanics : public Mechanics
{
public:
    MenuMechanics(Menu * menu);
    virtual ~MenuMechanics();

    void update(float seconds) override;
    Renderer * renderer() override;

    void keyPressed(QKeyEvent * event) override;
    void keyReleased(QKeyEvent * event) override;

    const Menu & menu() const;

protected:
    Menu * m_menu;
    MenuRenderer m_renderer;
    
};
