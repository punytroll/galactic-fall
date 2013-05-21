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

#ifndef GRAPHICS_TEXTURE_RENDER_TARGET_H
#define GRAPHICS_TEXTURE_RENDER_TARGET_H

#include "gl.h"
#include "render_target.h"

namespace Graphics
{
	class Texture;
	
	class TextureRenderTarget : public RenderTarget
	{
	public:
		// constructor & destructor
		TextureRenderTarget(void);
		~TextureRenderTarget(void);
		// getters
		Graphics::Texture * GetTexture(void);
		// setters
		void SetTexture(Graphics::Texture * Texture);
		// modifiers
		virtual void Activate(void);
	private:
		GLuint _FramebufferName;
		GLuint _RenderbufferName;
		Graphics::Texture * _Texture;
	};
	
	inline Graphics::Texture * TextureRenderTarget::GetTexture(void)
	{
		return _Texture;
	}
}

#endif
 
