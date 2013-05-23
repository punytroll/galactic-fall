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

#include "../draw_text.h"
#include "../graphics/gl.h"
#include "label.h"

UI::Label::Label(Widget * SupWidget, const std::string & Text) :
	Widget(SupWidget),
	m_HorizontalAlignment(Label::ALIGN_LEFT),
	m_Text(Text),
	m_TextColor(0),
	m_VerticalAlignment(Label::ALIGN_TOP),
	m_Wrap(false),
	m_WordWrap(false)
{
	m_TextColor = new Color(1.0f, 1.0f, 1.0f, 1.0f);
}

UI::Label::~Label(void)
{
	delete m_TextColor;
}

void UI::Label::SetTextColor(const Color & TextColor)
{
	delete m_TextColor;
	m_TextColor = new Color(TextColor);
}

void UI::Label::Draw(void) const
{
	float Width(GetSize().m_V.m_A[0]);
	
	Widget::Draw();
	GLColor4fv(m_TextColor->GetColor().m_V.m_A);
	if(m_Wrap == false)
	{
		GLPushMatrix();
		glTranslatef(((m_HorizontalAlignment == Label::ALIGN_LEFT) ? (0.0f) : ((m_HorizontalAlignment == Label::ALIGN_RIGHT) ? (Width - 6.0f * m_Text.length()) : (0.5f * (Width - 6.0f * m_Text.length())))), ((m_VerticalAlignment == Label::ALIGN_TOP) ? (0.0f) : ((m_VerticalAlignment == Label::ALIGN_BOTTOM) ? (GetSize().m_V.m_A[1] - 12.0f) : (0.5f * (GetSize().m_V.m_A[1] - 12.0f)))), 0.0f);
		DrawText(m_Text);
		GLPopMatrix();
	}
	else
	{
		std::string::size_type LabelWidthInCharacters(static_cast< std::string::size_type >(Width / 6.0f));
		std::string::size_type Length(0);
		std::string::size_type Start(0);
		std::vector< std::pair< std::string::size_type, std::string::size_type > > Lines;
		
		while(Start < m_Text.length())
		{
			// set the length of the line string to the label width and count downwards
			Length = LabelWidthInCharacters;
			// only look for a separator if we want to wrap at word boundaries and this is not the last line, else just wrap
			if((m_WordWrap == true) && (Start + Length < m_Text.length()))
			{
				while((Length > 0) && (m_Text[Start + Length] != ' '))
				{
					--Length;
				}
			}
			// if no separator could be found then just wrap at the label width
			if(Length == 0)
			{
				Length = LabelWidthInCharacters;
			}
			Lines.push_back(std::make_pair(Start, std::min(m_Text.length() - Start, Length)));
			Start += Length + 1;
		}
		for(std::vector< std::pair< std::string::size_type, std::string::size_type > >::size_type Line = 0; Line < Lines.size(); ++Line)
		{
			GLPushMatrix();
			glTranslatef(((m_HorizontalAlignment == Label::ALIGN_LEFT) ? (0.0f) : ((m_HorizontalAlignment == Label::ALIGN_RIGHT) ? (Width - 6.0f * Lines[Line].second) : ((Width - 6.0f * Lines[Line].second) / 2.0f))),  ((m_VerticalAlignment == Label::ALIGN_TOP) ? (12.0f * Line) : ((m_VerticalAlignment == Label::ALIGN_BOTTOM) ? (GetSize().m_V.m_A[1] - 12.0f * Line) : (0.5f * (GetSize().m_V.m_A[1] - 12.0f * Lines.size()) + 12.0f * Line))), 0.0f);
			DrawText(m_Text.substr(Lines[Line].first, Lines[Line].second));
			GLPopMatrix();
		}
	}
}
