#pragma once

#include <QList> 
#include "MenuItem.h"

class HighscoreEntry;

class CreditsItem : public MenuItem
{
    Q_OBJECT

public:
    CreditsItem();
    virtual ~CreditsItem();

    void accept(MenuRenderer * renderer) override;
    void keyPressed(QKeyEvent * event) override;

    bool isFocusable() const override;

    const QList<QString> & creditsForenames() const;
    const QList<QString> & creditsNicknames() const;
    const QList<QString> & creditsSurnames() const;
    const QList<QString> & thanks() const;

protected:
    const QList<QString> m_creditsForenames;
    const QList<QString> m_creditsNicknames;
    const QList<QString> m_creditsSurnames;
    const QList<QString> m_thanks;

};
