#include "StringComposer.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QByteArray>

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

bool StringComposer::readSpecificsFromFile(const QString & fileName, float textureSize)
{
    m_characterSpecifics.clear();
    
    QFile file(fileName);
    
    if (!file.open(QIODevice::ReadOnly)) {
        qCritical() << "Could not open" << qPrintable(fileName);
        return false;
    }
    
    QTextStream stream(&file);
    
    QString firstLine = stream.readLine();
    
    QRegularExpression regExp("\"(.+)\"");
    QString fontName = regExp.match(firstLine).captured(1);
    
    qDebug() << "Reading info of font" << qPrintable(fontName);
    
    stream.readLine();
    
    while (!stream.atEnd())
        parseCharacterLine(stream.readLine(), textureSize);
    
    file.close();
    
    return true;
}

void StringComposer::parseCharacterLine(const QString & line, float textureSize)
{
    QRegularExpression regExp("=(\\S+)");
    QRegularExpressionMatchIterator it = regExp.globalMatch(line);
    
    unsigned int id = it.next().captured(1).toUInt();
    
    float x = it.next().captured(1).toUInt();
    float y = it.next().captured(1).toUInt();
    float width = it.next().captured(1).toUInt();
    float height = it.next().captured(1).toUInt();
    float xOffset = it.next().captured(1).toFloat();
    float yOffset = it.next().captured(1).toFloat();
    
    float xAdvance = it.next().captured(1).toFloat() / textureSize;
    
    glm::vec2 position = glm::vec2(x, textureSize - (y + height)) / textureSize;
    glm::vec2 size = glm::vec2(width, height) / textureSize;
    glm::vec2 offset = glm::vec2(xOffset, yOffset - height) / textureSize;
    
    auto specifics = new CharacterSpecifics { position, size, offset, xAdvance };
    m_characterSpecifics.insert(id, specifics);
}
