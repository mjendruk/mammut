#pragma once

#include <list>
#include <vector>

#include <glm/glm.hpp>

#include <glow/ref_ptr.h>


namespace glow
{
    class Buffer;
    class Program;
    class VertexArrayObject;
}

class ParticleRenderer
{
public:
    ParticleRenderer();

    void paint(const glm::mat4 & view,
               const float fps,
               const std::vector<glm::vec3> & particles);

    void setProjection(const glm::mat4 & projection);


protected:
    void pushView(const glm::mat4 & view, const float fps);
    const glm::mat4 & newestView() const;
    const glm::mat4 & oldestView() const;

private:
    glow::ref_ptr<glow::Program> m_program;
    glow::ref_ptr<glow::VertexArrayObject> m_vao;
    glow::ref_ptr<glow::Buffer> m_buffer;

    std::list<glm::mat4> m_viewStack;
};
