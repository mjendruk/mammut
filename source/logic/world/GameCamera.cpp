#include "GameCamera.h"

#include <algorithm>

#include <QDebug>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Util.h>

const float GameCamera::s_rotationDuration = 0.25f;

GameCamera::GameCamera()
:   m_currentCenterOffset(0.0f)
{
}

GameCamera::~GameCamera()
{
}

void GameCamera::update(const glm::vec3 & position, const glm::vec3 & direction, float seconds)
{
    updateRotationProgress(seconds);
    updateLookAt(position, direction);
}

void GameCamera::gravityChangeEvent(const glm::mat3 & rotation)
{
    m_initialRotation = m_currentRotation;
    m_targetRotation = rotation;
    m_rotationProgress = 0.0f;
}

void GameCamera::updateRotationProgress(float seconds)
{
    m_rotationProgress = std::min(m_rotationProgress + seconds, s_rotationDuration);
    
    // convert to angles
    const float initialAngle = glm::degrees(atan2(m_initialRotation[0][1], m_initialRotation[0][0]));
    const float targetAngle = glm::degrees(atan2(m_targetRotation[0][1], m_targetRotation[0][0]));

    // convert to quaternions and slerp them
    glm::quat from = glm::rotate(glm::quat(), initialAngle, glm::vec3(0.0, 0.0, 1.0));
    glm::quat to = glm::rotate(glm::quat(), targetAngle, glm::vec3(0.0, 0.0, 1.0));
    
    m_currentRotation = glm::mat3_cast(glm::slerp(from, to, glm::smoothstep(0.0f, 1.0f, m_rotationProgress / s_rotationDuration)));
}

void GameCamera::updateLookAt(const glm::vec3 & position, const glm::vec3 & direction)
{
    const glm::vec3 defaultLookAt(0.0f, 0.0f, -1.0f);
    
    glm::vec3 realDirection = glm::length(direction) == 0.0f ? glm::vec3(0.0f, 0.0f, -1.0f) : direction;
    
    glm::vec3 centerOffset = glm::normalize(glm::normalize(realDirection) + defaultLookAt);
    m_currentCenterOffset = glm::mix(centerOffset, m_currentCenterOffset, 0.91f);
    
    m_eye = glm::vec3(glm::mix(m_eye.xy(), position.xy(), 0.5f), position.z);
    m_center = m_eye + m_currentCenterOffset;
    m_up = Util::toCartesian(glm::mat4(m_currentRotation) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
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
