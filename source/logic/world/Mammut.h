#pragma once

#include <QObject>
#include <glm/glm.hpp>

#include "MammutPhysics.h"

class Mammut : public QObject
{
    Q_OBJECT
    
public:
    Mammut(const glm::vec3 & translation);
    ~Mammut();
    
    void update();
    
    void gravityChangeEvent(const glm::mat3 & rotation);
    void collisionEvent(const PhysicsObject & object,
                        const glm::vec3 & collisionNormal);
    
    glm::mat4 modelTransform() const;
    
    glm::vec3 position() const;
    glm::vec3 velocity() const;
    
    glm::mat4 rotation() const;
    
    MammutPhysics * physics();
    
signals:
    void crashed();

protected:
    void slowDownDrifting();
    bool isStillOnObject() const;
    
protected:
    static const glm::vec3 s_size;
    
    MammutPhysics m_physics;
    
    glm::mat3 m_gravityTransform;
    bool m_isOnObject;
    bool m_isCrashed;
    
};
