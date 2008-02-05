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

#include "destroy_listener.h"

class KeyEventInformation;
class Widget;

class UserInterface : public DestroyListener
{
public:
	UserInterface(void);
	virtual ~UserInterface(void);
	void Draw(void) const;
	bool MouseButton(int Button, int State, float X, float Y);
	bool Key(const KeyEventInformation & KeyEventInformation);
	void MouseMotion(float X, float Y);
	// setters
	void SetCaptureWidget(Widget * Widget);
	void ReleaseCaptureWidget(void);
	// getters
	Widget * GetWidget(const std::string & Path);
	Widget * GetRootWidget(void);
	Widget * GetCaptureWidget(void);
	// virtual event handlers
	virtual void OnDestroy(Widget * EventSource);
private:
	Widget * m_CaptureWidget;
	Widget * m_HoverWidget;
	Widget * m_RootWidget;
};

inline Widget * UserInterface::GetCaptureWidget(void)
{
	return m_CaptureWidget;
}

inline Widget * UserInterface::GetRootWidget(void)
{
	return m_RootWidget;
}

#endif
