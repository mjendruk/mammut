#include "GameCamera.h"

#include <glm/gtx/quaternion.hpp>

#include "Mammoth.h"

const glm::vec3 GameCamera::s_offset = glm::vec3(0.0f, 0.1f, 0.4f);


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
    m_eye = m_mammoth.position() + glm::vec3(glm::vec4(s_offset, 1.0) * glm::inverse(m_mammoth.rotation()));
    m_up = glm::vec3(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) *  glm::inverse(m_mammoth.rotation()));
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