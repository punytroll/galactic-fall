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

#include <GL/gl.h>

#include "border.h"

Border::Border(Widget * SupWidget) :
	Widget(SupWidget)
{
}

Border::~Border(void)
{
}

void Border::Draw(void) const
{
	Widget::Draw();
	glColor4fv(m_ForegroundColor.GetColor().m_V.m_A);
	glBegin(GL_QUADS);
		// left
		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.0f, GetSize()[1]);
		glVertex2f(GetWidth(), GetSize()[1]);
		glVertex2f(GetWidth(), 0.0f);
		// bottom
		glVertex2f(0.0f, GetSize()[1]);
		glVertex2f(GetSize()[0], GetSize()[1]);
		glVertex2f(GetSize()[0], GetSize()[1] - GetWidth());
		glVertex2f(0.0f, GetSize()[1] - GetWidth());
		// right
		glVertex2f(GetSize()[0], GetSize()[1]);
		glVertex2f(GetSize()[0], 0.0f);
		glVertex2f(GetSize()[0] - GetWidth(), 0.0f);
		glVertex2f(GetSize()[0] - GetWidth(), GetSize()[1]);
		// top
		glVertex2f(GetSize()[0], 0.0f);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.0f, GetWidth());
		glVertex2f(GetSize()[0], GetWidth());
	glEnd();
}
