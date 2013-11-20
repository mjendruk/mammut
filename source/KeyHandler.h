#pragma once

#include <QObject>

class QEvent;


class KeyHandler : public QObject
{
	Q_OBJECT

public:
    KeyHandler();

    virtual ~KeyHandler();


 protected:
     bool eventFilter(QObject *obj, QEvent *event);
};