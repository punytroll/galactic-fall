/**
 * galactic-fall
 * Copyright (C) 2006-2018  Hagen Möbius
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

#ifndef SHIP_CLASS_H
#define SHIP_CLASS_H

#include <map>
#include <string>

#include <algebra/vector3f.h>

#include "template.h"

class Slot;
class VisualizationPrototype;

class ShipClass : public Template
{
public:
	// constructor
	ShipClass(const std::string & Identifier);
	// destructor
	virtual ~ShipClass(void);
	// getters
	const std::string & GetIdentifier(void) const;
	const std::map< std::string, Slot * > & GetSlots(void) const;
	/**
	 * @note Passes memory management responsibility for @a Slot to the ship class.
	 **/
	bool AddSlot(const std::string & SlotIdentifier, Slot * Slot);
private:
	std::map< std::string, Slot * > _Slots;
};

inline const std::string & ShipClass::GetIdentifier(void) const
{
	return GetSubTypeIdentifier();
}

inline const std::map< std::string, Slot * > & ShipClass::GetSlots(void) const
{
	return _Slots;
}

#endif
