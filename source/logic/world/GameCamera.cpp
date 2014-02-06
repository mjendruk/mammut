#include "GameCamera.h"

#include <iostream>
#include <algorithm>
#include <QDebug>

#include "glm/gtx/string_cast.hpp"

#include "Mammut.h"
#include <Util.h>

const float GameCamera::s_rotationDuration = 0.25f;

GameCamera::GameCamera()
:   m_currentEyeOffset(0.0f)
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
    const glm::vec3 standardLookAt(0.0f, 0.0f, -1.0f);
    
    glm::vec3 clampedDirection = glm::vec3(direction.xy(), glm::min(direction.z, 0.0f));
    
    glm::vec3 eyeOffset = glm::normalize(glm::normalize(clampedDirection) + standardLookAt);
    m_currentEyeOffset = glm::mix(eyeOffset, m_currentEyeOffset, 0.4f);
    
    m_eye = position;
    m_center = position + m_currentEyeOffset;
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
