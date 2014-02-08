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

#include <rendering/RawFile.h>
#include "CharacterDrawable.h"

const float StringDrawer::s_textureSize = 1024.0f;

StringDrawer::StringDrawer()
{
    initialize();
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
    
    if (!m_stringComposer.readSpecificsFromFile("data/fonts/P22UndergroundPro-Medium.txt", s_textureSize))
        return false;

    return true;
}

bool StringDrawer::initializeProgram()
{
    m_program = new glow::Program();

    QFile vertShaderFile("data/shaders/string_drawer.vert");
    if (!vertShaderFile.open(QIODevice::ReadOnly))
    {
        qCritical() << "Could not open vertex shader source";
        return false;
    }

    QFile fragShaderFile("data/shaders/string_drawer.frag");
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
    
    const QString fileName("data/fonts/P22UndergroundPro-Medium.1024.1024.r.ub.raw");
    
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
    QVector<glm::mat4> vertexTransforms;
    QVector<glm::mat4> textureCoordTransforms;
    
    QList<CharacterSpecifics *> characterSpecificsList = m_stringComposer.characterSequence(text);
    
    prepareTransforms(characterSpecificsList,
                      modelMatrix,
                      alignment,
                      vertexTransforms,
                      textureCoordTransforms);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    
    m_program->setUniform("characterAtlas", 0);
    m_program->setUniform("color", color);
    
    m_characterAtlas->bind(GL_TEXTURE0);
    
    m_program->use();
    m_drawable.draw(characterSpecificsList.count(),
                    vertexTransforms,
                    textureCoordTransforms);
    m_program->release();
    
    glDisable(GL_BLEND);
}

void StringDrawer::prepareTransforms(
    const QList<CharacterSpecifics *> characterSpecificsList,
    const glm::mat4 & modelMatrix,
    Alignment alignment,
    QVector<glm::mat4> & vertexTransforms,
    QVector<glm::mat4> & textureCoordTransforms)
{
    glm::mat4 transform = modelMatrix * alignmentTransform(characterSpecificsList, alignment);
    
    for (auto * specifics : characterSpecificsList)
    {
        glm::mat4 vertexTransform = transform * specifics->vertexTransform;
        
        vertexTransforms << vertexTransform;
        textureCoordTransforms << specifics->textureCoordTransform;
        
        transform *= glm::translate(specifics->xAdvance, 0.0f, 0.0f);
    }
}

glm::mat4 StringDrawer::alignmentTransform(
    const QList<CharacterSpecifics *> & characterSpecificsList, Alignment alignment)
{
    float offset;
    
    const float length = std::accumulate(characterSpecificsList.begin(),
                                         characterSpecificsList.end(), 0.0f,
        [] (float sum, CharacterSpecifics * specifics) {
            return sum + specifics->xAdvance;
        });
    
    switch (alignment) {
        case kAlignLeft:
            offset = 0;
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
