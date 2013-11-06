
#pragma once

#include "AbstractPainter.h"


class DebugPainter : public AbstractPainter
{
public:
    DebugPainter();
    virtual ~DebugPainter();

	virtual AbstractContext * context() const;

    virtual const bool initialize(const Format & format);

    virtual void paint(const unsigned int targetFBO);
    virtual void resize(
        const int width
    ,   const int height);
};
