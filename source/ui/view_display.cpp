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

#include <GL/gl.h>

#include "../color.h"
#include "../globals.h"
#include "../graphics/texture.h"
#include "../graphics/texture_render_target.h"
#include "../graphics/view.h"
#include "view_display.h"

UI::ViewDisplay::ViewDisplay(Widget * SupWidget) :
	Widget(SupWidget),
	_View(0)
{
	SetBackgroundColor(Color(0.15f, 0.15f, 0.15f, 1.0f));
}

UI::ViewDisplay::~ViewDisplay(void)
{
	assert(_View == 0);
}
	
Graphics::View * UI::ViewDisplay::GetView(void)
{
	return _View;
}

void UI::ViewDisplay::SetView(Graphics::View * View)
{
	if(View == 0)
	{
		assert(_View != 0);
		g_PrerenderedViews.erase(std::find(g_PrerenderedViews.begin(), g_PrerenderedViews.end(), _View));
		_View = 0;
	}
	else
	{
		assert(_View == 0);
		_View = View;
		g_PrerenderedViews.push_back(_View);
	}
}

void UI::ViewDisplay::Draw(void) const
{
	Widget::Draw();
	if(_View != 0)
	{
		assert(_View->GetRenderTarget() != 0);
		
		Graphics::TextureRenderTarget * TextureRenderTarget(dynamic_cast< Graphics::TextureRenderTarget * >(_View->GetRenderTarget()));
		
		assert(TextureRenderTarget != 0);
		assert(TextureRenderTarget->GetTexture() != 0);
		TextureRenderTarget->GetTexture()->Activate();
		glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.0f, GetSize()[1]);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(GetSize()[0], GetSize()[1]);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(GetSize()[0], 0.0f);
		glEnd();
		glPopAttrib();
	}
}
 
