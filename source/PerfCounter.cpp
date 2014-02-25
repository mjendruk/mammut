#include "PerfCounter.h"

#include <cassert>

#include <GL/glew.h>

#include <QString>
#include <QElapsedTimer>
#include <QMap>
#include <QVector>

namespace
{
    static QMap<QString, double> map;
    static QMap<QString, QElapsedTimer *> timerMap;
    static QVector<QString> orderedNames;
    static const float smoothingFactor = 0.95f;

    static QMap<QString, GLuint> glTimerMap;
    static QString runningGLQuery("");
}

void PerfCounter::begin(const QString & name)
{
    assert(!timerMap.contains(name));
    timerMap[name] = new QElapsedTimer();
    timerMap[name]->restart();
}

void PerfCounter::end(const QString & name)
{
    assert(timerMap.contains(name));

    qint64 elapsedTime = timerMap[name]->nsecsElapsed();
    delete timerMap[name];
    timerMap.remove(name);

    addNameToOrderedNames(name);

    addMeasurement(name, elapsedTime);
}

void PerfCounter::beginGL(const QString & name)
{
    assert(runningGLQuery == QString(""));
    runningGLQuery = name;

    GLuint queryID;
    if (!glTimerMap.contains(name)) {
        glGenQueries(1, &queryID);
        glTimerMap[name] = queryID;
    } else {
        queryID = glTimerMap[name];
        GLuint elapsedTimeNs;
        glGetQueryObjectuiv(queryID, GL_QUERY_RESULT, &elapsedTimeNs);

        addMeasurement(name, elapsedTimeNs);
    }

    glBeginQuery(GL_TIME_ELAPSED, queryID);
}

void PerfCounter::endGL(const QString & name)
{
    assert(glTimerMap.contains(name));
    assert(runningGLQuery == name);

    addNameToOrderedNames(name);

    runningGLQuery = "";
    glEndQuery(GL_TIME_ELAPSED);
}

QString PerfCounter::generateString()
{
    QString result;
    for (QString name : orderedNames)
        result += QString("%1: %2 ").arg(name).arg(map[name] / 1000000.0, 1, 'f', 2);
    return result.trimmed();
}

void PerfCounter::addNameToOrderedNames(const QString & name)
{
    if (!orderedNames.contains(name))
        orderedNames << name;
}

void PerfCounter::addMeasurement(const QString & name, qint64 nanoseconds)
{
    if (map[name] == 0.0f)
        map[name] = nanoseconds;
    else
        map[name] = nanoseconds * (1 - smoothingFactor) + map[name] * smoothingFactor;
}


