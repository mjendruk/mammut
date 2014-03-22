#include "ParticleRenderer.h"

#include <glow/Buffer.h>
#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>
#include <glow/Program.h>

#include <glowutils/global.h>


ParticleRenderer::ParticleRenderer()
{
    static const int attributeIndex = 0;

    m_program = new glow::Program();
    m_program->attach(glowutils::createShaderFromFile(GL_VERTEX_SHADER, "data/shaders/particle.vert"),
                      glowutils::createShaderFromFile(GL_GEOMETRY_SHADER, "data/shaders/particle.geom"),
                      glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/shaders/particle.frag"));
    m_program->link();

    m_buffer = new glow::Buffer(GL_ARRAY_BUFFER);

    m_vao = new glow::VertexArrayObject();
    glow::VertexAttributeBinding * binding = m_vao->binding(attributeIndex);
    binding->setAttribute(attributeIndex);
    binding->setBuffer(m_buffer, 0, sizeof(glm::vec3));
    binding->setFormat(3, GL_FLOAT);
    m_vao->enable(attributeIndex);
}

void ParticleRenderer::paint(
    const glm::mat4 & view,
    const float fps,
    const std::vector<glm::vec3> & particles)
{
    pushView(view, fps);

    m_program->setUniform("newView", newestView());
    m_program->setUniform("oldView", oldestView());

    m_buffer->setData(particles);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_program->use();

    m_vao->bind();
    m_vao->drawArrays(GL_POINTS, 0, particles.size());
    m_vao->unbind();

    m_program->release();

    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void ParticleRenderer::setProjection(const glm::mat4 & projection)
{
    m_program->setUniform("projection", projection);
}

void ParticleRenderer::pushView(const glm::mat4 & view, const float fps)
{
    const int maxSize = int(fps / 4.0f);
    
    m_viewStack.push_front(view);

    while(m_viewStack.size() > maxSize)
        m_viewStack.pop_back();   
}

const glm::mat4 & ParticleRenderer::newestView() const
{
    return m_viewStack.front();
}

const glm::mat4 & ParticleRenderer::oldestView() const
{
    return m_viewStack.back();
}
