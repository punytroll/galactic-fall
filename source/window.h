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

#ifndef WINDOW_H
#define WINDOW_H

#include "widget.h"

class Border;

namespace UI
{
	class Label;
}

class WWindow : public Widget
{
public:
	WWindow(Widget * SupWidget, const std::string & Title = "");
	// getters
	Border * GetBorder(void);
	UI::Label * GetTitleLabel(void);
private:
	// callbacks
	bool OnTitleLabelMouseButton(int Button, int State, float X, float Y);
	void OnTitleLabelMouseMoved(float X, float Y);
	bool OnResizeDragBoxMouseButton(int Button, int State, float X, float Y);
	void OnResizeDragBoxMouseMoved(float X, float Y);
	// member variables
	Border * m_Border;
	UI::Label * m_TitleLabel;
	Widget * m_ResizeDragBox;
	Vector2f m_GrabPosition;
};

inline Border * WWindow::GetBorder(void)
{
	return m_Border;
}

inline UI::Label * WWindow::GetTitleLabel(void)
{
	return m_TitleLabel;
}

#endif
