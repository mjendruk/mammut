
#include "MammutMotionState.h"

#include "Conversions.h"
#include "Mammut.h"

MammutMotionState::MammutMotionState(const glm::vec3 & translation, Mammut & mammut)
:   m_mammut(mammut)
{
    m_transform = btTransform(btQuaternion(0, 0, 0, 1),
                                     Conversions::toBtVec3(translation));
}

MammutMotionState::~MammutMotionState()
{

}

void MammutMotionState::getWorldTransform(btTransform & worldTransform) const
{
    worldTransform = m_transform;
}

void MammutMotionState::setWorldTransform(const btTransform & worldTransform)
{
    m_transform = worldTransform;
    
    glm::mat4 mat;
    mat *= Conversions::toGlmMat4(worldTransform.getOrigin());
    mat *= Conversions::toGlmMat4(worldTransform.getRotation());
    
    m_mammut.setPosition(Conversions::toGlmVec3(worldTransform.getOrigin()));
    m_mammut.setRotation(Conversions::toGlmMat4(worldTransform.getRotation()));
}
