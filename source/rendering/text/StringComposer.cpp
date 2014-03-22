#include "StringComposer.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QByteArray>

#include <glm/gtx/transform.hpp>

StringComposer::StringComposer()
{
}

StringComposer::~StringComposer()
{
    qDeleteAll(m_characterSpecifics);
}

QList<CharacterSpecifics *> StringComposer::characterSequence(const QString & string) const
{
    QByteArray latin1String = string.toLatin1();
    QList<CharacterSpecifics *> characterSequence;
    
    for (int i = 0; i < latin1String.size(); i++) {
        unsigned char id = latin1String[i];
        
        if (!m_characterSpecifics.contains(id))
            continue;
        
        characterSequence << m_characterSpecifics.value(id);
    }
    
    return characterSequence;
}

QList<CharacterSpecifics *> StringComposer::characterSequence(const int id, const int repeat) const
{
    QList<CharacterSpecifics *> characterSequence;

    if (m_characterSpecifics.contains(id)) {
        for (int i = 0; i < repeat; ++i)
            characterSequence << m_characterSpecifics.value(id);
    }

    return characterSequence;
}

bool StringComposer::readSpecificsFromFile(const QString & fileName, float textureSize)
{
    m_characterSpecifics.clear();
    
    QFile file(fileName);
    
    if (!file.open(QIODevice::ReadOnly)) {
        qCritical() << "Could not open" << qPrintable(fileName);
        return false;
    }
    
    QTextStream stream(&file);
    
    // Skip first two lines
    stream.readLine();
    stream.readLine();
    
    while (!stream.atEnd())
        parseCharacterLine(stream.readLine(), textureSize);
    
    file.close();
    
    return true;
}

void StringComposer::parseCharacterLine(const QString & line, float textureSize)
{
    // Note: The coordinate origin of the input file is at the upper left corner.
    
    QRegularExpression regExp("=(\\S+)");
    QRegularExpressionMatchIterator it = regExp.globalMatch(line);
    
    unsigned int id = it.next().captured(1).toUInt();
    
    float x = it.next().captured(1).toUInt();
    float y = it.next().captured(1).toUInt();
    float width = it.next().captured(1).toUInt();
    float height = it.next().captured(1).toUInt();
    float xOffset = it.next().captured(1).toFloat();
    float yOffset = it.next().captured(1).toFloat();
    float xAdvance = it.next().captured(1).toFloat();
    
    glm::vec3 size = glm::vec3(width, height, 1.0f) / textureSize;
    
    glm::vec3 textureCoordPosition = glm::vec3(x, textureSize - (y + height), 0.0f) / textureSize;
    glm::mat4 textureCoordTransform = glm::translate(textureCoordPosition)
                                      * glm::scale(size);
    
    glm::vec3 vertexPosition = glm::vec3(xOffset, yOffset - height, 0.0f) / textureSize;
    glm::mat4 vertexTransform = glm::translate(vertexPosition - glm::vec3(0.0f, 0.08f, 0.0f))
                                * glm::scale(size);
    
    auto specifics = new CharacterSpecifics { vertexTransform,
                                              textureCoordTransform,
                                              xAdvance / textureSize };
    
    m_characterSpecifics.insert(id, specifics);
}
