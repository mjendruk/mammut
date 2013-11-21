#include "GameCamera.h"


GameCamera::GameCamera(const glm::vec3 & eye, const glm::vec3 & center,
    const glm::vec3 & up)
:    m_eye(eye)
,    m_center(center)
,    m_up(up)
,    m_panVector(glm::vec3(0.f))
{

}

GameCamera::~GameCamera()
{
    
}

void GameCamera::pan()
{
    m_eye += m_panVector;
    m_center += m_panVector;
}

void GameCamera::setPanVector(glm::vec3 pan)
{
    m_panVector = pan;
}

void GameCamera::modifyPanVector(glm::vec3 panDelta)
{
    m_panVector += panDelta;
}

glm::vec3 GameCamera::panVector()
{
    return m_panVector;
}


const glm::vec3 GameCamera::eye() const
{
    return m_eye;
}

const glm::vec3 GameCamera::center() const
{
    return m_center;
}
const glm::vec3 GameCamera::up() const
{
    return m_up;
}