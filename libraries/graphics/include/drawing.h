/**
 * galactic-fall
 * Copyright (C) 2015-2019  Hagen Möbius
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

#ifndef GRAPHICS_DRAWING_H
#define GRAPHICS_DRAWING_H

#include <cstdint>
#include <string>
#include <vector>

#include <algebra/vector2f.h>

namespace Graphics
{
	class ColorRGBO;
	class Engine;
	class RenderContext;
	class Texture;
	
	class Drawing
	{
	public:
		static void DrawBoxFromPositionAndSize(Graphics::RenderContext * RenderContext, const Vector2f & Position, const Vector2f & Size);
		static void DrawBox(Graphics::RenderContext * RenderContext, float Left, float Top, float Bottom, float Right);
		static void DrawCircles(Graphics::RenderContext * RenderContext, const std::vector< Vector2f > & CenterPositions, const std::vector< Graphics::ColorRGBO > & Colors, float Size, std::uint32_t NumberOfVerticesPerQuarter);
		static void DrawLines(Graphics::RenderContext * RenderContext, const std::vector< std::pair< Vector2f, Vector2f > > & Positions, const std::vector< Graphics::ColorRGBO > & Colors);
		static void DrawPoints(Graphics::RenderContext * RenderContext, const std::vector< Vector2f > & Positions, const std::vector< Graphics::ColorRGBO > & Colors);
		static void DrawText(Graphics::RenderContext * RenderContext, const Vector2f & Position, const std::string & Text);
		static void DrawText(Graphics::RenderContext * RenderContext, float Left, float Top, const std::string & Text);
		static void DrawTextWithoutClippingRightHanded(Graphics::RenderContext * RenderContext, float Left, float Top, float Height, const std::string & Text);
		
		/**
		 * @note Uses the texture set on the render context.
		 **/
		static void DrawTexture(Graphics::RenderContext * RenderContext, float Left, float Top, float Bottom, float Right);
		static void InitializeFont(Graphics::Engine * Engine);
		static void DeinitializeFont(void);
	private:
		static void _DrawText(Graphics::RenderContext * RenderContext, float Left, float Top, float Height, const std::string & Text, const std::string & ProgamName, bool LeftHanded);
		static const Graphics::Texture * _FontTexture;
	};
}

#endif
