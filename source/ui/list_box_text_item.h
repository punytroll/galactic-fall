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

#ifndef UI_LIST_BOX_TEXT_ITEM_H
#define UI_LIST_BOX_TEXT_ITEM_H

#include <string>

#include "list_box_item.h"

namespace UI
{
	class Label;
	
	class ListBoxTextItem : public UI::ListBoxItem
	{
	public:
		ListBoxTextItem(void);
		// getters
		const std::string & GetText(void) const;
		// setters
		void SetText(const std::string & Text);
	private:
		// member variables
		UI::Label * _Label;
	};
}

#endif
