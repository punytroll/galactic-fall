/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
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
	class ScrollBar;
	
	class ScrollBox : public UI::Widget
	{
	public:
		ScrollBox(UI::Widget * SupWidget);
		virtual ~ScrollBox(void);
		// getters
		UI::Widget * GetContent(void);
		const UI::Widget * GetContent(void) const;
		UI::Widget * GetView(void);
		const UI::Widget * GetView(void) const;
		UI::ScrollBar * GetHorizontalScrollBar(void);
		UI::ScrollBar * GetVerticalScrollBar(void);
		// setters
		void SetHorizontalScrollBarVisible(bool Visible);
		void SetVerticalScrollBarVisible(bool Visible);
	protected:
		// callbacks
		void OnContentOrViewSizeChanged(void);
		void OnHorizontalScrollPositionChanged(void);
		void OnVerticalScrollPositionChanged(void);
	private:
		UI::Widget * m_Content;
		UI::Widget * m_View;
		UI::ScrollBar * m_HorizontalScrollBar;
		UI::ScrollBar * m_VerticalScrollBar;
	};

	inline UI::Widget * ScrollBox::GetContent(void)
	{
		return m_Content;
	}

	inline const UI::Widget * ScrollBox::GetContent(void) const
	{
		return m_Content;
	}

	inline UI::Widget * ScrollBox::GetView(void)
	{
		return m_View;
	}

	inline const UI::Widget * ScrollBox::GetView(void) const
	{
		return m_View;
	}

	inline UI::ScrollBar * ScrollBox::GetHorizontalScrollBar(void)
	{
		return m_HorizontalScrollBar;
	}

	inline UI::ScrollBar * ScrollBox::GetVerticalScrollBar(void)
	{
		return m_VerticalScrollBar;
	}
}

#endif
