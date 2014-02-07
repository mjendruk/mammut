#pragma once

#include "MenuItem.h"

class HighscoreItem : public MenuItem
{
    Q_OBJECT

public:
    HighscoreItem();
    virtual ~HighscoreItem();

    void accept(MenuRenderer * renderer) override;
    void keyPressed(QKeyEvent * event) override;

    bool isFocusable() const override;

};
