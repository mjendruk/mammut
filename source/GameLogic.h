#pragma once

#include <QVector>

#include "GameCamera.h"

class btDiscreteDynamicsWorld;
class btRigidBody;

class Cuboid;
class Mammoth;

class GameLogic
{
public:
    GameLogic();
    ~GameLogic();

    void update(int ms);
    void keyPressed(int key);
    void keyReleased(int key);
    //void assignCamera(RenderCamera * camera);
    const GameCamera & camera() const;

    const QVector<Cuboid *> & cuboids() const;
    const Mammoth * mammoth() const;

protected:
    void initialize();

protected:
    QVector<Cuboid *> m_cuboids;
    Mammoth * m_mammoth;
    GameCamera m_camera;

    btDiscreteDynamicsWorld * m_dynamicsWorld;
    btRigidBody * m_fallRigidBody;

    void initializeTestlevel();
};
