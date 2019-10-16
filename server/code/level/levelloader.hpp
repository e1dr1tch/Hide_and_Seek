#pragma once

#include <map>
#include <vector>
#include <string>

#include <tinyxml2/tinyxml2.h>

using namespace std;
using namespace tinyxml2;

class LevelLoader
{
    private:
        World* physicsWorld;

        string levelName;

        map < string, PhysicsObject* > physicsObjects;
        
        vector < Player* > players;

        /* helpers */
        void loadPhysicsObject(XMLElement* physicsObjectElem, PhysicsObject*& PO);
        void loadWeapon(XMLElement* weaponElem, Weapon*& WE);

        /* main */
        void loadPhysicsObjects();
        void loadWeapons();

        void loadSoldiers();

    public:
        LevelLoader(World* physicsWorld);

        void loadLevel(string name);

        void getPhysicsObjectsData(map < string, PhysicsObject* > &physicsObjects) const;
        void getPlayersData(vector < Player* > &players) const;

        ~LevelLoader();
};
