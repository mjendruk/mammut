#pragma once

#include "AbstractBackground.h"

class BlankBackground : public AbstractBackground
{
public:
    BlankBackground();
    virtual ~BlankBackground();

    void resize(int width, int height, float devicePixelRatio);
    void render(float devicePixelRatio);

};
