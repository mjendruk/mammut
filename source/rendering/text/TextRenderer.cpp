#include "TextRenderer.h"

#include <QDebug>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/Texture.h>

#include <glowutils/global.h>

#include <rendering/RawFile.h>
#include "CharacterDrawable.h"

const float TextRenderer::s_lineHeight = 0.11f;
const float TextRenderer::s_textureSize = 1024.0f;

TextRenderer::TextRenderer()
{
    m_valid = initialize();
}

TextRenderer::~TextRenderer()
{

}

bool TextRenderer::initialize()
{
    if (!initializeProgram())
        return false;

    if (!initializeTexture())
        return false;
    
    if (!m_stringComposer.readSpecificsFromFile("data/fonts/P22UndergroundPro-Medium.txt", s_textureSize))
        return false;

    return true;
}

bool TextRenderer::initializeProgram()
{
    m_program = new glow::Program();
    
    m_program->attach(
        glowutils::createShaderFromFile(GL_VERTEX_SHADER, "data/shaders/string_drawer.vert"),
        glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/shaders/string_drawer.frag"));
    
    m_program->link();
    
    if (!m_program->isLinked()) {
        qDebug() << m_program->infoLog().c_str();
        return false;
    }

    return true;
}

bool TextRenderer::initializeTexture()
{
    m_characterAtlas = new glow::Texture();
    
    RawFile file("data/fonts/P22UndergroundPro-Medium.1024.1024.r.ub.raw");
    
    if (!file.isValid())
        return false;
    
    m_characterAtlas->image2D(0, GL_R8, s_textureSize, s_textureSize, 0, GL_RED, GL_UNSIGNED_BYTE, file.data());
    
    m_characterAtlas->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_characterAtlas->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_program->setUniform("characterAtlas", 0);
    
    return true;
}

void TextRenderer::paint(
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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    m_characterAtlas->bindActive(GL_TEXTURE0);
    
    m_program->setUniform("color", color);
    
    m_program->use();
    m_drawable.draw(characterSpecificsList.count(),
                    vertexTransforms,
                    textureCoordTransforms);
    m_program->release();
    
    glDisable(GL_BLEND);
}

bool TextRenderer::isValid() const
{
    return m_valid;
}

void TextRenderer::prepareTransforms(
    const QList<CharacterSpecifics *> characterSpecificsList,
    const glm::mat4 & modelMatrix,
    Alignment alignment,
    QVector<glm::mat4> & vertexTransforms,
    QVector<glm::mat4> & textureCoordTransforms)
{
    glm::mat4 transform = modelMatrix * alignmentTransform(characterSpecificsList, alignment);
    
    for (CharacterSpecifics * specifics : characterSpecificsList) {
        glm::mat4 vertexTransform = transform * specifics->vertexTransform;
        
        vertexTransforms << vertexTransform;
        textureCoordTransforms << specifics->textureCoordTransform;
        
        transform *= glm::translate(specifics->xAdvance, 0.0f, 0.0f);
    }
}

glm::mat4 TextRenderer::alignmentTransform(
    const QList<CharacterSpecifics *> & characterSpecificsList, Alignment alignment)
{
    float offset;
    
    float length = 0.0f;
    for (CharacterSpecifics * specifics : characterSpecificsList)
        length += specifics->xAdvance;
    
    switch (alignment)
    {
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