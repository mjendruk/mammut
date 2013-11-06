
#pragma once

#include <glow/Program.h>
#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>

#include "AbstractPainter.h"


class Painter : public AbstractPainter
{
public:
    Painter();
	virtual ~Painter();

	virtual AbstractContext * context() const;

    virtual const bool initialize(const Format & format);

	virtual void paint(const unsigned int targetFBO);
    virtual void resize(
        const int width
    ,   const int height);

protected:
    glow::ref_ptr<glow::VertexArrayObject> m_vao;
    glow::ref_ptr<glow::Buffer> m_buffer;

    glow::ref_ptr<glow::Program> m_program;
};
