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

#include <list>
#include <optional>
#include <string>

#include <algebra/vector2f.h>

namespace ARX
{
	class Item;
}

class Settings
{
public:
	struct KeyBinding
	{
		// constructor
		KeyBinding(int Code, std::string Event, std::string Action);
		// fields
		std::string Action;
		int Code;
		std::string Event;
	};
	
	// getters
	auto GetKeyBindings() const -> std::list<Settings::KeyBinding> const *;
	auto GetWindowDimensions() const -> Vector2f const *;
	// modifiers
	auto LoadFromItem(ARX::Item * Item) -> void;
private:
	std::optional<std::list<Settings::KeyBinding>> m_KeyBindings;
	std::optional<Vector2f> m_WindowDimensions;
};

inline auto Settings::GetKeyBindings() const -> std::list<Settings::KeyBinding> const *
{
	return ((m_KeyBindings.has_value() == true) ? (std::addressof(m_KeyBindings.value())) : (nullptr));
}

inline auto Settings::GetWindowDimensions() const -> Vector2f const *
{
	return ((m_WindowDimensions.has_value() == true) ? (std::addressof(m_WindowDimensions.value())) : (nullptr));
}

#endif
