#include "HighscoreEntry.h"

HighscoreEntry::HighscoreEntry(const QString & name, unsigned int score)
:   m_name(name)
,   m_score(score)
{
}

HighscoreEntry::~HighscoreEntry()
{
}

const QString & HighscoreEntry::name() const
{
    return m_name;
}

unsigned int HighscoreEntry::score() const
{
    return m_score;
}
