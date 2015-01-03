#pragma once

#include <glm/glm.hpp>

#include <glow/ref_ptr.h>

namespace glow
{
    class Program;
}

class BunchOfTetsDrawable;

class TetPainter
{
public:
    TetPainter();
    ~TetPainter();

    void setNearFarUniform(const glm::vec2 & nearFar);
    void setViewProjectionUniform(const glm::mat4 & viewProjection);
    void setViewUniform(const glm::mat4 & view);
    void setEyeUniform(const glm::vec3 & eye);

    void paint(BunchOfTetsDrawable & drawable, const glm::mat4 & modelMatrix);

protected:
    void initialize();

protected:
    glow::ref_ptr<glow::Program> m_program;
};
