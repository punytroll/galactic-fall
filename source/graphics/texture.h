/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
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

#ifndef GRAPHICS_TEXTURE_H
#define GRAPHICS_TEXTURE_H

#include <string>

#include <GL/gl.h>

namespace Graphics
{
	class Texture
	{
	public:
		// constructor & destructor
		Texture(void);
		Texture(const std::string & Identifier);
		~Texture(void);
		// getters
		std::uint32_t GetHeight(void);
		const std::string & GetIdentifier(void);
		GLuint GetTextureName(void);
		std::uint32_t GetWidth(void);
		// setters
		void SetData(std::uint32_t Width, std::uint32_t Height, std::uint32_t Format, const unsigned char * Data);
		// modifiers
		void Activate(void) const;
		void Create(std::uint32_t Width, std::uint32_t Height, std::uint32_t Format);
		void Deactivate(void) const;
	private:
		std::uint32_t _Height;
		std::string * _Identifier;
		GLuint _TextureName;
		std::uint32_t _Width;
	};
	
	inline std::uint32_t Texture::GetHeight(void)
	{
		return _Height;
	}
	
	inline GLuint Texture::GetTextureName(void)
	{
		return _TextureName;
	}
	
	inline std::uint32_t Texture::GetWidth(void)
	{
		return _Width;
	}
}

#endif
