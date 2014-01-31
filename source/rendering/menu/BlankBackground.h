#pragma once

#include "AbstractBackground.h"

class BlankBackground : public AbstractBackground
{
public:
    BlankBackground();
    virtual ~BlankBackground();

    void resize(int width, int height);
    void render(float devicePixelRatio);

};
