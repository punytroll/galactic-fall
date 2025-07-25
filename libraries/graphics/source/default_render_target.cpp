/**
 * galactic-fall
 * Copyright (C) 2013-2025  Hagen Möbius
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

#include <graphics/default_render_target.h>
#include <graphics/gl.h>

void Graphics::DefaultRenderTarget::Activate(void)
{
	GLBindFramebuffer(GL_FRAMEBUFFER, 0);
	GLViewport(0, 0, _Width, _Height);
}

void Graphics::DefaultRenderTarget::SetSize(std::uint32_t Width, std::uint32_t Height)
{
	_Width = Width;
	_Height = Height;
}
