#pragma once

#include <QString>
#include "MenuItem.h"

class MenuText : public MenuItem
{
    Q_OBJECT

public:
    MenuText(const QString & text);
    virtual ~MenuText();

    void accept(MenuRenderer * renderer) override;
    void keyPressed(QKeyEvent * event) override;

    bool isFocusable() const override;

    const QString & text() const;

protected:
    QString m_text;

};
