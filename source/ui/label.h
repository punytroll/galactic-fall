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

#ifndef UI_LABEL_H
#define UI_LABEL_H

#include <string>

#include "widget.h"

class Color;

namespace UI
{
	class Label : public UI::Widget
	{
	public:
		enum HorizontalAlignment
		{
			ALIGN_LEFT,
			ALIGN_RIGHT,
			ALIGN_HORIZONTAL_CENTER
		};
		
		enum VerticalAlignment
		{
			ALIGN_TOP,
			ALIGN_BOTTOM,
			ALIGN_VERTICAL_CENTER
		};
		
		// constructor & destructor
		Label(UI::Widget * SupWidget = 0, const std::string & Text = "");
		virtual ~Label(void);
		// getters
		UI::Label::HorizontalAlignment GetHorizontalAlignment(void) const;
		const std::string & GetText(void) const;
		const Color * GetTextColor(void) const;
		UI::Label::VerticalAlignment GetVerticalAlignment(void) const;
		bool GetWrap(void) const;
		bool GetWordWrap(void) const;
		// setters
		void SetText(const std::string & Text);
		void SetTextColor(const Color & ForegroundColor);
		void SetWrap(bool Wrap);
		void SetWordWrap(bool WordWrap);
		void SetHorizontalAlignment(UI::Label::HorizontalAlignment HorizontalAlignment);
		void SetVerticalAlignment(UI::Label::VerticalAlignment VerticalAlignment);
		// modifiers
		virtual void Draw(void) const;
	private:
		UI::Label::HorizontalAlignment m_HorizontalAlignment;
		std::string m_Text;
		Color * m_TextColor;
		UI::Label::VerticalAlignment m_VerticalAlignment;
		bool m_Wrap;
		bool m_WordWrap;
	};

	inline UI::Label::HorizontalAlignment Label::GetHorizontalAlignment(void) const
	{
		return m_HorizontalAlignment;
	}

	inline const std::string & Label::GetText(void) const
	{
		return m_Text;
	}

	inline const Color * Label::GetTextColor(void) const
	{
		return m_TextColor;
	}

	inline bool Label::GetWrap(void) const
	{
		return m_Wrap;
	}

	inline bool Label::GetWordWrap(void) const
	{
		return m_WordWrap;
	}

	inline UI::Label::VerticalAlignment Label::GetVerticalAlignment(void) const
	{
		return m_VerticalAlignment;
	}

	inline void Label::SetHorizontalAlignment(UI::Label::HorizontalAlignment HorizontalAlignment)
	{
		m_HorizontalAlignment = HorizontalAlignment;
	}

	inline void Label::SetText(const std::string & Text)
	{
		m_Text = Text;
	}

	inline void Label::SetWrap(bool Wrap)
	{
		m_Wrap = Wrap;
	}

	inline void Label::SetWordWrap(bool WordWrap)
	{
		m_WordWrap = WordWrap;
	}

	inline void Label::SetVerticalAlignment(UI::Label::VerticalAlignment VerticalAlignment)
	{
		m_VerticalAlignment = VerticalAlignment;
	}
}

#endif
