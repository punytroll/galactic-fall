/**
 * galactic-fall
 * Copyright (C) 2009-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include "settings.h"

Settings::KeyBinding::KeyBinding(std::uint32_t Code, std::string Event, std::string Action) :
	Action{std::move(Action)},
	Code(Code),
	Event{std::move(Event)}
{
}

auto Settings::SetKeyBindings(std::optional<std::list<Settings::KeyBinding>> KeyBindings) -> void
{
    m_KeyBindings = std::move(KeyBindings);
}

auto Settings::SetWindowDimensions(std::optional<Vector2f> WindowDimensions) -> void
{
    m_WindowDimensions = std::move(WindowDimensions);
}
