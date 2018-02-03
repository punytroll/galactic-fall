/**
 * galactic-fall
 * Copyright (C) 2018  Hagen Möbius
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

#ifndef UI_LIST_BOX_OBJECT_ITEM_H
#define UI_LIST_BOX_OBJECT_ITEM_H

#include "list_box_item.h"

class Object;

namespace UI
{
	class ListBoxObjectItem : public UI::ListBoxItem
	{
	public:
		// constructor
		ListBoxObjectItem(Object * Object);
		// getters
		Object * GetObject(void);
	private:
		// event handlers
		void _OnDestroying(UI::Event & DestroyingEvent);
		void _OnObjectDestroying(void);
		// member variables
		Object * _Object;
		Connection _ObjectDestroyingConnection;
	};
	
	inline Object * ListBoxObjectItem::GetObject(void)
	{
		return _Object;
	}
}

#endif
