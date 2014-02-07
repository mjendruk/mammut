#pragma once

#include <QList>

class HighscoreEntry;

class HighscoreList
{
public:
    HighscoreList();
    virtual ~HighscoreList();

    bool addScore(const QString & name, unsigned int score);
    const QList<HighscoreEntry> & scores() const;

protected:
    QList<HighscoreEntry> m_list;

};
