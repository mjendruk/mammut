#pragma once

class AbstractBackground
{
public:
    virtual ~AbstractBackground();

    virtual void resize(int width, int height, float devicePixelRatio) = 0;
    virtual void render(float devicePixelRatio) = 0;

};
