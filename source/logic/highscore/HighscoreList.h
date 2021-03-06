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
    void readFromFile();
    void handleCorruptFile();
    void writeToFile() const;
    
protected:
    static const QString s_filename;
    static const int s_magicNumber;
    static const int s_limit;

    QList<HighscoreEntry> m_list;
};
