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

#include <GL/gl.h>

#include "../color.h"
#include "../globals.h"
#include "../graphics/engine.h"
#include "../graphics/texture.h"
#include "../graphics/texture_manager.h"
#include "image.h"

UI::Image::Image(Widget * SupWidget) :
	Widget(SupWidget),
	_TextureIdentifier(0)
{
	SetBackgroundColor(Color(0.15f, 0.15f, 0.15f, 1.0f));
}

UI::Image::~Image(void)
{
	UnsetTexture();
}

void UI::Image::SetTextureIdentifier(const std::string & TextureIdentifier)
{
	UnsetTexture();
	_TextureIdentifier = new std::string(TextureIdentifier);
}

void UI::Image::UnsetTexture(void)
{
	if(_TextureIdentifier != 0)
	{
		delete _TextureIdentifier;
		_TextureIdentifier = 0;
	}
}

void UI::Image::Draw(void) const
{
	Widget::Draw();
	if(_TextureIdentifier != 0)
	{
		glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		assert(g_GraphicsEngine != 0);
		assert(g_GraphicsEngine->GetTextureManager() != 0);
		
		const Graphics::Texture * Texture(g_GraphicsEngine->GetTextureManager()->Get(*_TextureIdentifier));
		
		assert(Texture != 0);
		Texture->Activate();
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
 
