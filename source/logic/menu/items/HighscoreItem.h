#pragma once

#include <QList> 
#include "MenuItem.h"

class HighscoreEntry;

class HighscoreItem : public MenuItem
{
    Q_OBJECT

public:
    HighscoreItem(const QList<HighscoreEntry> & scores);
    virtual ~HighscoreItem();

    void accept(MenuRenderer * renderer) override;
    void keyPressed(QKeyEvent * event) override;

    bool isFocusable() const override;

    const QList<HighscoreEntry> & scores() const;

protected:
    QList<HighscoreEntry> m_scores;

};
