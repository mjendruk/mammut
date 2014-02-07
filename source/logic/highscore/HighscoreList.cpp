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

    if (m_list.last().score() >= score)
        return false;

    QMutableListIterator<HighscoreEntry> it(m_list);
    while (it.hasNext())
    {
        if (it.peekNext().score() < score)
        {
            it.insert(HighscoreEntry(name, score));
            break;
        }

        it.next();
    }

    m_list.removeLast();
    return true;
}

const QList<HighscoreEntry> & HighscoreList::scores() const
{
    return m_list;
}
