
#pragma once

#include <memory>

#include "AbstractApplication.h"

class Painter;

class Viewer;
class Canvas;

class Application : public AbstractApplication
{
public:
    Application(int & argc, char ** argv);
    virtual ~Application();

protected:
    Viewer * m_viewer;
	Canvas * m_canvas;

	Painter * m_painter;
};