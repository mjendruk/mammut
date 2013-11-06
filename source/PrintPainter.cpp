
#include "PrintPainter.h"

PrintPainter::PrintPainter()
{
}

PrintPainter::~PrintPainter()
{
}

AbstractContext * PrintPainter::context() const
{
	return nullptr;
}

const bool PrintPainter::initialize(const Format & format)
{
    return false;
}

void PrintPainter::paint(const unsigned int targetFBO)
{
}
 
void PrintPainter::resize(
    const int width
,   const int height)
{
}

