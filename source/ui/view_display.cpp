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
#include "../graphics/gl.h"
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
		
		Graphics::TextureRenderTarget * TextureRenderTarget(dynamic_cast< Graphics::TextureRenderTarget * >(_View->GetRenderTarget()));
		
		assert(TextureRenderTarget != nullptr);
		assert(TextureRenderTarget->GetTexture() != nullptr);
		TextureRenderTarget->GetTexture()->Activate();
		GLPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
		GLDisable(GL_LIGHTING);
		GLDisable(GL_DEPTH_TEST);
		GLEnable(GL_BLEND);
		GLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		GLColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		GLEnable(GL_TEXTURE_2D);
		GLBegin(GL_QUADS);
		GLTexCoord2f(0.0f, 1.0f);
		GLVertex2f(0.0f, 0.0f);
		GLTexCoord2f(0.0f, 0.0f);
		GLVertex2f(0.0f, GetSize()[1]);
		GLTexCoord2f(1.0f, 0.0f);
		GLVertex2f(GetSize()[0], GetSize()[1]);
		GLTexCoord2f(1.0f, 1.0f);
		GLVertex2f(GetSize()[0], 0.0f);
		GLEnd();
		GLPopAttrib();
	}
}
 
