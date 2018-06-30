/**
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

#include "event.h"
#include "../graphics/color_rgbo.h"
#include "list_box_item.h"

UI::ListBoxItem::ListBoxItem(void) :
	_Selected(false)
{
	ConnectMouseEnterCallback(std::bind(&UI::ListBoxItem::_OnMouseEnter, this, std::placeholders::_1));
	ConnectMouseLeaveCallback(std::bind(&UI::ListBoxItem::_OnMouseLeave, this, std::placeholders::_1));
}

void UI::ListBoxItem::SetSelected(bool Selected)
{
	_Selected = Selected;
	if(_Selected == false)
	{
		UnsetBackgroundColor();
	}
	else
	{
		SetBackgroundColor(Graphics::ColorRGBO(0.4f, 0.1f, 0.1f, 1.0f));
	}
}

void UI::ListBoxItem::_OnMouseEnter(UI::Event & MouseEnterEvent)
{
	if((MouseEnterEvent.GetPhase() == UI::Event::Phase::Target) && (_Selected == false))
	{
		SetBackgroundColor(Graphics::ColorRGBO(0.3f, 0.2f, 0.2f, 1.0f));
	}
}

void UI::ListBoxItem::_OnMouseLeave(UI::Event & MouseLeaveEvent)
{
	if((MouseLeaveEvent.GetPhase() == UI::Event::Phase::Target) && (_Selected == false))
	{
		UnsetBackgroundColor();
	}
}
