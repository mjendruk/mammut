#pragma once

#include <memory>

#include <QVector>

class btDiscreteDynamicsWorld;
class btRigidBody;

class Cuboid;
class GameCamera;
class Mammut;

class GameLogic
{
public:
    GameLogic();
    ~GameLogic();

    void update(int ms);
    void keyPressed(int key);
    void keyReleased(int key);
    const GameCamera & camera() const;

    const QVector<Cuboid *> & cuboids() const;
    const Mammut & mammut() const;

protected:
    enum gravity
    {
        up = 0,
        right = 1,
        down = 2,
        left = 3
    };

    QVector<Cuboid *> m_cuboids;
    std::unique_ptr<Mammut> m_mammut;
    std::unique_ptr<GameCamera> m_camera;

    btDiscreteDynamicsWorld * m_dynamicsWorld;
    btRigidBody * m_fallRigidBody;

    int m_activeGravity = gravity::down;

    void initializeTestlevel();
    void changeGravity(int delta);
};
