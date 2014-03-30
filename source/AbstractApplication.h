#pragma once

#include <QGuiApplication>

// Application initializes Application strings from CMake generated Meta.

class AbstractApplication : public QGuiApplication
{
public:
    AbstractApplication(
        int & argc
    ,   char ** argv);

    virtual ~AbstractApplication();

    virtual const QString title() const;
};