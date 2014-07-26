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

#ifndef UI_LIST_BOX_H
#define UI_LIST_BOX_H

#include "scroll_box.h"

namespace UI
{
	class ListBoxItem;
	
	class ListBox : public UI::ScrollBox
	{
	public:
		ListBox(UI::Widget * SupWidget = nullptr);
		UI::ListBoxItem * GetSelectedItem(void);
	private:
		void _OnItemMouseButton(UI::MouseButtonEvent & MouseButtonEvent, UI::ListBoxItem * Item);
		void _OnSubWidgetAdded(UI::SubWidgetEvent & SubWidgetEvent);
		void _OnSubWidgetRemoved(UI::SubWidgetEvent & SubWidgetEvent);
		UI::ListBoxItem * _SelectedItem;
	};
}

inline UI::ListBoxItem * UI::ListBox::GetSelectedItem(void)
{
	return _SelectedItem;
}

#endif
