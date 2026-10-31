/**
 * galactic-fall
 * Copyright (C) 2009-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__SETTINGS_H
#define GALACTIC_FALL__SOURCE__SETTINGS_H

#include <cstdint>
#include <list>
#include <optional>
#include <string>

#include <algebra/vector2f.h>

class Settings
{
public:
	struct KeyBinding
	{
		// constructor
		KeyBinding(std::uint32_t Code, std::string Event, std::string Action);
		// fields
		std::string Action;
		std::uint32_t Code;
		std::string Event;
	};
	
	auto GetKeyBindings() const -> std::optional<std::list<Settings::KeyBinding>> const &;
	auto GetWindowDimensions() const -> std::optional<Vector2f> const &;
    auto SetKeyBindings(std::optional<std::list<Settings::KeyBinding>> KeyBindings) -> void;
    auto SetWindowDimensions(std::optional<Vector2f> WindowDimensions) -> void;
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
