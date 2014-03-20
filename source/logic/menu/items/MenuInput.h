#pragma once

#include <QString>
#include "MenuItem.h"

class MenuInput : public MenuItem
{
    Q_OBJECT

public:
    MenuInput(const QString & label,
              const QString & initialText);
    
    virtual ~MenuInput();

    void accept(MenuRenderer * renderer) override;
    void keyPressed(QKeyEvent * event) override;

    bool isFocusable() const override;

    const QString & label() const;
    const QString & text() const;

signals:
    void enterPressed(const QString & text);

protected:
    QString m_label;
    QString m_text;

};
