#include "GameCamera.h"

#include <iostream>
#include <algorithm>
#include <QDebug>

#include "glm/gtx/string_cast.hpp"

#include "Mammut.h"

const glm::vec3 GameCamera::s_centerOffset = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 GameCamera::s_eyeOffset = glm::vec3(0.0f, 0.05f, 0.0f);
const float GameCamera::s_rotationDuration = 0.25f;

GameCamera::GameCamera()
{
}

GameCamera::~GameCamera()
{
}

void GameCamera::update(const glm::vec3 & position, float seconds)
{
	updateRotationProgress(seconds);
    updateLookAt(position);
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

void GameCamera::updateLookAt(const glm::vec3 & position)
{
    m_center = position + s_centerOffset;
    
    const glm::vec4 homogenousEyeOffset = glm::mat4(m_currentRotation) * glm::vec4(s_eyeOffset, 1.0);
	m_eye = position + homogenousEyeOffset.xyz() / homogenousEyeOffset.w;
    
    const glm::vec4 homogenousUp = glm::mat4(m_currentRotation) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	m_up = homogenousUp.xyz() / homogenousUp.w;
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
