#pragma once

#include <QVector>

class btDiscreteDynamicsWorld;
class btRigidBody;

class Quad;

class GameLogic
{
public:
    GameLogic();
    ~GameLogic();

    void update(int ms);

	QVector<Quad *> * quads();

protected:
	void initialize();
	bool m_initialized;

	QVector<Quad *> m_quads;

	btDiscreteDynamicsWorld * m_dynamicsWorld;
	btRigidBody * m_fallRigidBody;
};
