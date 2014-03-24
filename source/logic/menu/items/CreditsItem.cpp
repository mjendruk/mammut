#include "CreditsItem.h"

#include <logic/highscore/HighscoreEntry.h>

#include <rendering/menu/MenuRenderer.h>


CreditsItem::CreditsItem()
: m_creditsForenames({ "Carolin", "Johannes", "Max" })
, m_creditsNicknames({ "RNG TAMER", "DEMOMAN", "BIZ GUY" })
, m_creditsSurnames({ "Fiedler", "Linke", "Jendruk" })
, m_thanks({ "Sebastian Pasewaldt", "John Chapman", "Timothy Lottes", "Morgan McGuire et al.", "G3D team" })
{
}

CreditsItem::~CreditsItem()
{
}

void CreditsItem::accept(MenuRenderer * renderer)
{
    renderer->render(this);
}

void CreditsItem::keyPressed(QKeyEvent * event)
{
}

bool CreditsItem::isFocusable() const
{
    return false;
}

const QList<QString> & CreditsItem::creditsForenames() const
{
    return m_creditsForenames;
}

const QList<QString> & CreditsItem::creditsNicknames() const
{
    return m_creditsNicknames;
}

const QList<QString> & CreditsItem::creditsSurnames() const
{
    return m_creditsSurnames;
}

const QList<QString> & CreditsItem::thanks() const
{
    return m_thanks;
}
