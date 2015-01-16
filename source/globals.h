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

#include <vector>

#ifndef NDEBUG
#define ON_DEBUG(A) (A)
#else
#define ON_DEBUG(A)
#endif

/// DISCLAIMER: The intention of future development is to always create the more general concept of things so this file may someday be empty!

template < class ClassType >
class ClassManager;

class BatteryClass;
class CommodityClass;
class Galaxy;
class GeneratorClass;
class MessageDispatcher;
class ObjectFactory;
class OutputObserver;
class ShipClass;
class SlotClass;
class SystemStatistics;
class TimeoutNotificationManager;

namespace Graphics
{
	class Engine;
	class Node;
	class View;
}

namespace UI
{
	class UserInterface;
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

extern ClassManager< BatteryClass > * g_BatteryClassManager;
extern OutputObserver * g_CharacterObserver;
extern ClassManager< CommodityClass > * g_CommodityClassManager;
extern Galaxy * g_Galaxy;
extern TimeoutNotificationManager * g_GameTimeTimeoutNotifications;
extern ClassManager< GeneratorClass > * g_GeneratorClassManager;
extern Graphics::Engine * g_GraphicsEngine;
extern MessageDispatcher * g_MessageDispatcher;
extern ObjectFactory * g_ObjectFactory;
extern std::vector< Graphics::View * > g_PrerenderedViews;
extern TimeoutNotificationManager * g_RealTimeTimeoutNotifications;
extern ClassManager< ShipClass > * g_ShipClassManager;
extern ClassManager< SlotClass > * g_SlotClassManager;
extern SystemStatistics * g_SystemStatistics;
extern float g_TimeWarp;
extern UI::UserInterface * g_UserInterface;

#endif
