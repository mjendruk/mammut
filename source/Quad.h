
#pragma once

#include <glm/glm.hpp>
#include <glow/ref_ptr.h>
#include <glow/Array.h>

namespace glow {
    class VertexArrayObject;
    class Buffer;
}

class Quad
{
public:
    Quad();
    Quad(const glm::mat4 & matrix);
    ~Quad();

    void draw();

    const glm::mat4 & matrix();
    void setMatrix(const glm::mat4 & matrix);

protected:
    static glow::Vec3Array vertices();
    static glow::Array<glm::lowp_uvec3> indices();
    void initialize();
    
    glow::ref_ptr<glow::VertexArrayObject> m_vao;
    glow::ref_ptr<glow::Buffer> m_vertices;
    glow::ref_ptr<glow::Buffer> m_indices;
    glm::mat4 m_matrix;
    int m_size;

};
