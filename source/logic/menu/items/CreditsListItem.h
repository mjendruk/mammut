#pragma once

#include <QList> 
#include "MenuItem.h"

class HighscoreEntry;

class CreditsListItem : public MenuItem
{
    Q_OBJECT

public:
    CreditsListItem();
    virtual ~CreditsListItem();

    void accept(MenuRenderer * renderer) override;
    void keyPressed(QKeyEvent * event) override;

    bool isFocusable() const override;

    const QList<QString> & creditsForenames() const;
    const QList<QString> & creditsSurnames() const;
    const QList<QString> & thanks() const;

protected:
    static const QList<QString> m_creditsForenames;
    static const QList<QString> m_creditsSurnames;
    static const QList<QString> m_thanks;

};
