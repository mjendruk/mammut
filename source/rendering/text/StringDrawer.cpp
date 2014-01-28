#include "StringDrawer.h"

#include <numeric>

#include <QDebug>
#include <QFile>
#include <QTextStream>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Texture.h>

#include "RawFile.h"
#include "CharacterDrawable.h"

const float StringDrawer::s_textureSize = 1024.0f;

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
    
    if (!m_stringComposer.readSpecificsFromFile("data/P22UndergroundPro-Medium.txt", s_textureSize))
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
    
    const QString fileName("data/P22UndergroundPro-Medium.1024.1024.r.ub.raw");
    
    RawFile file(fileName.toStdString());
    
    if (!file.isValid())
        return false;
    
    m_characterAtlas->image2D(0, GL_R8, s_textureSize, s_textureSize, 0, GL_RED, GL_UNSIGNED_BYTE, file.data());
    
    m_characterAtlas->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_characterAtlas->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void StringDrawer::paint(
    const QString & text,
    const glm::mat4 & modelMatrix,
    Alignment alignment, 
    const glm::vec3 color)
{
    m_program->setUniform("characterAtlas", 0);
    m_program->setUniform("color", color);
    
    m_characterAtlas->bind(GL_TEXTURE0);
    
    QList<CharacterSpecifics *> list = m_stringComposer.characterSequence(text);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    
    glm::mat4 transform = modelMatrix * alignmentTransform(list, alignment);
    
    for (int i = 0; i < list.size(); i++) {
        CharacterSpecifics * currentSpecifics = list[i];
        
        glm::mat4 positionTransform;
        glm::mat4 textureCoordTransform;

        textureCoordTransform *= glm::translate(glm::vec3(currentSpecifics->position, 0.0f));
        textureCoordTransform *= glm::scale(glm::vec3(currentSpecifics->size, 1.0f));
        
        positionTransform *= transform;
        positionTransform *= glm::translate(glm::vec3(currentSpecifics->offset, 0.0f));
        positionTransform *= glm::scale(glm::vec3(currentSpecifics->size, 1.0f));
        
        m_program->setUniform("positionTransform", positionTransform);
        m_program->setUniform("textureCoordTransform", textureCoordTransform);
        
        m_program->use();
        m_drawable.draw();
        m_program->release();
        
        transform *= glm::translate(currentSpecifics->xAdvance, 0.0f, 0.0f);
    }
    
    glDisable(GL_BLEND);
}

glm::mat4 StringDrawer::alignmentTransform(const QList<CharacterSpecifics *> & list, Alignment alignment) const
{
    float offset;
    
    const float length = std::accumulate(list.begin(), list.end(), 0.0f,
        [] (float sum, CharacterSpecifics * specifics) {
            return sum + specifics->xAdvance;
        });
    
    switch (alignment) {
        case kAlignLeft:
            offset = - list.first()->offset.x;
            break;
            
        case kAlignCenter:
            offset = - length / 2.0f;
            break;
            
        case kAlignRight:
            offset = - length;
            break;
    }
    
    return glm::translate(offset, 0.0f, 0.0f);
}
