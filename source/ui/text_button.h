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

#ifndef UI_TEXT_BUTTON_H
#define UI_TEXT_BUTTON_H

#include "button.h"

namespace UI
{
	class Label;
	
	class TextButton : public UI::Button
	{
	public:
		TextButton(UI::Widget * SupWidget, const std::string & Text);
		// setters
		void SetText(const std::string & Text);
	private:
		UI::Label * _Label;
	};
}

#endif
