/**
 * galactic-fall
 * Copyright (C) 2009-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__SETTINGS_H
#define GALACTIC_FALL__SOURCE__SETTINGS_H

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
	auto GetKeyBindings() const -> std::optional<std::list<Settings::KeyBinding>> const &;
	auto GetWindowDimensions() const -> std::optional<Vector2f> const &;
	// modifiers
	auto LoadFromItem(ARX::Item * Item) -> void;
private:
	std::optional<std::list<Settings::KeyBinding>> m_KeyBindings;
	std::optional<Vector2f> m_WindowDimensions;
};

inline auto Settings::GetKeyBindings() const -> std::optional<std::list<Settings::KeyBinding>> const &
{
	return m_KeyBindings;
}

inline auto Settings::GetWindowDimensions() const -> std::optional<Vector2f> const &
{
	return m_WindowDimensions;
}

#endif
