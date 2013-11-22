#include "GameCamera.h"

#include <glm/gtx/quaternion.hpp>

#include "Mammoth.h"


GameCamera::GameCamera(Mammoth & mammoth)
:   m_mammoth(mammoth)
{
}

GameCamera::~GameCamera()
{
    
}

void GameCamera::update()
{
    m_center = m_mammoth.position();
    m_eye = m_mammoth.position() + glm::vec3((glm::vec4(0.0f, 2.0f, 2.0f, 1.0f) * glm::inverse(m_mammoth.rotation())));
    m_up = glm::vec3(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) *  glm::inverse(m_mammoth.rotation()));
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