#pragma once

#include <QObject>
#include <QString>

class MenuItem : public QObject
{
    Q_OBJECT

public:
    MenuItem(const QString & label);
    ~MenuItem();

    void click();

    const QString & label() const;

signals:
    void clicked();

protected:
    QString m_label;

};
