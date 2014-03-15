#pragma once

#include <glm/glm.hpp>

#include <QVector>

#include "Tet.h"

namespace glowutils
{
    class AxisAlignedBoundingBox;
}

class Cuboid;
class tetgenio;

class TetGenerator
{
public:
    ~TetGenerator();

    static QVector<Tet *> * splitBox(const glowutils::AxisAlignedBoundingBox & box);

protected:
    static tetgenio * tetcall_main(const glowutils::AxisAlignedBoundingBox & box);
};