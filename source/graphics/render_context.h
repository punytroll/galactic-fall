/**
 * galactic-fall
 * Copyright (C) 2015  Hagen Möbius
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

#ifndef GRAPHICS_RENDER_CONTEXT_H
#define GRAPHICS_RENDER_CONTEXT_H

#include <set>
#include <vector>

namespace Graphics
{
	class Camera;
	class Engine;
	class Light;
	class Style;
	
	class RenderContext
	{
	public:
		// constructor & destructor
		RenderContext(void);
		~RenderContext(void);
		// setters
		void SetCamera(Graphics::Camera * Camera);
		void SetEngine(Graphics::Engine * Engine);
		void SetLight(Graphics::Light * Light);
		void SetStyle(Graphics::Style * Style);
	private:
		Graphics::Camera * _Camera;
		Graphics::Engine * _Engine;
		Graphics::Light * _Light;
		Graphics::Style * _Style;
	};
	
	inline void Graphics::RenderContext::SetCamera(Graphics::Camera * Camera)
	{
		_Camera = Camera;
	}
	
	inline void Graphics::RenderContext::SetEngine(Graphics::Engine * Engine)
	{
		_Engine = Engine;
	}
	
	inline void Graphics::RenderContext::SetLight(Graphics::Light * Light)
	{
		_Light = Light;
	}
	
	inline void Graphics::RenderContext::SetStyle(Graphics::Style * Style)
	{
		_Style = Style;
	}
}

#endif
