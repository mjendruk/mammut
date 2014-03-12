#pragma once

#include <QList>

class HighscoreEntry;
class QString;

class HighscoreList
{
public:
    HighscoreList();
    virtual ~HighscoreList();

    void addScore(const QString & name, unsigned int score);
    bool isHighscore(unsigned int score) const;

    const QList<HighscoreEntry> & scores() const;

protected:
    void readHighscoresFromFile();
    void writeHighscoresToFile() const;
    
    static const QString s_filename;
    static const int s_limit = 10;

protected:
    QList<HighscoreEntry> m_list;

};
