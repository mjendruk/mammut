#include "GameCamera.h"

#include <algorithm>

#include <QDebug>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/random.hpp>

#include <Util.h>

const float GameCamera::s_rotationDuration = 0.25f;

GameCamera::GameCamera()
:   m_currentCenterOffset(0.0f)
,   m_noise(Sound::kDeepNoise, true)
{
    m_noise.setVolume(0.0f);
    m_noise.setPaused(false);
}

GameCamera::~GameCamera()
{
    m_noise.setPaused(true);
}

void GameCamera::update(
    const glm::vec3 & position,
    const glm::vec3 & direction,
    float seconds,
    float normalizedCaveDistance)
{
    updateRotationProgress(seconds);
    updateLookAt(position, direction, normalizedCaveDistance);
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

float linearstep(float edge0, float edge1, float x)
{
    return (glm::clamp(x, edge0, edge1) - edge0) / (edge1 - edge0);
}

float calculateShakiness(float normalizedCaveDistance)
{
    return glm::pow(linearstep(0.3f, 0.9f, normalizedCaveDistance), 2.0f);
}

void GameCamera::updateLookAt(const glm::vec3 & position, const glm::vec3 & direction, float normalizedCaveDistance)
{
    const glm::vec3 defaultLookAt(0.0f, 0.0f, -1.0f);
    const glm::vec3 realDirection = (glm::length(direction) == 0.0f) ? glm::vec3(0.0f, 0.0f, -1.0f) : direction;
    
    float shakiness = calculateShakiness(normalizedCaveDistance);

    m_noise.setVolume(shakiness * 0.95);
    
    glm::vec3 shakeLookAt = glm::normalize(glm::vec3(glm::gaussRand(0.0f, 8.0f),
                                                     glm::gaussRand(0.0f, 8.0f),
                                                     glm::linearRand(0.1f, 0.3f)));
    
    glm::vec3 lookAt = glm::mix(defaultLookAt, shakeLookAt, shakiness);
    
    glm::vec3 centerOffset = glm::normalize(glm::normalize(realDirection) + lookAt);
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
