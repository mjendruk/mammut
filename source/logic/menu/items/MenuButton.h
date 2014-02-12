#pragma once

#include <QString>
#include "MenuItem.h"

class MenuButton : public MenuItem
{
    Q_OBJECT

public:
    MenuButton(const QString & label);
    virtual ~MenuButton();

    void accept(MenuRenderer * renderer) override;
    void keyPressed(QKeyEvent * event) override;

    bool isFocusable() const override;

    const QString & label() const;

signals:
    void pressed();

protected:
    QString m_label;

};
