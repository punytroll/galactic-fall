#ifndef GLOBALS_H
#define GLOBALS_H

/// DISCLAIMER: The intention of future development is to always create the more general concept of things so this file may someday be empty!

#define EV_DOWN 1
#define EV_UP 2

#define EV_MOUSE_BUTTON_LEFT 1
#define EV_MOUSE_BUTTON_RIGHT 2
#define EV_MOUSE_BUTTON_MIDDLE 3
#define EV_MOUSE_WHEEL_DOWN 4
#define EV_MOUSE_WHEEL_UP 5

class Character;
class CommodityManager;
class ModelManager;
class Ship;
class SystemManager;
class UserInterface;

extern Character * g_PlayerCharacter;
extern Ship * g_PlayerShip;
extern CommodityManager g_CommodityManager;
extern ModelManager g_ModelManager;
extern SystemManager g_SystemManager;
extern UserInterface g_UserInterface;

#endif
