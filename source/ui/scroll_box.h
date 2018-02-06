/**
 * galactic-fall
 * Copyright (C) 2007-2018  Hagen Möbius
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

#ifndef UI_SCROLL_BOX_H
#define UI_SCROLL_BOX_H

#include "widget.h"

namespace UI
{
	class Button;
	class Event;
	class ScrollBar;
	
	class ScrollBox : public UI::Widget
	{
	public:
		ScrollBox(void);
		virtual ~ScrollBox(void);
		// getters
		UI::Widget * GetContent(void);
		UI::Widget * GetView(void);
		UI::ScrollBar * GetHorizontalScrollBar(void);
		UI::ScrollBar * GetVerticalScrollBar(void);
		// setters
		void SetHorizontalScrollBarVisible(bool Visible);
		void SetVerticalScrollBarVisible(bool Visible);
		// modifiers
		void Clear(void);
	protected:
		// callbacks
		void _OnContentOrViewSizeChanged(UI::Event & SizeChangedEvent);
		void _OnHorizontalScrollPositionChanged(void);
		void _OnMouseButton(UI::MouseButtonEvent & MouseButtonEvent);
		void _OnVerticalScrollPositionChanged(void);
	private:
		UI::Widget * _Content;
		UI::Widget * _View;
		UI::ScrollBar * _HorizontalScrollBar;
		UI::ScrollBar * _VerticalScrollBar;
	};

	inline UI::Widget * ScrollBox::GetContent(void)
	{
		return _Content;
	}

	inline UI::Widget * ScrollBox::GetView(void)
	{
		return _View;
	}

	inline UI::ScrollBar * ScrollBox::GetHorizontalScrollBar(void)
	{
		return _HorizontalScrollBar;
	}

	inline UI::ScrollBar * ScrollBox::GetVerticalScrollBar(void)
	{
		return _VerticalScrollBar;
	}
}

#endif
