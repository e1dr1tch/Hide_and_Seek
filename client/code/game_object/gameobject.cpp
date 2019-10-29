#include "../global/globaluse.hpp"

#include "../shader/shader.hpp"

#include "../window/renderquad.hpp"
#include "../window/glfwevents.hpp"
#include "../window/window.hpp"

#include "../player/camera.hpp"

#include "../debug/debugdrawer.hpp"

#include "../world/raytracer.hpp"
#include "../world/constrainthandler.hpp"
#include "../world/bulletevents.hpp"
#include "../world/world.hpp"

#include "sphere.hpp"
#include "openglmotionstate.hpp"
#include "animation.hpp"
#include "mesh.hpp"
#include "bone.hpp"
#include "skeleton.hpp"
#include "viewfrustum.hpp"
#include "boundsphere.hpp"
#include "modelloader.hpp"
#include "physicsobject.hpp"
#include "gameobject.hpp"

set < string > GameObject::globalNames;

GameObject::GameObject(string name)
{
    if (globalNames.find(name) != globalNames.end())
    {
        throw runtime_error("ERROR::GameObject invalid name");
    }

    globalNames.insert(name);
    this->name = name;
    setVisible(true);
    setShadow(true);
    setCull(true);
        
    minNormalCosAngle = 0.0;

    modelLoader = new ModelLoader();
    physicsObject = nullptr;

    skeleton = nullptr;
    viewFrustum = nullptr;
    boundSphere = nullptr;
    sphere = nullptr;

    interpolation = false;
    interpolationCoeff = 1.0;
    localTransform = nextTransform = prevTransform = mat4(1.0);
    ready = true;

    userPointer = nullptr;
}

void GameObject::removePhysicsObject()
{
    if (physicsObject)
    {
        delete physicsObject;
        physicsObject = nullptr;
    }
}

void GameObject::removeGraphicsObject()
{
    for (size_t i = 0; i < meshes.size(); i++)
    {
        delete meshes[i];
    }
    meshes.clear();

    delete skeleton;
    skeleton = nullptr;

    delete boundSphere;
    boundSphere = nullptr;

    graphicsObject = "";
    modelLoader->clear();
}

void GameObject::setName(string name)
{
    if (globalNames.find(name) != globalNames.end())
    {
        throw runtime_error("ERROR::GameObject invalid name");
    }

    globalNames.erase(this->name);
    globalNames.insert(name);
    this->name = name;
}
        
void GameObject::setCull(bool cull)
{
    this->cull = cull;
}
        
void GameObject::setMinNormalCosAngle(float minNormalCosAngle)
{
    this->minNormalCosAngle = minNormalCosAngle;
}
        
void GameObject::setVisible(bool visible)
{
    this->visible = visible;
}

void GameObject::setShadow(bool shadow)
{
    this->shadow = shadow;
}
        
void GameObject::setCollidable(bool collidable)
{
    if (physicsObject)
    {
        physicsObject->setCollidable(collidable);
    }
}

void GameObject::setStatic(bool stat)
{
    if (physicsObject)
    {
        physicsObject->setStatic(stat);
    }
}

void GameObject::setGraphicsObject(string path)
{
    removeGraphicsObject();
    graphicsObject = path;

    modelLoader->loadModel(path);
    modelLoader->getModelData(skeleton, meshes);
}
        
void GameObject::setViewFrustum(ViewFrustum* viewFrustum)
{
    this->viewFrustum = viewFrustum;
}

void GameObject::setPhysicsObject(PhysicsObject* object)
{
    removePhysicsObject();

    physicsObject = object;
    physicsObject->setUserPointer(this);
}

void GameObject::setUserPointer(void* userPointer)
{
    this->userPointer = userPointer;
}
        
void GameObject::createBoundSphere()
{
    boundSphere = new BoundSphere(meshes);
    boundSphere->construct();
}

void GameObject::setLocalRotation(vec3 axis, float angle, bool add)
{
    unique_lock < mutex > lk(mtx);
    
    while (!ready)
    {
        cv.wait(lk);
    }

    vec3 sc;
    quat rot;
    vec3 tran;
    vec3 skew;
    vec4 perspective;

    decompose(localTransform, sc, rot, tran, skew, perspective);

    localTransform = mat4(1.0);
    localTransform *= scale(sc); 
    localTransform *= translate(tran);
    localTransform *= rotate(radians(angle), axis);

    if (add)
    {
        localTransform *= toMat4(conjugate(rot));
    }
}

void GameObject::setLocalScale(vec3 growth, bool add)
{
    unique_lock < mutex > lk(mtx);
    
    while (!ready)
    {
        cv.wait(lk);
    }

    vec3 sc;
    quat rot;
    vec3 tran;
    vec3 skew;
    vec4 perspective;

    decompose(localTransform, sc, rot, tran, skew, perspective);

    localTransform = mat4(1.0);
    localTransform *= scale(growth);

    if (add)
    {
        localTransform *= scale(sc);
    }
    
    localTransform *= translate(tran);
    localTransform *= toMat4(conjugate(rot));
}

void GameObject::setLocalPosition(vec3 translation, bool add)
{
    unique_lock < mutex > lk(mtx);
    
    while (!ready)
    {
        cv.wait(lk);
    }

    vec3 sc;
    quat rot;
    vec3 tran;
    vec3 skew;
    vec4 perspective;

    decompose(localTransform, sc, rot, tran, skew, perspective);

    localTransform = mat4(1.0);
    localTransform *= scale(sc);
    localTransform *= translate(translation);

    if (add)
    {
        localTransform *= translate(tran);
    }
    
    localTransform *= toMat4(conjugate(rot));
}
        
void GameObject::setLocalTransform(mat4 localTransform)
{
    this->localTransform = localTransform;
}

