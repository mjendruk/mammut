
#include "DebugPainter.h"


DebugPainter::DebugPainter()
{
}

DebugPainter::~DebugPainter()
{
}

AbstractContext * DebugPainter::context() const
{
	return nullptr;
}

const bool DebugPainter::initialize(const Format & format)
{
    return false;
}

void DebugPainter::paint(const unsigned int targetFBO)
{
}
 
void DebugPainter::resize(
    const int width
,   const int height)
{
}
