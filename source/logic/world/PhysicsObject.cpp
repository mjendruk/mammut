#include "PhysicsObject.h"

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::collisionEvent(const PhysicsObject & object, 
    const btVector3 & collisionNormal)
{
}

bool PhysicsObject::containsBoost() const
{
    return false;
}

void PhysicsObject::collectBoost() const
{
}
