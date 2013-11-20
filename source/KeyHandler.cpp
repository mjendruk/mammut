 #include "KeyHandler.h"

#include <QEvent>
#include <QDebug>



KeyHandler::KeyHandler()
{

}

KeyHandler::~KeyHandler()
{

}

 bool KeyHandler::eventFilter(QObject *obj, QEvent *event)
 {
     if (event->type() == QEvent::KeyPress) {
         qDebug() << "hi";
     } else {
         // standard event processing
         return QObject::eventFilter(obj, event);
     }
 }