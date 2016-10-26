#ifndef RWENGINE_COLLISIONINSTANCE_HPP
#define RWENGINE_COLLISIONINSTANCE_HPP
#include <btBulletDynamicsCommon.h>
#include <data/CollisionModel.hpp>
#include <string>
#include <vector>

class GameObject;
struct DynamicObjectData;
struct VehicleHandlingInfo;

/**
 * @brief CollisionInstance stores bullet body information
 */
class CollisionInstance {
public:
    CollisionInstance()
        : m_body(nullptr)
        , m_vertArray(nullptr)
        , m_motionState(nullptr)
        , m_collisionHeight(0.f) {
    }

    ~CollisionInstance();

    bool createPhysicsBody(GameObject* object, CollisionModel* collision,
                           DynamicObjectData* dynamics = nullptr,
                           VehicleHandlingInfo* handling = nullptr);

    btRigidBody* getBulletBody() const {
        return m_body;
    }

    float getBoundingHeight() const {
        return m_collisionHeight;
    }

    void changeMass(float newMass);

private:
    btRigidBody* m_body;
    std::vector<btCollisionShape*> m_shapes;
    btTriangleIndexVertexArray* m_vertArray;
    btMotionState* m_motionState;

    float m_collisionHeight;
};

#endif
