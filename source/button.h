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

#ifndef BUTTON_H
#define BUTTON_H

#include <string>

#include "mouse_button_listener.h"
#include "mouse_motion_listener.h"
#include "widget.h"

template < typename ReturnType >
class Callback0;

class Button : public Widget, public MouseButtonListener, public MouseMotionListener
{
public:
	Button(Widget * SupWidget);
	~Button(void);
	void AddClickedHandler(Callback0< void > ClickedHandler);
	// listener handlers
	virtual bool OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y);
	virtual void OnMouseEnter(Widget * EventSource);
	virtual void OnMouseLeave(Widget * EventSource);
private:
	std::list< Callback0< void > > m_ClickedEvent;
};

#endif
