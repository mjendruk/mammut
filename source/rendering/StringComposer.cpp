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
        unsigned int id = latin1String[i];
        
        if (!m_characterSpecifics.contains(id))
            continue;
        
        characterSequence << m_characterSpecifics.value(id);
    }
    
    return characterSequence;
}

bool StringComposer::readSpecificsFromFile(const QString & fileName)
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
        parseCharacterLine(stream.readLine());
    
    file.close();
    
    return true;
}

void StringComposer::parseCharacterLine(const QString & line)
{
    QRegularExpression regExp("=(\\S+)");
    QRegularExpressionMatchIterator it = regExp.globalMatch(line);
    
    unsigned int id = it.next().captured(1).toUInt();
    
    auto specifics = new CharacterSpecifics {
        glm::vec2(it.next().captured(1).toUInt(), it.next().captured(1).toUInt()),
        glm::vec2(it.next().captured(1).toUInt(), it.next().captured(1).toUInt()),
        glm::vec2(it.next().captured(1).toFloat(), it.next().captured(1).toFloat()),
        it.next().captured(1).toFloat()
    };
    
    m_characterSpecifics.insert(id, specifics);
}
