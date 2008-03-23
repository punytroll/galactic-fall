/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

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

template < class ClassType >
class ClassManager;

class AssetClass;
class CommodityClass;
class Galaxy;
class MessageDispatcher;
class ObjectFactory;
class ShipClass;
class SlotClass;
class SystemStatistics;
class UserInterface;
class TimeoutNotificationManager;
class WeaponClass;

namespace Graphics
{
	class Engine;
	class MeshManager;
	class ModelManager;
	class Node;
	class TextureManager;
}

enum WantReturnCode
{
	OK,
	NO_STORAGE,
	NO_JUMP_TARGET,
	NO_LAND_TARGET,
	NO_SCOOP_TARGET,
	NOT_ENOUGH_CREDITS,
	NOT_ENOUGH_FUEL,
	NOT_ENOUGH_SPACE,
	TOO_FAR_AWAY,
	TOO_FAST,
	TOO_HIGH_RELATIVE_VELOCITY,
	TOO_NEAR_TO_STELLAR_OBJECT
};

extern ClassManager< AssetClass > * g_AssetClassManager;
extern ClassManager< CommodityClass > * g_CommodityClassManager;
extern Graphics::Node * g_CommodityLayer;
extern Galaxy * g_Galaxy;
extern TimeoutNotificationManager * g_GameTimeTimeoutNotifications;
extern Graphics::Engine * g_GraphicsEngine;
extern Graphics::MeshManager * g_MeshManager;
extern MessageDispatcher * g_MessageDispatcher;
extern Graphics::ModelManager * g_ModelManager;
extern ObjectFactory * g_ObjectFactory;
extern TimeoutNotificationManager * g_RealTimeTimeoutNotifications;
extern ClassManager< ShipClass > * g_ShipClassManager;
extern Graphics::Node * g_ShotLayer;
extern ClassManager< SlotClass > * g_SlotClassManager;
extern SystemStatistics * g_SystemStatistics;
extern Graphics::TextureManager * g_TextureManager;
extern UserInterface * g_UserInterface;
extern ClassManager< WeaponClass > * g_WeaponClassManager;

#endif
