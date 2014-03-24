#include "ControlsListItem.h"

#include <logic/menu/items/ControlsItem.h>

#include <rendering/menu/MenuRenderer.h>

ControlsListItem::ControlsListItem(const QList<ControlsItem> & gravityKeys, const QList<ControlsItem> & boostKeys)
:   m_gravityKeys(gravityKeys)
,   m_boostKeys(boostKeys)
{
}

ControlsListItem::~ControlsListItem()
{
}

void ControlsListItem::accept(MenuRenderer * renderer)
{
    renderer->render(this);
}

void ControlsListItem::keyPressed(QKeyEvent * event)
{
}

bool ControlsListItem::isFocusable() const
{
    return false;
}

const QList<ControlsItem> & ControlsListItem::gravityKeys() const
{
    return m_gravityKeys;
}

const QList<ControlsItem> & ControlsListItem::boostKeys() const
{
    return m_boostKeys;
}
