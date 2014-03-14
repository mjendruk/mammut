#pragma once

class QString;
typedef long long qint64;

class PerfCounter
{
public:
    static void begin(const QString & name);
    static void beginGL(const QString & name);
    static void end(const QString & name);
    static void endGL(const QString & name);
    static QString generateString();

protected:
    static void addNameToOrderedNames(const QString & name);
    static void addMeasurement(const QString & name, qint64 nanoseconds);
};
