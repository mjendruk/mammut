#pragma once

class AbstractBackground
{
public:
    virtual ~AbstractBackground() = default;

    virtual void render(float devicePixelRatio) = 0;
};
