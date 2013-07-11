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

#include "../graphics/gl.h"
#include "border.h"

UI::Border::Border(Widget * SupWidget) :
	Widget(SupWidget)
{
}

UI::Border::~Border(void)
{
}

void UI::Border::Draw(void) const
{
	Widget::Draw();
	GLColor4fv(m_ForegroundColor.GetColor().GetPointer());
	GLBegin(GL_QUADS);
		// left
		GLVertex2f(0.0f, 0.0f);
		GLVertex2f(0.0f, GetSize()[1]);
		GLVertex2f(GetWidth(), GetSize()[1]);
		GLVertex2f(GetWidth(), 0.0f);
		// bottom
		GLVertex2f(0.0f, GetSize()[1]);
		GLVertex2f(GetSize()[0], GetSize()[1]);
		GLVertex2f(GetSize()[0], GetSize()[1] - GetWidth());
		GLVertex2f(0.0f, GetSize()[1] - GetWidth());
		// right
		GLVertex2f(GetSize()[0], GetSize()[1]);
		GLVertex2f(GetSize()[0], 0.0f);
		GLVertex2f(GetSize()[0] - GetWidth(), 0.0f);
		GLVertex2f(GetSize()[0] - GetWidth(), GetSize()[1]);
		// top
		GLVertex2f(GetSize()[0], 0.0f);
		GLVertex2f(0.0f, 0.0f);
		GLVertex2f(0.0f, GetWidth());
		GLVertex2f(GetSize()[0], GetWidth());
	GLEnd();
}
