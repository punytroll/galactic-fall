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

#include "button.h"
#include "clicked_listener.h"
#include "color.h"
#include "globals.h"

Button::Button(Widget * SupWidget) :
	Widget(SupWidget)
{
	SetBackgroundColor(Color(0.3f, 0.3f, 0.3f));
	AddMouseButtonListener(this);
	AddMouseMotionListener(this);
}

Button::~Button(void)
{
}

void Button::AddClickedListener(ClickedListener * ClickedListener)
{
	m_ClickedListeners.push_back(ClickedListener);
}

bool Button::OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y)
{
	if((Button == 1 /* LEFT */) && (State == EV_UP))
	{
		for(std::list< ClickedListener * >::iterator ClickedListenerIterator = m_ClickedListeners.begin(); ClickedListenerIterator != m_ClickedListeners.end(); ++ClickedListenerIterator)
		{
			if((*ClickedListenerIterator)->OnClicked(this) == true)
			{
				return true;
			}
		}
	}
	
	return false;
}

void Button::OnMouseEnter(Widget * EventSource)
{
	SetBackgroundColor(Color(0.4f, 0.4f, 0.4f));
}

void Button::OnMouseLeave(Widget * EventSource)
{
	SetBackgroundColor(Color(0.3f, 0.3f, 0.3f));
}
