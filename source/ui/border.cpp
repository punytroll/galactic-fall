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

#include "../graphics/color_rgbo.h"
#include "../graphics/drawing.h"
#include "../graphics/render_context.h"
#include "border.h"

UI::Border::Border(Widget * SupWidget) :
	Widget(SupWidget),
	_Color(nullptr),
	_LineWidth(1.0f)
{
}

UI::Border::~Border(void)
{
	delete _Color;
	_Color = nullptr;
}

void UI::Border::Draw(Graphics::RenderContext * RenderContext)
{
	Widget::Draw(RenderContext);
	if(_Color != nullptr)
	{
		assert(RenderContext != nullptr);
		RenderContext->SetColorRGBO(*_Color);
		RenderContext->SetProgramIdentifier("widget");
		RenderContext->ActivateProgram();
		
		auto GlobalPosition{GetGlobalPosition()};
		
		Graphics::Drawing::DrawBox(RenderContext, GlobalPosition[0], GlobalPosition[1], GlobalPosition[1] + GetSize()[1], GlobalPosition[0] + _LineWidth);
		Graphics::Drawing::DrawBox(RenderContext, GlobalPosition[0], GlobalPosition[1], GlobalPosition[1] + _LineWidth, GlobalPosition[0] + GetSize()[0]);
		Graphics::Drawing::DrawBox(RenderContext, GlobalPosition[0], GlobalPosition[1] + GetSize()[1] - _LineWidth, GlobalPosition[1] + GetSize()[1], GlobalPosition[0] + GetSize()[0]);
		Graphics::Drawing::DrawBox(RenderContext, GlobalPosition[0] + GetSize()[0] - _LineWidth, GlobalPosition[1], GlobalPosition[1] + GetSize()[1], GlobalPosition[0] + GetSize()[0]);
		RenderContext->DeactivateProgram();
		RenderContext->UnsetProgramIdentifier();
		RenderContext->UnsetColorRGBO();
	}
}

void UI::Border::SetColor(const Graphics::ColorRGBO & BorderColor)
{
	delete _Color;
	_Color = new Graphics::ColorRGBO(BorderColor);
}
