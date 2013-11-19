#pragma once

#include <QList>
#include <QVector>

#include <glm/glm.hpp>
#include <glow/ref_ptr.h>
#include <glow/Array.h>

#include "AbstractPainter.h"

namespace glow
{
    class Program;
    class Shader;
    class Icosahedron;
    class VertexArrayObject;
    class Buffer;
}

class Cuboid;


class Painter : public AbstractPainter
{
public:
    Painter();
    virtual ~Painter();

    virtual bool initialize();
    
    virtual void paint(float timef);
    virtual void resize(int width, int height);

    virtual void update();
    virtual void update(const QList<glow::Program *> & programs);


    void drawCuboid();


    const QVector<Cuboid *> * m_cuboids;

protected:
    void initializeCuboidData();

    static glow::Vec3Array vertices();
    static glow::Array<glm::lowp_uvec3> indices();


    bool m_initialized;
    glow::Program * m_program;
    glow::Shader * m_fragShader;
    glow::Shader * m_vertShader;
    glow::ref_ptr<glow::VertexArrayObject> m_cuboidVao;
    glow::ref_ptr<glow::Buffer> m_cuboidVertices;
    glow::ref_ptr<glow::Buffer> m_cuboidIndices;

};
