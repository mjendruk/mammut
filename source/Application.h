
#pragma once

#include <memory>

#include "AbstractApplication.h"

class Canvas;
class AbstractPainter;

class Application : public AbstractApplication
{
    Q_OBJECT
public:
    Application(int & argc, char ** argv);
    virtual ~Application();

protected slots:
    void run();

protected:
    Canvas * m_canvas;
    AbstractPainter * m_painter;
    bool m_loop;

};
