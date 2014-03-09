#pragma once

class btManifoldPoint;
class btRigidBody;
class btVector3;

class PhysicsObject
{
public:
    virtual ~PhysicsObject();

    virtual void collisionEvent(const PhysicsObject & object,
                                const btVector3 & collisionNormal);

    virtual btRigidBody * rigidBody() const = 0;

    virtual bool containsBoost() const;
    virtual void collectBoost() const;
};
