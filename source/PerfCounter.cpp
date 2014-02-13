#include "PerfCounter.h"

#include <cassert>

#include <QString>
#include <QElapsedTimer>
#include <QMap>
#include <QVector>

namespace
{
    static QMap<QString, double> map;
    static QMap<QString, QElapsedTimer*> timerMap;
    static QVector<QString> orderedNames;
    static const float smoothingFactor = 0.95f;
}

void PerfCounter::begin(QString name)
{
    assert(!timerMap.contains(name));
    timerMap[name] = new QElapsedTimer();
    timerMap[name]->restart();
}

void PerfCounter::end(QString name)
{
    assert(timerMap.contains(name));

    qint64 elapsedTime = timerMap[name]->nsecsElapsed();
    delete timerMap[name];
    timerMap.remove(name);

    if (!map.contains(name)) {
        map[name] = elapsedTime;
        orderedNames << name;
    }
    else {
        map[name] = elapsedTime * (1 - smoothingFactor) + map[name] * smoothingFactor;
    }
}

QString PerfCounter::getString()
{
    QString result;
    for (QString name : orderedNames)
        result += QString("%1: %2 ").arg(name).arg(map[name] / 1000000.0, 1, 'f', 2);
    return result.trimmed();
}