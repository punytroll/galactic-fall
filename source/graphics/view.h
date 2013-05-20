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

#ifndef GRAPHICS_VIEW_H
#define GRAPHICS_VIEW_H

namespace Graphics
{
	class Camera;
	class Engine;
	class RenderTarget;
	class Scene;
	
	class View
	{
		friend Graphics::Engine;
	public:
		View(void);
		~View(void);
		// getters
		Graphics::Camera * GetCamera(void);
		Graphics::Engine * GetEngine(void);
		Graphics::RenderTarget * GetRenderTarget(void);
		Graphics::Scene * GetScene(void);
		// setters
		void SetRenderTarget(Graphics::RenderTarget * RenderTarget);
		void SetScene(Graphics::Scene * Scene);
		// modifiers
		void Render(void);
	private:
		void _SetEngine(Graphics::Engine * Engine);
		Graphics::Camera * _Camera;
		Graphics::Engine * _Engine;
		Graphics::RenderTarget * _RenderTarget;
		Graphics::Scene * _Scene;
	};
	
	inline Graphics::Camera * View::GetCamera(void)
	{
		return _Camera;
	}
	
	inline Graphics::Engine * View::GetEngine(void)
	{
		return _Engine;
	}
	
	inline Graphics::RenderTarget * View::GetRenderTarget(void)
	{
		return _RenderTarget;
	}
	
	inline Graphics::Scene * View::GetScene(void)
	{
		return _Scene;
	}
}

#endif
