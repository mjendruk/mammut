#include "StringDrawer.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QImage>
#include <QGLWidget>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Texture.h>

#include "RawFile.h"
#include "CharacterDrawable.h"

StringDrawer::StringDrawer()
{

}

StringDrawer::~StringDrawer()
{

}

bool StringDrawer::initialize()
{
    if (!initializeProgram())
        return false;

    if (!initializeTexture())
        return false;
    
    if (!m_stringComposer.readSpecificsFromFile("data/NeoSans.txt"))
        return false;
    
    m_drawable.initialize();

    return true;
}

bool StringDrawer::initializeProgram()
{
    m_program = new glow::Program();

    QFile vertShaderFile("data/string_drawer.vert");
    if (!vertShaderFile.open(QIODevice::ReadOnly))
    {
        qCritical() << "Could not open vertex shader source";
        return false;
    }

    QFile fragShaderFile("data/string_drawer.frag");
    if (!fragShaderFile.open(QIODevice::ReadOnly))
    {
        qCritical() << "Could not open fragment shader source";
        return false;
    }

    QTextStream vertShaderStream(&vertShaderFile);
    QTextStream fragShaderStream(&fragShaderFile);

    auto vertShader = glow::Shader::fromString(GL_VERTEX_SHADER, vertShaderStream.readAll().toStdString());
    auto fragShader = glow::Shader::fromString(GL_FRAGMENT_SHADER, fragShaderStream.readAll().toStdString());

    m_program->attach(vertShader, fragShader);
    m_program->link();

    vertShaderFile.close();
    fragShaderFile.close();

    return true;
}

bool StringDrawer::initializeTexture()
{
    m_characterAtlas = new glow::Texture();
    
    const QString fileName("data/NeoSans.png");
    
    if (!QFile::exists(fileName)) {
        qCritical() << "Could not find font source";
        return false;
    }
    
    QImage texture(fileName);
    texture = QGLWidget::convertToGLFormat(texture);
    
    m_characterAtlas->image2D(0, GL_R8, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());
    
    m_characterAtlas->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_characterAtlas->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void StringDrawer::paint(const QString & string, const glm::mat4 & modelMatrix)
{
    glm::mat4 positionTransform;
    glm::mat4 textureCoordTransform;
    
    CharacterSpecifics * charH = m_stringComposer.characterSequence("$").at(0);

    glm::vec2 position(charH->position.x, 512.0f - (charH->position.y + charH->size.y));
    textureCoordTransform *= glm::translate(glm::vec3(position / glm::vec2(512.0f), 0.0f));
    
    textureCoordTransform *= glm::scale(glm::vec3(charH->size / glm::vec2(512.0f), 1.0f));
    
    m_program->setUniform("positionTransform", positionTransform);
    m_program->setUniform("textureCoordTransform", textureCoordTransform);
    m_program->setUniform("characterAtlas", 0);
    
    m_characterAtlas->bind(GL_TEXTURE0);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    
    m_program->use();
    
    m_drawable.draw();
    
    m_program->release();
    
    glDisable(GL_BLEND);
}
