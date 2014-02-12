#pragma once

#include <QList>

class HighscoreEntry;

class HighscoreList
{
public:
    HighscoreList();
    virtual ~HighscoreList();

    bool addScore(const QString & name, unsigned int score);
    bool isHighscore(unsigned int score) const;

    const QList<HighscoreEntry> & scores() const;

protected:
    static const int s_limit = 10;

protected:
    QList<HighscoreEntry> m_list;

};
