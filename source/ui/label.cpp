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

#include "../graphics/color_rgbo.h"
#include "../graphics/drawing.h"
#include "../graphics/render_context.h"
#include "label.h"

UI::Label::Label(void) :
	_HorizontalAlignment(UI::Label::HorizontalAlignment::Left),
	_TextColor(new Graphics::ColorRGBO(1.0f, 1.0f, 1.0f, 1.0f)),
	_VerticalAlignment(UI::Label::VerticalAlignment::Top),
	_Wrap(false),
	_WordWrap(false)
{
}

UI::Label::~Label(void)
{
	delete _TextColor;
}

void UI::Label::SetTextColor(const Graphics::ColorRGBO & TextColor)
{
	delete _TextColor;
	_TextColor = new Graphics::ColorRGBO{TextColor};
}

void UI::Label::Draw(Graphics::RenderContext * RenderContext)
{
	Widget::Draw(RenderContext);
	assert(_TextColor != nullptr);
	RenderContext->SetColorRGBO(*_TextColor);
	
	auto Width{GetWidth()};
	auto GlobalPosition{GetGlobalPosition()};
	
	if(_Wrap == false)
	{
		Graphics::Drawing::DrawText(RenderContext, GlobalPosition[0] + ((_HorizontalAlignment == UI::Label::HorizontalAlignment::Left) ? (0.0f) : ((_HorizontalAlignment == UI::Label::HorizontalAlignment::Right) ? (Width - 6.0f * _Text.length()) : (0.5f * (Width - 6.0f * _Text.length())))), GlobalPosition[1] + ((_VerticalAlignment == UI::Label::VerticalAlignment::Top) ? (0.0f) : ((_VerticalAlignment == UI::Label::VerticalAlignment::Bottom) ? (GetHeight() - 12.0f) : (0.5f * (GetHeight() - 12.0f)))), _Text);
	}
	else
	{
		std::string::size_type LabelWidthInCharacters(static_cast< std::string::size_type >(Width / 6.0f));
		std::string::size_type Length(0);
		std::string::size_type Start(0);
		std::vector< std::pair< std::string::size_type, std::string::size_type > > Lines;
		
		while(Start < _Text.length())
		{
			// set the length of the line string to the label width and count downwards
			Length = LabelWidthInCharacters;
			// only look for a separator if we want to wrap at word boundaries and this is not the last line, else just wrap
			if((_WordWrap == true) && (Start + Length < _Text.length()))
			{
				while((Length > 0) && (_Text[Start + Length] != ' '))
				{
					--Length;
				}
			}
			// if no separator could be found then just wrap at the label width
			if(Length == 0)
			{
				Length = LabelWidthInCharacters;
			}
			Lines.push_back(std::make_pair(Start, std::min(_Text.length() - Start, Length)));
			Start += Length + 1;
		}
		for(std::vector< std::pair< std::string::size_type, std::string::size_type > >::size_type Line = 0; Line < Lines.size(); ++Line)
		{
			Graphics::Drawing::DrawText(RenderContext, GlobalPosition[0] + ((_HorizontalAlignment == UI::Label::HorizontalAlignment::Left) ? (0.0f) : ((_HorizontalAlignment == UI::Label::HorizontalAlignment::Right) ? (Width - 6.0f * Lines[Line].second) : ((Width - 6.0f * Lines[Line].second) / 2.0f))), GlobalPosition[1] + ((_VerticalAlignment == UI::Label::VerticalAlignment::Top) ? (12.0f * Line) : ((_VerticalAlignment == UI::Label::VerticalAlignment::Bottom) ? (GetHeight() - 12.0f * Line) : (0.5f * (GetHeight() - 12.0f * Lines.size()) + 12.0f * Line))), _Text.substr(Lines[Line].first, Lines[Line].second));
		}
	}
	RenderContext->UnsetColorRGBO();
}
