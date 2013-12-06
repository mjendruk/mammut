#include "GameCamera.h"

#include "Mammut.h"

const glm::vec3 GameCamera::s_centerOffset = glm::vec3(0.0f, 0.1f, -3.0f);
const glm::vec3 GameCamera::s_eyeOffset = glm::vec3(0.0f, 0.1f, 0.3f);

GameCamera::GameCamera(Mammut & mammut)
:   m_mammut(mammut)
{
}

GameCamera::~GameCamera()
{
}

void GameCamera::update()
{
    m_center = m_mammut.position() + s_centerOffset;
    m_eye = m_mammut.position() + glm::vec3(glm::vec4(s_eyeOffset, 1.0) * glm::inverse(m_mammut.rotation()));
    m_up = glm::vec3(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) *  glm::inverse(m_mammut.rotation()));
}

const glm::vec3 & GameCamera::eye() const
{
    return m_eye;
}

const glm::vec3 & GameCamera::center() const
{
    return m_center;
}

const glm::vec3 & GameCamera::up() const
{
    return m_up;
}
