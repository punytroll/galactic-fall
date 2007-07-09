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

class CommodityManager;
class Galaxy;
class ModelManager;
class UserInterface;

extern CommodityManager g_CommodityManager;
extern Galaxy * g_Galaxy;
extern ModelManager g_ModelManager;
extern UserInterface g_UserInterface;

#endif
