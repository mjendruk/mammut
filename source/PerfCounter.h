#pragma once

class QString;

class PerfCounter
{
public:
    static void begin(QString name);
    static void end(QString name);
    static QString getString();
};
