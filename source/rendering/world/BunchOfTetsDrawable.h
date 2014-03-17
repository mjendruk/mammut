#pragma once

#include <glm/glm.hpp>

#include <glow/ref_ptr.h>

namespace glow
{
    class Program;
    class VertexArrayObject;
    class Buffer;
    class Texture;
}

class BunchOfTets;


class BunchOfTetsDrawable
{
public:
    BunchOfTetsDrawable(const BunchOfTets & bunchOfTets);
    ~BunchOfTetsDrawable();

    void setNearFarUniform(const glm::vec2 & nearFar);
    void setViewProjectionUniform(const glm::mat4 & viewProjection);
    void setViewUniform(const glm::mat4 & view);
    void setEyeUniform(const glm::vec3 & eye);

    void initialize();
    void paint(const glm::mat4 & model);

protected:
    const BunchOfTets & m_bunchOfTets;

    glow::ref_ptr<glow::VertexArrayObject> m_vao;

    glow::ref_ptr<glow::Texture> m_vertexTexture;
    glow::ref_ptr<glow::Texture> m_normalTexture;
    glow::ref_ptr<glow::Texture> m_matrixTexture;

    glow::ref_ptr<glow::Buffer> m_vertexBuffer;
    glow::ref_ptr<glow::Buffer> m_normalBuffer;
    glow::ref_ptr<glow::Buffer> m_matrixBuffer;

    glow::ref_ptr<glow::Program> m_program;
};
