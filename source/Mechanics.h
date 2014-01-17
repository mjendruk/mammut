#pragma once

class QKeyEvent;
class Renderer;

class Mechanics
{
public:
    virtual ~Mechanics();

    virtual void update(float seconds) = 0;
    virtual Renderer * renderer() = 0;

    virtual void keyPressed(QKeyEvent * event) = 0;
    virtual void keyReleased(QKeyEvent * event) = 0;

};
