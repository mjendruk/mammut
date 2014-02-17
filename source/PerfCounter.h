#pragma once

class QString;

class PerfCounter
{
public:
    static void begin(const QString & name);
    static void end(const QString & name);
    static QString generateString();
};
