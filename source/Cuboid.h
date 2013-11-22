#pragma once

#include <glm/glm.hpp>

class btDiscreteDynamicsWorld;
class btRigidBody;


class Cuboid
{
public:
    Cuboid(btDiscreteDynamicsWorld * dynamicsWorld, const glm::vec3 & size, glm::vec3 translationVector);
         
    ~Cuboid();

    const glm::vec3 & size() const;
    const glm::mat4 & modelMatrix() const;
    void setModelMatrix(const glm::mat4 & modelMatrix);

    void update();

protected:
    glm::mat4 m_modelMatrix;
    glm::vec3 m_size;
    btDiscreteDynamicsWorld * m_dynamicsWorld;
    btRigidBody* m_rigidBody;
};
