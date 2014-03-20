#pragma once

#include <QObject>
#include <glm/glm.hpp>

#include "MammutPhysics.h"

class Mammut : public QObject
{
    Q_OBJECT
    
public:
    enum class BoostDirection{kUp, kRight, kDown, kLeft};

    Mammut(const glm::vec3 & translation);
    ~Mammut();
    
    void update();
    
    void gravityChangeEvent(const glm::mat3 & rotation);
    void collisionEvent(const PhysicsObject & object,
                        const glm::vec3 & collisionNormal);
    
    void caveCollisionEvent();

    glm::mat4 modelTransform() const;
    
    glm::vec3 position() const;
    glm::vec3 velocity() const;
    glm::mat4 rotation() const;

    void addZShift(float zShift);
    
    MammutPhysics * physics();

    int collectedBoosts() const;
    void applyBoost(BoostDirection direction);
    
signals:
    void crashed();

protected:
    void slowDownDrifting();
    void crash();

    bool isStillOnObject() const;

    void collectBoostFromObject(const PhysicsObject & object);
    void updateBoostState();
    
protected:
    static const glm::vec3 s_size;
    static const int s_maxNumBoosts;
    
    MammutPhysics m_physics;
    
    glm::mat3 m_gravityTransform;
    bool m_isOnObject;
    bool m_isCrashed;

    bool m_boostIsActive;
    unsigned int m_collectedBoosts;
    int physicSteps;

    double m_zDistance;
};
