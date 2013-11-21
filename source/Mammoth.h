#pragma once

#include <glm/glm.hpp>

class btDiscreteDynamicsWorld;
class btRigidBody;

class Mammoth
{
public:
    //Cuboid(btDiscreteDynamicsWorld dynamicsWorld, const glm::vec3 & size);
    
    Mammoth(btDiscreteDynamicsWorld * dynamicsWorld, const glm::vec3 & size, glm::vec3 translationVector = glm::vec3());
         
    ~Mammoth();

    const glm::vec3 & size() const;
    const glm::mat4 & modelMatrix() const;
    void setModelMatrix(const glm::mat4 & modelMatrix);

    void updatePhysics();

protected:
    glm::mat4 m_modelMatrix;
    glm::vec3 m_size;
    btDiscreteDynamicsWorld * m_dynamicsWorld;
    btRigidBody* m_rigidBody;
};
