#pragma once

class IDrawable
{
public:
    virtual ~IDrawable() {}

    virtual void initialize() = 0;
    virtual void draw() = 0;

};
