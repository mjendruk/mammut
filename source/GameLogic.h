#pragma once

#include <QVector>

class btDiscreteDynamicsWorld;
class btRigidBody;

class Cuboid;

class GameLogic
{
public:
    GameLogic();
    ~GameLogic();

    void update(int ms);
    void keyPressed(int key);
    void keyReleased(int key);

    const QVector<Cuboid *> & cuboids() const;

protected:
    void initialize();

protected:
    QVector<Cuboid *> m_cuboids;

    btDiscreteDynamicsWorld * m_dynamicsWorld;
    btRigidBody * m_fallRigidBody;
};
