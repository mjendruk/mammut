
#pragma once

#include <memory>

#include <glwhite/AbstractApplication.h>

class Painter;

namespace glwhite
{
    class Viewer;
	class Canvas;
}

class Application : public glwhite::AbstractApplication
{
public:
    Application(int & argc, char ** argv);
    virtual ~Application();

protected:
    glwhite::Viewer * m_viewer;
	glwhite::Canvas * m_canvas;

	Painter * m_painter;
};