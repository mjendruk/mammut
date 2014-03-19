#pragma once

#include <glm/glm.hpp>

#include <QObject>
#include <QThread>

namespace glowutils
{
    class AxisAlignedBoundingBox;
}

class Cuboid;
class tetgenio;


//implements the singleton pattern, see http://stackoverflow.com/questions/1008019/c-singleton-design-pattern
class TetGenerator : public QObject
{
    Q_OBJECT

public:
    static TetGenerator & instance();


public slots:
    void processCuboidAsync(Cuboid * cuboid);

signals:
    void gotANewCuboid(Cuboid * cuboid);

protected:
    TetGenerator();
    ~TetGenerator();

    TetGenerator(TetGenerator const &) = delete;
    void operator=(TetGenerator const &) = delete;

    void processCuboid(Cuboid * cuboid);
    static tetgenio * tetcall_main(const glowutils::AxisAlignedBoundingBox & box);

protected:
    QThread m_thread;
};
