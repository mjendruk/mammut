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

	const QVector<Quad *> & quads() const;

protected:
	void initialize();

protected:
	QVector<Quad *> m_quads;

	btDiscreteDynamicsWorld * m_dynamicsWorld;
	btRigidBody * m_fallRigidBody;
};
