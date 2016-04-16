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

namespace Graphics
{
	class ColorRGBO;
	class RenderContext;
}

namespace UI
{
	class Label : public UI::Widget
	{
	public:
		enum class HorizontalAlignment
		{
			Left,
			Right,
			Center
		};
		
		enum class VerticalAlignment
		{
			Top,
			Bottom,
			Center
		};
		
		// constructor & destructor
		Label(UI::Widget * SupWidget = nullptr, const std::string & Text = "");
		virtual ~Label(void);
		// getters
		UI::Label::HorizontalAlignment GetHorizontalAlignment(void) const;
		const std::string & GetText(void) const;
		const Graphics::ColorRGBO * GetTextColor(void) const;
		UI::Label::VerticalAlignment GetVerticalAlignment(void) const;
		bool GetWrap(void) const;
		bool GetWordWrap(void) const;
		// setters
		void SetText(const std::string & Text);
		void SetTextColor(const Graphics::ColorRGBO & ForegroundColor);
		void SetWrap(bool Wrap);
		void SetWordWrap(bool WordWrap);
		void SetHorizontalAlignment(UI::Label::HorizontalAlignment HorizontalAlignment);
		void SetVerticalAlignment(UI::Label::VerticalAlignment VerticalAlignment);
		// modifiers
		virtual void Draw(Graphics::RenderContext * RenderContext) override;
	private:
		UI::Label::HorizontalAlignment _HorizontalAlignment;
		std::string _Text;
		Graphics::ColorRGBO * _TextColor;
		UI::Label::VerticalAlignment _VerticalAlignment;
		bool _Wrap;
		bool _WordWrap;
	};

	inline UI::Label::HorizontalAlignment Label::GetHorizontalAlignment(void) const
	{
		return _HorizontalAlignment;
	}

	inline const std::string & Label::GetText(void) const
	{
		return _Text;
	}

	inline const Graphics::ColorRGBO * Label::GetTextColor(void) const
	{
		return _TextColor;
	}

	inline bool Label::GetWrap(void) const
	{
		return _Wrap;
	}

	inline bool Label::GetWordWrap(void) const
	{
		return _WordWrap;
	}

	inline UI::Label::VerticalAlignment Label::GetVerticalAlignment(void) const
	{
		return _VerticalAlignment;
	}

	inline void Label::SetHorizontalAlignment(UI::Label::HorizontalAlignment HorizontalAlignment)
	{
		_HorizontalAlignment = HorizontalAlignment;
	}

	inline void Label::SetText(const std::string & Text)
	{
		_Text = Text;
	}

	inline void Label::SetWrap(bool Wrap)
	{
		_Wrap = Wrap;
	}

	inline void Label::SetWordWrap(bool WordWrap)
	{
		_WordWrap = WordWrap;
	}

	inline void Label::SetVerticalAlignment(UI::Label::VerticalAlignment VerticalAlignment)
	{
		_VerticalAlignment = VerticalAlignment;
	}
}

#endif
