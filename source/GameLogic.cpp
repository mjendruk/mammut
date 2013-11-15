#include "GameLogic.h"

#include <QThread>

#include "Application.h"

GameLogic::GameLogic()
{

}  

GameLogic::~GameLogic()
{

}

void GameLogic::update(int ms)
{
	//Do heavy weight lifting
	QThread::msleep(2);
}