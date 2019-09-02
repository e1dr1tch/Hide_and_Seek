#pragma once

#include <stdexcept>
#include <vector>
#include <string>

#include <tinyxml2/tinyxml2.h>

using namespace std;
using namespace tinyxml2;

class PlayerDataCollector
{
    private:
        int playerID;

        mat4 model;
        
    public:
        PlayerDataCollector();

        void setPlayerID(int playerID);
        
        void collect(Player* player);

        string getData() const;
        
        ~PlayerDataCollector();
};
