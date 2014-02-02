#include "GameCamera.h"

#include <iostream>
#include <algorithm>
#include <QDebug>

#include "glm/gtx/string_cast.hpp"

#include "Mammut.h"

const glm::vec3 GameCamera::s_centerOffset = glm::vec3(0.0f, 0.0f, -3.0f);
const glm::vec3 GameCamera::s_eyeOffset = glm::vec3(0.0f, 0.1f, 0.3f);
const float GameCamera::rotationDuration = 0.25;

GameCamera::GameCamera(Mammut & mammut)
:   m_mammut(mammut)
{
}

GameCamera::~GameCamera()
{
}

void GameCamera::update(float seconds)
{
	if (m_lastRotation != m_mammut.rotation()) {
		m_rotationProgress = 0.0;
		m_lastRotation = m_mammut.rotation();
		m_oldRotation = m_currentRotation;
	}

	m_rotationProgress = std::min(m_rotationProgress + seconds, rotationDuration);

	m_currentRotation = calculateRotation(m_oldRotation, glm::inverse(m_mammut.rotation()), m_rotationProgress);

	m_center = m_mammut.position() + s_centerOffset;
	m_eye = m_mammut.position() + glm::vec3(glm::vec4(s_eyeOffset, 1.0) * m_currentRotation);
	m_up = glm::normalize(glm::vec3((m_eye - m_center).xy, 0.0));
}

const glm::mat4 GameCamera::calculateRotation(const glm::mat4 & oldRotation, const glm::mat4 & targetRotation, float rotationProgress)
{
	//convert to angles
	float oldAngle = glm::degrees(atan2(oldRotation[0][1], oldRotation[0][0]));
	float targetAngle = glm::degrees(atan2(targetRotation[0][1], targetRotation[0][0]));

	//prevent 270° turns
	if (targetAngle - oldAngle > 181)
		oldAngle += 360;
	if (oldAngle - targetAngle > 181)
		targetAngle += 360;

	//convert to quaternions and slerp them
	glm::quat from = glm::rotate(glm::quat(), oldAngle, glm::vec3(0.0, 0.0, 1.0));
	glm::quat to = glm::rotate(glm::quat(), targetAngle, glm::vec3(0.0, 0.0, 1.0));
	return glm::mat4_cast(glm::mix(from, to, glm::smoothstep(0.0f, 1.0f, rotationProgress / rotationDuration)));
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
