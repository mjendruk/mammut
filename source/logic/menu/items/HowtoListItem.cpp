#include "HowtoListItem.h"

#include <logic/menu/items/HowtoItem.h>

#include <rendering/menu/MenuRenderer.h>

HowtoListItem::HowtoListItem(const QList<HowtoItem> & gravityKeys, const QList<HowtoItem> & boostKeys)
:   m_gravityKeys(gravityKeys)
,   m_boostKeys(boostKeys)
{
}

HowtoListItem::~HowtoListItem()
{
}

void HowtoListItem::accept(MenuRenderer * renderer)
{
    renderer->render(this);
}

void HowtoListItem::keyPressed(QKeyEvent * event)
{
}

bool HowtoListItem::isFocusable() const
{
    return false;
}

const QList<HowtoItem> & HowtoListItem::gravityKeys() const
{
    return m_gravityKeys;
}

const QList<HowtoItem> & HowtoListItem::boostKeys() const
{
    return m_boostKeys;
}
