#include "HighscoreList.h"

#include <cassert>

#include <QMutableListIterator>
#include <QFile>
#include <QDataStream>
#include <QString>
#include <QDebug>

#include "HighscoreEntry.h"

const QString HighscoreList::s_filename = "highscores.mh";
const int HighscoreList::s_magicNumber = 326244764;
const int HighscoreList::s_limit = 10;

HighscoreList::HighscoreList()
{
    readFromFile();
}

HighscoreList::~HighscoreList()
{
}

void HighscoreList::addScore(const QString & name, unsigned int score)
{
    assert(isHighscore(score));

    if (m_list.size() >= s_limit)
    {
        if (m_list.last().score() >= score)
            return;
        
        m_list.removeLast();
    }

    QMutableListIterator<HighscoreEntry> it(m_list);
    while (it.hasNext())
    {
        if (it.peekNext().score() < score)
            break;

        it.next();
    }
    
    it.insert(HighscoreEntry(name, score));

    writeToFile();
}

bool HighscoreList::isHighscore(unsigned int score) const
{
    return m_list.size() < s_limit || m_list.last().score() < score;
}

const QList<HighscoreEntry> & HighscoreList::scores() const
{
    return m_list;
}

void HighscoreList::readFromFile()
{
    QFile file(s_filename);
    file.open(QIODevice::ReadOnly);

    if (!file.exists()) {
        qDebug() << "No highscore file found. A new one will be created.";
        return;
    }

    QDataStream in(&file);

    int magicNumber;
    in >> magicNumber;

    int numHighscores;
    in >> numHighscores;

    if (magicNumber != s_magicNumber || numHighscores > s_limit) {
        handleCorruptFile();
        return;
    }

    for (int i = 0; i < numHighscores; i++) {
        QString name;
        unsigned int score;
        in >> name >> score;

        m_list.append(HighscoreEntry(name, score));
    }

    if (in.status() == QDataStream::ReadPastEnd || !in.atEnd() || numHighscores > s_limit) {
        handleCorruptFile();
    }
}

void HighscoreList::handleCorruptFile()
{
    m_list.clear();
    qWarning() << "Invalid highscore file, ignoring it";
}

void HighscoreList::writeToFile() const
{
    QFile file(s_filename);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QDataStream out(&file);

    out << s_magicNumber;
    out << m_list.size();

    for (HighscoreEntry entry : m_list) {
        out << entry.name() << entry.score();
    }
}
