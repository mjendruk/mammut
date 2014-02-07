#pragma once

#include <QString>

class HighscoreEntry
{
public:
    HighscoreEntry(const QString & name, unsigned int score);
    ~HighscoreEntry();

    const QString & name() const;
    unsigned int score() const;

private:
    QString m_name;
    unsigned int m_score;

};
