#include "CreditsListItem.h"

#include <logic/highscore/HighscoreEntry.h>

#include <rendering/menu/MenuRenderer.h>

const QList<QString> CreditsListItem::m_creditsForenames = { "Carolin", "Johannes", "Max" };
const QList<QString> CreditsListItem::m_creditsNicknames = { "TINKERER", "WRECKER", "ARTIFICER" };
const QList<QString> CreditsListItem::m_creditsSurnames = { "Fiedler", "Linke", "Jendruk" };
const QList<QString> CreditsListItem::m_thanks = { "Sebastian Pasewaldt", "John Chapman", "Timothy Lottes", "Morgan McGuire et al.", "G3D Engine"};

CreditsListItem::CreditsListItem()
{
}

CreditsListItem::~CreditsListItem()
{
}

void CreditsListItem::accept(MenuRenderer * renderer)
{
    renderer->render(this);
}

void CreditsListItem::keyPressed(QKeyEvent * event)
{
}

bool CreditsListItem::isFocusable() const
{
    return false;
}

const QList<QString> & CreditsListItem::creditsForenames() const
{
    return m_creditsForenames;
}

const QList<QString> & CreditsListItem::creditsNicknames() const
{
    return m_creditsNicknames;
}

const QList<QString> & CreditsListItem::creditsSurnames() const
{
    return m_creditsSurnames;
}

const QList<QString> & CreditsListItem::thanks() const
{
    return m_thanks;
}
