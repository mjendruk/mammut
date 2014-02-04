#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Mammut;

class GameCamera
{
public:
    GameCamera(Mammut & mammut);

    virtual ~GameCamera();

    void update(float seconds);


    const glm::vec3 & eye() const;
    const glm::vec3 & center() const;
    const glm::vec3 & up() const;

protected:
	static const glm::mat4 calculateRotation(const glm::mat4 & oldRotation, const glm::mat4 & targetRotation, float rotationProgress);

    static const glm::vec3 s_centerOffset;
	static const float rotationDuration;

    glm::vec3 m_eye;
    glm::vec3 m_center;
    glm::vec3 m_up;

	glm::mat4 m_lastRotation;
	glm::mat4 m_oldRotation;
	glm::mat4 m_currentRotation;
	float m_rotationProgress;

    Mammut & m_mammut;
};
