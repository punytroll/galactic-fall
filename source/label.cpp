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

#include <vector>

#include <GL/gl.h>

#include "draw_text.h"
#include "label.h"

Label::Label(Widget * SupWidget) :
	Widget(SupWidget),
	m_ForegroundColor(1.0f, 1.0f, 1.0f, 1.0f),
	m_Wrap(false),
	m_WordWrap(false),
	m_HorizontalAlignment(Label::ALIGN_LEFT),
	m_VerticalAlignment(Label::ALIGN_TOP)
{
}

Label::Label(Widget * SupWidget, const std::string & String) :
	Widget(SupWidget),
	m_String(String),
	m_ForegroundColor(1.0f, 1.0f, 1.0f, 1.0f),
	m_Wrap(false),
	m_WordWrap(false),
	m_HorizontalAlignment(Label::ALIGN_LEFT),
	m_VerticalAlignment(Label::ALIGN_TOP)
{
}

Label::~Label(void)
{
}

void Label::SetString(const std::string & String)
{
	m_String = String;
}

void Label::SetForegroundColor(const Color & ForegroundColor)
{
	m_ForegroundColor = ForegroundColor;
}

void Label::SetWrap(bool Wrap)
{
	m_Wrap = Wrap;
}

void Label::SetWordWrap(bool WordWrap)
{
	m_WordWrap = WordWrap;
}

void Label::SetHorizontalAlignment(const Label::HorizontalAlignment & HorizontalAlignment)
{
	m_HorizontalAlignment = HorizontalAlignment;
}

void Label::SetVerticalAlignment(const Label::VerticalAlignment & VerticalAlignment)
{
	m_VerticalAlignment = VerticalAlignment;
}

void Label::Draw(void) const
{
	float Width(GetSize().m_V.m_A[0]);
	
	Widget::Draw();
	glColor4fv(m_ForegroundColor.GetColor().m_V.m_A);
	if(m_Wrap == false)
	{
		glPushMatrix();
		glTranslatef(((m_HorizontalAlignment == Label::ALIGN_LEFT) ? (0.0f) : ((m_HorizontalAlignment == Label::ALIGN_RIGHT) ? (Width - 6.0f * m_String.length()) : (0.5f * (Width - 6.0f * m_String.length())))), ((m_VerticalAlignment == Label::ALIGN_TOP) ? (0.0f) : ((m_VerticalAlignment == Label::ALIGN_BOTTOM) ? (GetSize().m_V.m_A[1] - 12.0f) : (0.5f * (GetSize().m_V.m_A[1] - 12.0f)))), 0.0f);
		DrawText(m_String);
		glPopMatrix();
	}
	else
	{
		std::string::size_type LabelWidthInCharacters(static_cast< std::string::size_type >(Width / 6.0f));
		std::string::size_type Length(0);
		std::string::size_type Start(0);
		std::vector< std::pair< std::string::size_type, std::string::size_type > > Lines;
		
		while(Start < m_String.length())
		{
			// set the length of the line string to the label width and count downwards
			Length = LabelWidthInCharacters;
			// only look for a separator if we want to wrap at word boundaries and this is not the last line, else just wrap
			if((m_WordWrap == true) && (Start + Length < m_String.length()))
			{
				while((Length > 0) && (m_String[Start + Length] != ' '))
				{
					--Length;
				}
			}
			// if no separator could be found then just wrap at the label width
			if(Length == 0)
			{
				Length = LabelWidthInCharacters;
			}
			Lines.push_back(std::make_pair(Start, std::min(m_String.length() - Start, Length)));
			Start += Length + 1;
		}
		for(std::vector< std::pair< std::string::size_type, std::string::size_type > >::size_type Line = 0; Line < Lines.size(); ++Line)
		{
			glPushMatrix();
			glTranslatef(((m_HorizontalAlignment == Label::ALIGN_LEFT) ? (0.0f) : ((m_HorizontalAlignment == Label::ALIGN_RIGHT) ? (Width - 6.0f * Lines[Line].second) : ((Width - 6.0f * Lines[Line].second) / 2.0f))),  ((m_VerticalAlignment == Label::ALIGN_TOP) ? (12.0f * Line) : ((m_VerticalAlignment == Label::ALIGN_BOTTOM) ? (GetSize().m_V.m_A[1] - 12.0f * Line) : (0.5f * (GetSize().m_V.m_A[1] - 12.0f * Lines.size()) + 12.0f * Line))), 0.0f);
			DrawText(m_String.substr(Lines[Line].first, Lines[Line].second));
			glPopMatrix();
		}
	}
}
