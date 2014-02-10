#pragma once

#include <QObject>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class GameCamera : public QObject
{
public:
    GameCamera();
    virtual ~GameCamera();

    void update(const glm::vec3 & position,
                const glm::vec3 & direction,
                float seconds);
    
    void gravityChangeEvent(const glm::mat3 & rotation);

    const glm::vec3 & eye() const;
    const glm::vec3 & center() const;
    const glm::vec3 & up() const;

protected:
    void updateRotationProgress(float seconds);
    void updateLookAt(const glm::vec3 & position,
                      const glm::vec3 & direction);

    static const float s_rotationDuration;

    glm::vec3 m_eye;
    glm::vec3 m_center;
    glm::vec3 m_up;
    
    glm::vec3 m_currentCenterOffset;

    glm::mat3 m_initialRotation;
    glm::mat3 m_targetRotation;
    glm::mat3 m_currentRotation;
    float m_rotationProgress;

};
