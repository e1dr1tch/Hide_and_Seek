#pragma once

#include <string>
#include <vector>

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

#include <glm/glm.hpp>

using namespace std;
using namespace glm;

class Weapon : public GameObject
{
    protected:
        Window* window;

        vec3 offset;
        quat twist;

        float lastShotTime;

        int storageBullets;
        int magazineSize;
        int magazineBullets;

        vector < vec3 > recoil;

        /* animaiton duration? */
        float shotSpeed;
        float shotPower;

    public:
        Weapon(Window* window, string name);

        void setOffset(vec3 offset);
        void setTwist(vec3 axis, float angle);

        void setStorageBullets(int storageBullets);
        void setMagazineSize(int magazineSize);
        void setMagazineBullets(int magazineBullets);

        void setShotSpeed(float shotSpeed);
        void setShotPower(float shotPower);

        void reload();
        void drop(vec3 where);
        void pick(vec3 forward, vec3 up);
        void choose();
        
        virtual bool fire() = 0;

        vec3 getOffset() const;
        quat getTwist() const;
        
        int getStorageBullets() const;
        int getMagazineSize() const;
        int getMagazineBullets() const;

        float getShotSpeed() const;
        float getShotPower() const;

        void updateStatus();
        void updateRotation(mat3 rotation);
        void updatePosition(vec3 center, vec3 forward, vec3 up);

        virtual ~Weapon();
};
