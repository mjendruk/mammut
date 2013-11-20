#pragma once

#include <QVector>

#include "KeyHandler.h"

class btDiscreteDynamicsWorld;
class btRigidBody;

class Cuboid;

class GameLogic
{
public:
    GameLogic();
    ~GameLogic();

    void update(int ms);

    const QVector<Cuboid *> & cuboids() const;
    KeyHandler & keyHandler();

protected:
    void initialize();

protected:
    KeyHandler m_keyHandler;
    QVector<Cuboid *> m_cuboids;

    btDiscreteDynamicsWorld * m_dynamicsWorld;
    btRigidBody * m_fallRigidBody;
};
