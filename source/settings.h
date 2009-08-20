/**
 * galactic-fall
 * Copyright (C) 2009  Hagen Möbius
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

#ifndef SETTINGS_H
#define SETTINGS_H

#include "math/vector2f.h"

namespace Arxx
{
	class Item;
}

class Settings
{
public:
	struct KeyBinding
	{
		// constructor
		KeyBinding(int Code, const std::string & Event, const std::string & Action);
		// fields
		std::string Action;
		int Code;
		std::string Event;
	};
	
	// constructor and destructor
	Settings(void);
	~Settings(void);
	// getters
	const std::list< Settings::KeyBinding > * const GetKeyBindings(void) const;
	const Vector2f * const GetWindowDimensions(void) const;
	// setters
	void SetKeyBindings(const std::list< Settings::KeyBinding > & KeyBindings);
	void SetWindowDimensions(const Vector2f & WindowDimensions);
	// modifiers
	void LoadFromItem(Arxx::Item * Item);
private:
	std::list< Settings::KeyBinding > * _KeyBindings;
	Vector2f * _WindowDimensions;
};

inline const std::list< Settings::KeyBinding > * const Settings::GetKeyBindings(void) const
{
	return _KeyBindings;
}

inline const Vector2f * const Settings::GetWindowDimensions(void) const
{
	return _WindowDimensions;
}

#endif
