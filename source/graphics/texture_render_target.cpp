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

#include "texture.h"
#include "texture_render_target.h"
#include "gl.h"

Graphics::TextureRenderTarget::TextureRenderTarget(void) :
	_FramebufferName(0),
	_RenderbufferName(0),
	_Texture(nullptr)
{
}

Graphics::TextureRenderTarget::~TextureRenderTarget(void)
{
	assert(_FramebufferName == 0);
	assert(_RenderbufferName == 0);
	assert(_Texture == nullptr);
}

void Graphics::TextureRenderTarget::Activate(void)
{
	assert(_FramebufferName != 0);
	GLBindFramebuffer(GL_FRAMEBUFFER, _FramebufferName);
	assert(_Texture != nullptr);
	GLViewport(0, 0, _Texture->GetWidth(), _Texture->GetHeight());
}

void Graphics::TextureRenderTarget::SetTexture(Graphics::Texture * Texture)
{
	if(Texture == nullptr)
	{
		assert(_Texture != nullptr);
		_Texture = nullptr;
		GLDeleteRenderbuffers(1, &_RenderbufferName);
		_RenderbufferName = 0;
		GLDeleteFramebuffers(1, &_FramebufferName);
		_FramebufferName = 0;
	}
	else
	{
		assert(_Texture == nullptr);
		_Texture = Texture;
		_Texture->Activate();
		assert(_FramebufferName == 0);
		GLGenFramebuffers(1, &_FramebufferName);
		GLBindFramebuffer(GL_FRAMEBUFFER, _FramebufferName);
		assert(_RenderbufferName == 0);
		GLGenRenderbuffers(1, &_RenderbufferName);
		GLBindRenderbuffer(GL_RENDERBUFFER, _RenderbufferName);
		GLRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _Texture->GetWidth(), _Texture->GetHeight());
		GLFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _RenderbufferName);
		GLFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _Texture->GetTextureName(), 0);
	}
}
