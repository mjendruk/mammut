#pragma once

#include <QObject>

class QKeyEvent;
class MenuRenderer;

class MenuItem : public QObject
{
public:
    virtual ~MenuItem();

    virtual void accept(MenuRenderer * renderer) = 0;
    virtual void keyPressed(QKeyEvent * event) = 0;

};
