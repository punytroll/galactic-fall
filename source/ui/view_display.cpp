/**
 * galactic-fall
 * Copyright (C) 2013  Hagen Möbius
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

#include <algorithm>

#include "../globals.h"
#include "../graphics/drawing.h"
#include "../graphics/texture.h"
#include "../graphics/texture_render_target.h"
#include "../graphics/view.h"
#include "view_display.h"

UI::ViewDisplay::ViewDisplay(Widget * SupWidget) :
	Widget(SupWidget),
	_View(nullptr)
{
}

UI::ViewDisplay::~ViewDisplay(void)
{
	assert(_View == nullptr);
}
	
Graphics::View * UI::ViewDisplay::GetView(void)
{
	return _View;
}

void UI::ViewDisplay::SetView(Graphics::View * View)
{
	if(View == nullptr)
	{
		assert(_View != nullptr);
		g_PrerenderedViews.erase(std::find(g_PrerenderedViews.begin(), g_PrerenderedViews.end(), _View));
		_View = nullptr;
	}
	else
	{
		assert(_View == nullptr);
		_View = View;
		g_PrerenderedViews.push_back(_View);
	}
}

void UI::ViewDisplay::Draw(Graphics::RenderContext * RenderContext)
{
	Widget::Draw(RenderContext);
	if(_View != nullptr)
	{
		assert(_View->GetRenderTarget() != nullptr);
		
		auto TextureRenderTarget(dynamic_cast< Graphics::TextureRenderTarget * >(_View->GetRenderTarget()));
		
		assert(TextureRenderTarget != nullptr);
		assert(TextureRenderTarget->GetTexture() != nullptr);
		
		auto GlobalPosition{GetGlobalPosition()};
		
		Graphics::Drawing::DrawTexture(RenderContext, GlobalPosition[0], GlobalPosition[1], GlobalPosition[1] + GetHeight(), GlobalPosition[0] + GetWidth(), TextureRenderTarget->GetTexture());
	}
}
 
