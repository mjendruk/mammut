#include "MenuItem.h"

MenuItem::MenuItem(const QString & label)
:   m_label(label)
{
}

MenuItem::~MenuItem()
{
}

void MenuItem::click()
{
    emit clicked();
}

const QString & MenuItem::label() const
{
    return m_label;
}
