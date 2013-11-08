
#include <cassert>

#include "FileAssociatedShader.h"
#include "Painter.h"
#include "Camera.h"


Painter::Painter()
: m_program(nullptr)
, m_fragShader(nullptr)
, m_vertShader(nullptr)
{
}

Painter::~Painter()
{
    delete m_program;

    delete m_fragShader;
    delete m_vertShader;
}

bool Painter::initialize()
{
    glClearColor(1.f, 1.f, 1.f, 0.f);

    m_program = new QOpenGLShaderProgram();

    m_fragShader = FileAssociatedShader::getOrCreate(
        QOpenGLShader::Fragment, "data/default.frag", *m_program);
    m_vertShader = FileAssociatedShader::getOrCreate(
        QOpenGLShader::Vertex, "data/default.vert", *m_program);
    m_program->link();

    return true;
}

void Painter::resize(
    int width
,   int height)
{
    glViewport(0, 0, width, height);

    if (m_program->isLinked())
    {
        m_program->bind();
        QMatrix4x4 mat;
        for (int i = 0; i < 16; i++)
            mat.data()[i] = camera()->viewProjection()[i%4][i/4]; //TODO
        m_program->setUniformValue("transform", mat);
        m_program->release();
    }
}

void Painter::update()
{
    if (m_program->isLinked())
    {
        m_program->bind();
        QMatrix4x4 mat;
        for (int i = 0; i < 16; i++)
            mat.data()[i] = camera()->viewProjection()[i%4][i/4]; //TODO
        m_program->setUniformValue("transform", mat);
        m_program->release();
    }
}

void Painter::update(const QList<QOpenGLShaderProgram *> & programs)
{
    if (programs.contains(m_program) && m_program->isLinked())
    {
        m_program->bind();
        QMatrix4x4 mat;
        for (int i = 0; i < 16; i++)
            mat.data()[i] = camera()->viewProjection()[i%4][i/4]; //TODO
        m_program->setUniformValue("transform", mat);
        m_program->release();
    }
}

void Painter::paint(float timef)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_program->isLinked())
    {
        m_program->bind();
        // Custom shizzle
        m_program->release();
    }
 
}
