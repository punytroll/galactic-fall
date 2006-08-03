#include <iostream>
#include <vector>

#include <GL/gl.h>

#include "draw_text.h"
#include "label.h"

Label::Label(Widget * SupWidget) :
	Widget(SupWidget),
	m_ForegroundColor(1.0f, 1.0f, 1.0f),
	m_Wrap(false),
	m_WordWrap(false),
	m_HorizontalAlignment(Label::ALIGN_LEFT),
	m_VerticalAlignment(Label::ALIGN_TOP)
{
}

Label::Label(Widget * SupWidget, const std::string & String) :
	Widget(SupWidget),
	m_String(String),
	m_ForegroundColor(1.0f, 1.0f, 1.0f),
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
	glColor3fv(m_ForegroundColor.GetColor().m_V.m_A);
	if(m_Wrap == false)
	{
		glPushMatrix();
		glTranslatef(((m_HorizontalAlignment == Label::ALIGN_LEFT) ? (0.0f) : ((m_HorizontalAlignment == Label::ALIGN_RIGHT) ? (Width - 6.0f * m_String.length()) : (0.5f * (Width - 6.0f * m_String.length())))), ((m_VerticalAlignment == Label::ALIGN_TOP) ? (0.0f) : ((m_VerticalAlignment == Label::ALIGN_BOTTOM) ? (GetSize().m_V.m_A[1] - 12.0f) : (0.5f * (GetSize().m_V.m_A[1] - 12.0f)))), 0.0f);
		DrawText(m_String);
		glPopMatrix();
	}
	else
	{
		std::string::size_type CharacterWidth(static_cast< std::string::size_type >(Width / 6.0f));
		std::string::size_type Length(0);
		std::string::size_type Start(0);
		std::vector< std::pair< std::string::size_type, std::string::size_type > > Lines;
		
		while(Start < m_String.length())
		{
			Length = CharacterWidth;
			while((m_String[Start + Length] != ' ') && (m_WordWrap == true) && (Start + Length < m_String.length()))
			{
				--Length;
			}
			if(Start + Length >= m_String.length())
			{
				Lines.push_back(std::make_pair(Start, m_String.length() - Start));
			}
			else
			{
				Lines.push_back(std::make_pair(Start, Length));
			}
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
