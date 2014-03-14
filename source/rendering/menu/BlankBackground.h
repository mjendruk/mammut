#pragma once

#include "AbstractBackground.h"

class BlankBackground : public AbstractBackground
{
public:
    BlankBackground();
    virtual ~BlankBackground();

    void render(float devicePixelRatio) override;
};
