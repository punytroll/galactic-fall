#ifndef GLOBALS_H
#define GLOBALS_H

/// DISCLAIMER: The intention of future development is to always create the more general concept of things so this file may someday be empty!

#define GLUT_WHEEL_UP 3
#define GLUT_WHEEL_DOWN 4

class Character;
class CommodityManager;
class Ship;
class SystemManager;

extern Character * g_PlayerCharacter;
extern Ship * g_PlayerShip;
extern CommodityManager g_CommodityManager;
extern SystemManager g_SystemManager;

#endif
