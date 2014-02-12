#pragma once

#include "MenuItem.h"

class MenuLogo : public MenuItem
{
    Q_OBJECT

public:
    MenuLogo();
    virtual ~MenuLogo();

    void accept(MenuRenderer * renderer) override;
    void keyPressed(QKeyEvent * event) override;

    bool isFocusable() const override;

};
