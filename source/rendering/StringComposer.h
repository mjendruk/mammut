#pragma once

#include <QList>
#include <QHash>
#include <QString>

#include <glm/glm.hpp>

struct CharacterSpecifics
{
    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 offset;
    float xAdvance;
};

class StringComposer
{
public:
    StringComposer();
    ~StringComposer();

    bool readSpecificsFromFile(const QString & fileName, float textureSize);
    QList<CharacterSpecifics *> characterSequence(const QString & string) const;

protected:
    void parseCharacterLine(const QString & line, float textureSize);

    QHash<unsigned int, CharacterSpecifics *> m_characterSpecifics;
};
