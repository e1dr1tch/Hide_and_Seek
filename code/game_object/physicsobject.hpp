#pragma once

//native
#include <iostream>
#include <vector>

//bullet
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

using namespace std;

class CompoundShape
{
    private:
        btCompoundShape* shape;
        vector < btCollisionShape* > childShapes;
    
    public:
        CompoundShape();

        void setShape(btCompoundShape* shape);

        void add(btCollisionShape* childShape, btVector3 position, btQuaternion rotation = btQuaternion(btVector3(0, 0, 1), 0));

        btCompoundShape* getShape() const;
        btCollisionShape* getChildShape(unsigned int index) const;
        btTransform getChildTransform(unsigned int index) const;

        ~CompoundShape();
};

class PhysicsObject
{
    private:
        btDynamicsWorld* world;

        bool collidable;

        float mass;
        btCollisionShape* phShape;
        CompoundShape* comShape;
        btRigidBody* body;

        OpenGLMotionState* motionState;

        void updateBody(btCollisionShape* shape, float mass, btVector3 position, btQuaternion rotation);

    public:
        PhysicsObject(btDynamicsWorld* world);
        PhysicsObject(btDynamicsWorld* world, btCollisionShape* shape, float mass, btVector3 position, btQuaternion rotation = btQuaternion(btVector3(0, 0, 1), 0));
        PhysicsObject(btDynamicsWorld* world, CompoundShape* shape, float mass, btVector3 position, btQuaternion rotation);

        void setShape(btCollisionShape* shape);
        void setShape(CompoundShape* shape);
        void setMass(float mass);
        void setPosition(btVector3 position);
        void setRotation(btQuaternion rotation);
        void setCollidable(bool collidable);

        float getMass() const;
        btCollisionShape* getShape() const;
        CompoundShape* getCompoundShape() const;
        btRigidBody* getRigidBody() const;
        bool isCollidable() const;

        btScalar* getTransform() const;

        ~PhysicsObject();
};
