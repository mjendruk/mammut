#pragma once

#include <QObject>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class GameCamera : public QObject
{
public:
    GameCamera();
    virtual ~GameCamera();

    void update(const glm::vec3 & position, float seconds);
    void gravityChangeEvent(const glm::mat3 & rotation);

    const glm::vec3 & eye() const;
    const glm::vec3 & center() const;
    const glm::vec3 & up() const;

protected:
    void updateRotationProgress(float seconds);
    void updateLookAt(const glm::vec3 & position);

    static const glm::vec3 s_centerOffset;
    static const glm::vec3 s_eyeOffset;
	static const float s_rotationDuration;

    glm::vec3 m_eye;
    glm::vec3 m_center;
    glm::vec3 m_up;

    glm::mat3 m_initialRotation;
    glm::mat3 m_targetRotation;
	glm::mat3 m_currentRotation;
	float m_rotationProgress;

};
