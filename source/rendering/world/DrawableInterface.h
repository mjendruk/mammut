#pragma once

class DrawableInterface
{
public:
    virtual ~DrawableInterface() {}

    virtual void initialize() = 0;
    virtual void draw() = 0;

};
