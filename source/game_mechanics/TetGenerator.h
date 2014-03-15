#pragma once

#include <QVector>

class Cuboid;
class tetgenio;

class TetGenerator
{
public:
    ~TetGenerator();

static QVector<glm::vec3> split();

protected:
    static tetgenio * tetcall_main();
};