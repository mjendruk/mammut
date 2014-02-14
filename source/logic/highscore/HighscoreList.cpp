#include "HighscoreList.h"

#include <cassert>

#include <QMutableListIterator>
#include "HighscoreEntry.h"

HighscoreList::HighscoreList()
{
    addScore("Mum", 1000);
}

HighscoreList::~HighscoreList()
{
}

void HighscoreList::addScore(const QString & name, unsigned int score)
{
    assert(isHighscore(score));

    if (m_list.isEmpty())
    {
        m_list.append(HighscoreEntry(name, score));
        return;
    }

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

    return;
}

bool HighscoreList::isHighscore(unsigned int score) const
{
    return m_list.size() < s_limit || m_list.last().score() < score;
}

const QList<HighscoreEntry> & HighscoreList::scores() const
{
    return m_list;
}
