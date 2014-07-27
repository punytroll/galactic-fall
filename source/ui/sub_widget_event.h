/**
 * galactic-fall
 * Copyright (C) 2014  Hagen Möbius
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

#ifndef UI_SUB_WIDGET_EVENT_H
#define UI_SUB_WIDGET_EVENT_H

#include <events/connection.h>

#include "event.h"

namespace UI
{
	class Widget;
	
	class SubWidgetEvent : public UI::Event
	{
	public:
		SubWidgetEvent(void);
		// getters
		UI::Widget * GetSubWidget(void);
		// setters
		void SetSubWidget(UI::Widget * SubWidget);
	private:
		UI::Widget * _SubWidget;
	};
}

inline UI::Widget * UI::SubWidgetEvent::GetSubWidget(void)
{
	return _SubWidget;
}

inline void UI::SubWidgetEvent::SetSubWidget(UI::Widget * SubWidget)
{
	_SubWidget = SubWidget;
}

#endif
