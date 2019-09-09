#pragma once

//native
#include <algorithm>
#include <deque>
#include <cmath>
#include <stdexcept>

//bullet
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

using namespace std;

class Soldier : public Player
{
    private:
        deque < Weapon* > weapons;
        deque < Weapon* > new_weapons;

    public:
        Soldier(float speed = 1);
        Soldier(PhysicsObject* physicsObject, float speed = 1);

        void pick(Weapon* weapon);
        void drop(btVector3 direction);
        
        void next();
        void prev();

        void newToOldWeapons();

        deque < Weapon* > getWeapons() const;
        deque < Weapon* > getNewWeapons() const;
        
        ~Soldier();
};