void GameObject::clearLocalTransform()
{
    localTransform = mat4(1.0);
}
        
void GameObject::clearPhysicsObjectTransform()
{
    physicsObject->clearTransform();
    
    this->interpolation = false;
    prevTransform = nextTransform = mat4(1.0);
    interpolationCoeff = 1.0;
}
        
void GameObject::setPhysicsObjectTransform(mat4 model, bool interpolation)
{
    if (!physicsObject)
    {
        throw(runtime_error("ERROR::GameObject::setPhysicsObjectTrasnform() no physics object"));
    }

    this->interpolation = interpolation;

    if (!interpolation)
    {
        unique_ptr < btScalar > transform(global.glmMat42BtScalar(model));
        physicsObject->setTransform(transform.get());

        prevTransform = nextTransform = model;
        interpolationCoeff = 1.0;
    }
    else
    {
        prevTransform = nextTransform;
        nextTransform = model;
        interpolationCoeff = 0.0;
    }
}

void GameObject::addAnimation(Animation* anim)
{
    if (animations.find(anim->getName()) == animations.end())
    {
        animations.insert({anim->getName(), anim});
    }
}
        
void GameObject::removeAnimation(string name)
{
    if (animations.find(name) != animations.end())
    {
        Animation* anim = animations.find(name)->second;

        animations.erase(name);

        delete anim;
    }
}

void GameObject::playAnimation(string name, bool reset)
{
    if (animations.find(name) != animations.end())
    {
        skeleton->playAnimation(animations.find(name)->second, reset);
    }
}

void GameObject::stopAnimation()
{
    skeleton->stopAnimation();
}

PhysicsObject* GameObject::getPhysicsObject() const
{
    return physicsObject;
}

string GameObject::getGraphicsObject() const
{
    return graphicsObject;
}
        
Skeleton* GameObject::getSkeleton() const
{
    return skeleton;
}

string GameObject::getName() const
{
    return name;
}

bool GameObject::isCull() const
{
    return cull;
}

bool GameObject::isVisible() const
{
    return visible;
}

bool GameObject::isShadow() const
{
    return shadow;
}

bool GameObject::isCollidable() const
{
    if (physicsObject)
    {
        return physicsObject->isCollidable();
    }

    return true;
}

bool GameObject::isStatic() const
{
    if (physicsObject)
    {
        return physicsObject->isStatic();
    }

    return false;
}

mat4 GameObject::getPhysicsObjectTransform() const
{
    if (!physicsObject)
    {
        return mat4(1.0);
    }

    /* 16 elements */
    unique_ptr < btScalar > transform(physicsObject->getTransform());

    mat4 ret = global.btScalar2glmMat4(transform.get());

    return ret;
}

mat4 GameObject::getLocalTransform() const
{
    return localTransform;
}

void* GameObject::getUserPointer() const
{
    return userPointer;
}
        
Animation* GameObject::getActiveAnimation() const
{
    return skeleton->getAnimation();
}

Animation* GameObject::getAnimation(string name) const
{
    if (animations.find(name) != animations.end())
    {
        return animations.find(name)->second;
    }

    return nullptr;
}

void GameObject::render(Shader* shader, bool viewCull)
{
    if (interpolation && interpolationCoeff < 1.0)
    {
        mat4 model = float(1.0 - interpolationCoeff) * prevTransform + interpolationCoeff * nextTransform;

        unique_ptr < btScalar > scalarModel(global.glmMat42BtScalar(model));

        physicsObject->setTransform(scalarModel.get());

        interpolationCoeff += 0.2;
    }
    
    if (visible && viewCull && viewFrustum && boundSphere)
    {
        unique_lock < mutex > lk(mtx);
        ready = false;

        mat4 transform = getPhysicsObjectTransform() * localTransform;

        lk.unlock();
        ready = true;
        cv.notify_all();

        boundSphere->applyTransform(transform);

        if (!viewFrustum->isSphereInFrustum(boundSphere->getTransformedCenter(), boundSphere->getTransformedRadius()))
        {
            return;
        }
    }

    if (skeleton)
    {
        skeleton->update(shader);
    }

    if (visible)
    {
        if (!cull)
        {
            glDisable(GL_CULL_FACE);
        }

        unique_lock < mutex > lk(mtx);
        ready = false;

        shader->setMat4("localTransform", localTransform);
        shader->setMat4("model", getPhysicsObjectTransform());
        
        ready = true;
        lk.unlock();
        cv.notify_all();

        /* minimal diffuse value */
        shader->setFloat("minNormalCosAngle", minNormalCosAngle);

        for (size_t i = 0; i < meshes.size(); i++)
        {
            meshes[i]->render(shader);
        }
    
        glEnable(GL_CULL_FACE);
    }
}

/********* DEBUG **********/

void GameObject::createDebugSphere(int depth)
{
    sphere = new Sphere();

    if (boundSphere)
    {
        sphere->setColor(global.getRandomVec3());
        sphere->construct(boundSphere->getCenter(), boundSphere->getRadius(), depth);
    }
}

void GameObject::renderDebugSphere(Shader *shader)
{
    if (visible && sphere && viewFrustum)
    {
        mat4 transform = getPhysicsObjectTransform() * localTransform;
       
        shader->setMat4("transform", transform);
        shader->setMat4("projection", viewFrustum->getProjection());
        shader->setMat4("view", viewFrustum->getView());
        
        sphere->render(shader);
    }
}

/********* DEBUG **********/

GameObject::~GameObject()
{
    globalNames.erase(name);

    removeGraphicsObject();
    removePhysicsObject();

    delete modelLoader;

    delete sphere;

    for (auto& i: animations)
    {
        delete i.second;
    }
}
