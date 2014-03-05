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

#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include "widget.h"

namespace UI
{
	class Border;
	class Label;

	class Window : public UI::Widget
	{
	public:
		Window(UI::Widget * SupWidget, const std::string & Title = "");
		// setters
		void SetTitle(const std::string & Title);
	private:
		// callbacks
		void _OnTitleLabelMouseButton(UI::MouseButtonEvent & MouseButtonEvent);
		void _OnTitleLabelMouseMoved(float X, float Y);
		void _OnResizeDragBoxMouseButton(UI::MouseButtonEvent & MouseButtonEvent, Widget * ResizeDragBox);
		void _OnResizeDragBoxMouseMoved(float X, float Y, Widget * ResizeDragBox);
		// member variables
		UI::Label * _TitleLabel;
		Vector2f _GrabPosition;
	};
}

#endif
