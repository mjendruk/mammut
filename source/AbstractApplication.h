#pragma once

#include <QApplication>

// Application initializes Application strings from CMake generated Meta.

class AbstractApplication : public QApplication
{
public:
    AbstractApplication(
        int & argc
    ,   char ** argv);

    virtual ~AbstractApplication();

    virtual const QString title() const;
};