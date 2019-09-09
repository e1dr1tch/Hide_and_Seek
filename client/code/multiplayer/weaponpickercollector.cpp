#include "../shader/shader.hpp"

#include "../global/convert.hpp"

#include "../framebuffer/framebuffer.hpp"
#include "../framebuffer/colorbuffer.hpp"

#include "../window/renderquad.hpp"
#include "../window/glfwevents.hpp"
#include "../window/window.hpp"

#include "../player/camera.hpp"

#include "../debug/debugsphere.hpp"
#include "../debug/debugdrawer.hpp"

#include "../game_object/openglmotionstate.hpp"
#include "../game_object/animation.hpp"
#include "../game_object/mesh.hpp"
#include "../game_object/bone.hpp"
#include "../game_object/skeleton.hpp"
#include "../game_object/viewfrustum.hpp"
#include "../game_object/boundsphere.hpp"
#include "../game_object/modelloader.hpp"
#include "../game_object/physicsobject.hpp"
#include "../game_object/gameobject.hpp"
#include "../game_object/weapon.hpp"
#include "../game_object/rifle.hpp"

#include "../world/raytracer.hpp"

#include "../player/player.hpp"
#include "../player/soldier.hpp"

#include "weaponpickercollector.hpp"

WeaponPickerCollector::WeaponPickerCollector() 
{
    playerID = 0;
    pickFrom = pickTo = vec3(0.0);
}

void WeaponPickerCollector::setPlayerID(int playerID)
{
    this->playerID = playerID;
}

void WeaponPickerCollector::collect(Player* player)
{
    Soldier* soldier = dynamic_cast < Soldier* >(player);

    if (!soldier)
    {
        throw(runtime_error("ERROR::WeaponPickerCollector::collect() player is not a soldier"));
    }

    pair < vec3, vec3 > ray = soldier->getPickRay();
    pickFrom = ray.first;
    pickTo = ray.second;
}

string WeaponPickerCollector::getData() const
{
    if (pickFrom == vec3(0.0) && pickTo == vec3(0.0))
    {
        return "";
    }

    XMLDocument weaponPickerDoc;

    /* root */
    XMLNode* root = weaponPickerDoc.NewElement("Pick");

    weaponPickerDoc.InsertFirstChild(root);

    /* playerID */
    XMLElement* playerIDElem = weaponPickerDoc.NewElement("id");
    playerIDElem->SetText(playerID);

    root->InsertEndChild(playerIDElem);
    
    /* pickFrom */
    XMLElement* fromElem = weaponPickerDoc.NewElement("frm");
    fromElem->SetAttribute("x", cutFloat(pickFrom.x, 4));
    fromElem->SetAttribute("y", cutFloat(pickFrom.y, 4));
    fromElem->SetAttribute("z", cutFloat(pickFrom.z, 4));

    root->InsertEndChild(fromElem);
    
    /* pickTo */
    XMLElement* toElem = weaponPickerDoc.NewElement("to");
    toElem->SetAttribute("x", cutFloat(pickTo.x, 4));
    toElem->SetAttribute("y", cutFloat(pickTo.y, 4));
    toElem->SetAttribute("z", cutFloat(pickTo.z, 4));

    root->InsertEndChild(toElem);

    /* printer */
    XMLPrinter weaponPickerPrinter;
    weaponPickerDoc.Print(&weaponPickerPrinter);

    string res("BEG\n");
    res += weaponPickerPrinter.CStr();
    res += "END\n";

    return res;    
}

void WeaponPickerCollector::clear()
{
    pickFrom = pickTo = vec3(0.0);
}

WeaponPickerCollector::~WeaponPickerCollector() {}