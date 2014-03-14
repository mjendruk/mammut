#pragma once

class AbstractBackground
{
public:
    virtual ~AbstractBackground();

    virtual void render(float devicePixelRatio) = 0;
};
