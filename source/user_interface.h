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

#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <string>

#include "callbacks/connection_handle.h"

class KeyEventInformation;

namespace UI
{
	class Widget;
}

class UserInterface
{
public:
	UserInterface(void);
	virtual ~UserInterface(void);
	void Draw(void) const;
	bool MouseButton(int Button, int State, float X, float Y);
	bool Key(const KeyEventInformation & KeyEventInformation);
	void MouseMoved(float X, float Y);
	// setters
	void SetCaptureWidget(UI::Widget * Widget);
	void ReleaseCaptureWidget(void);
	// getters
	UI::Widget * GetWidget(const std::string & Path);
	UI::Widget * GetRootWidget(void);
	UI::Widget * GetCaptureWidget(void);
private:
	// callbacks
	void OnCaptureWidgetDestroying(void);
	void OnHoverWidgetDestroying(void);
	void OnRootWidgetDestroying(void);
	// member variables
	UI::Widget * m_CaptureWidget;
	ConnectionHandle m_CaptureWidgetDestroyingCallbackConnectionHandle;
	UI::Widget * m_HoverWidget;
	UI::Widget * m_RootWidget;
};

inline UI::Widget * UserInterface::GetCaptureWidget(void)
{
	return m_CaptureWidget;
}

inline UI::Widget * UserInterface::GetRootWidget(void)
{
	return m_RootWidget;
}

#endif
