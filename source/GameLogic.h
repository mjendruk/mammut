#pragma once

#include <memory>

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
    const Mammoth & mammoth() const;

protected:
    QVector<Cuboid *> m_cuboids;
    std::unique_ptr<Mammoth> m_mammoth;
    std::unique_ptr<GameCamera> m_camera;

    btDiscreteDynamicsWorld * m_dynamicsWorld;
    btRigidBody * m_fallRigidBody;

    void initializeTestlevel();
};
