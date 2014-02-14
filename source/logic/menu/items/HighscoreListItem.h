#pragma once

#include <QList> 
#include "MenuItem.h"

class HighscoreEntry;

class HighscoreListItem : public MenuItem
{
    Q_OBJECT

public:
    HighscoreListItem(const QList<HighscoreEntry> & scores);
    virtual ~HighscoreListItem();

    void accept(MenuRenderer * renderer) override;
    void keyPressed(QKeyEvent * event) override;

    bool isFocusable() const override;

    const QList<HighscoreEntry> & scores() const;

protected:
    QList<HighscoreEntry> m_scores;

};
