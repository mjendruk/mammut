#include "HighscoreList.h"

#include <QMutableListIterator>
#include "HighscoreEntry.h"

HighscoreList::HighscoreList()
{
}

HighscoreList::~HighscoreList()
{
}

bool HighscoreList::addScore(const QString & name, unsigned int score)
{
    if (m_list.isEmpty())
    {
        m_list.append(HighscoreEntry(name, score));
        return true;
    }

    if (m_list.size() >= s_limit)
    {
        if (m_list.last().score() >= score)
            return false;
        
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

    return true;
}

bool HighscoreList::isHighscore(unsigned int score) const
{
    return m_list.last().score() < score || m_list.size() < s_limit;
}

const QList<HighscoreEntry> & HighscoreList::scores() const
{
    return m_list;
}
