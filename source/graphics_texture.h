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
#include <vector>

#include <GL/gl.h>

#include "type_definitions.h"

namespace Graphics
{
	class Texture
	{
	public:
		/**
		 * @brief The default constructor.
		 **/
		Texture(const std::string & Identifier);
		~Texture(void);
		// getters
		const std::string & GetIdentifier(void) const;
		// setters
		void SetData(unsigned_numeric Width, unsigned_numeric Height, unsigned_numeric Format, const unsigned char * Data);
		// modifiers
		void Activate(void) const;
	private:
		std::string _Identifier;
		GLuint _Texture;
	};
}

inline const std::string & Graphics::Texture::GetIdentifier(void) const
{
	return _Identifier;
}

#endif
