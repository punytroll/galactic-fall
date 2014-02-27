/**
 * galactic-fall
 * Copyright (C) 2007, 2014  Hagen Möbius
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

#include "../color.h"
#include "../graphics/gl.h"
#include "border.h"

UI::Border::Border(Widget * SupWidget) :
	Widget(SupWidget),
	_Color(0),
	_Width(1.0f)
{
}

UI::Border::~Border(void)
{
	delete _Color;
	_Color = 0;
}

void UI::Border::Draw(void)
{
	Widget::Draw();
	if(_Color != 0)
	{
		GLColor4fv(_Color->GetColor().GetPointer());
		GLBegin(GL_QUADS);
			// left
			GLVertex2f(0.0f, 0.0f);
			GLVertex2f(0.0f, GetSize()[1]);
			GLVertex2f(_Width, GetSize()[1]);
			GLVertex2f(_Width, 0.0f);
			// bottom
			GLVertex2f(0.0f, GetSize()[1]);
			GLVertex2f(GetSize()[0], GetSize()[1]);
			GLVertex2f(GetSize()[0], GetSize()[1] - _Width);
			GLVertex2f(0.0f, GetSize()[1] - _Width);
			// right
			GLVertex2f(GetSize()[0], GetSize()[1]);
			GLVertex2f(GetSize()[0], 0.0f);
			GLVertex2f(GetSize()[0] - _Width, 0.0f);
			GLVertex2f(GetSize()[0] - _Width, GetSize()[1]);
			// top
			GLVertex2f(GetSize()[0], 0.0f);
			GLVertex2f(0.0f, 0.0f);
			GLVertex2f(0.0f, _Width);
			GLVertex2f(GetSize()[0], _Width);
		GLEnd();
	}
}

void UI::Border::SetColor(const Color & BorderColor)
{
	delete _Color;
	_Color = new Color(BorderColor);
}
