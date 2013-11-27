
#pragma once

#include <glm/glm.hpp>
#include <LinearMath/btMotionState.h>

class Mammut;

class MammutMotionState : public btMotionState
{
public:
    MammutMotionState(const glm::vec3 & translation, Mammut & mammut);
    ~MammutMotionState();

    virtual void getWorldTransform(btTransform & worldTransform) const;
    virtual void setWorldTransform(const btTransform & worldTransform);

protected:
    btTransform m_transform;
    Mammut & m_mammut;

};
