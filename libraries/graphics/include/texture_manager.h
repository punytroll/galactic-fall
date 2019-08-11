/**
 * galactic-fall
 * Copyright (C) 2007-2019  Hagen Möbius
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

#ifndef GRAPHICS_TEXTURE_MANAGER_H
#define GRAPHICS_TEXTURE_MANAGER_H

#include <map>
#include <string>

namespace Graphics
{
	class Texture;

	class TextureManager
	{
	public:
		~TextureManager(void);
		const std::map< std::string, Graphics::Texture * > & Get(void) const;
		const Graphics::Texture * Get(const std::string & Identifier) const;
		Graphics::Texture * Create(const std::string & Identifier);
		void Destroy(const std::string & Identifier);
	private:
		std::map< std::string, Graphics::Texture * > m_Managed;
	};
	
	inline const std::map< std::string, Graphics::Texture * > & Graphics::TextureManager::Get(void) const
	{
		return m_Managed;
	}
}

#endif
