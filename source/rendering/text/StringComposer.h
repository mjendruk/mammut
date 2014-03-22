#pragma once

#include <QList>
#include <QHash>
#include <QString>

#include <glm/glm.hpp>

struct CharacterSpecifics
{
    glm::mat4 vertexTransform;
    glm::mat4 textureCoordTransform;
    float xAdvance;
};

class StringComposer
{
public:
    StringComposer();
    ~StringComposer();

    bool readSpecificsFromFile(const QString & fileName, float textureSize);
    QList<CharacterSpecifics *> characterSequence(const QString & string) const;
    QList<CharacterSpecifics *> characterSequence(const int id, const int repeat = 1) const;

protected:
    void parseCharacterLine(const QString & line, float textureSize);

    QHash<unsigned int, CharacterSpecifics *> m_characterSpecifics;
};
