#pragma once

class DrawableInterface
{
public:
    virtual ~DrawableInterface() {}

    virtual void draw() = 0;

protected:
    virtual void initialize() = 0;
};
