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
#include <string>
#include <vector>

class Vector4f;

namespace Graphics
{
	class Camera;
	class ColorRGBO;
	class Engine;
	class Light;
	class Node;
	class Program;
	class Style;
	class Texture;
	
	class RenderContext
	{
	public:
		// constructor & destructor
		RenderContext(void);
		~RenderContext(void);
		// getters
		Graphics::Style * GetStyle(void);
		// setters
		void SetCamera(Graphics::Camera * Camera);
		void SetClipping(float Left, float Top, float Bottom, float Right);
		void SetColorRGBO(const Graphics::ColorRGBO & Color);
		void SetEngine(Graphics::Engine * Engine);
		void SetLight(Graphics::Light * Light);
		void SetNode(Graphics::Node * Node);
		void SetProgramIdentifier(const std::string & ProgramIdentifier);
		void SetStyle(Graphics::Style * Style);
		void SetTexture(const Graphics::Texture * Texture);
		// modifiers
		void ActivateProgram(void);
		void DeactivateProgram(void);
		void UnsetClipping(void);
		void UnsetColorRGBO(void);
		void UnsetProgramIdentifier(void);
	private:
		Graphics::Camera * _Camera;
		Vector4f * _Clipping;
		Graphics::ColorRGBO * _ColorRGBO;
		Graphics::Engine * _Engine;
		Graphics::Light * _Light;
		Graphics::Node * _Node;
		Graphics::Program * _Program;
		std::string * _ProgramIdentifier;
		Graphics::Style * _Style;
		const Graphics::Texture * _Texture;
	};
	
	inline Graphics::Style * Graphics::RenderContext::GetStyle(void)
	{
		return _Style;
	}
}

#endif
