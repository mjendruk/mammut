#pragma once

#include <QObject>

class QKeyEvent;

class Mechanics : public QObject
{
public:
    virtual ~Mechanics();

    virtual void update(float seconds) = 0;

    virtual void keyPressed(QKeyEvent * event) = 0;
    virtual void keyReleased(QKeyEvent * event) = 0;

};
