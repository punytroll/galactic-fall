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

#ifndef GRAPHICS_PROGRAM_H
#define GRAPHICS_PROGRAM_H

#include <list>
#include <string>

namespace Graphics
{
	class Program
	{
	public:
		// constructor
		Program(const std::string & Identifier);
		// getters
		const std::string & GetIdentifier(void) const;
		// modifiers
		void AddShaderIdentifier(const std::string & ShaderIdentifier);
	private:
		std::string _Identifier;
		std::list< std::string > _ShaderIdentifiers;
	};
	
	inline const std::string & Graphics::Program::GetIdentifier(void) const
	{
		return _Identifier;
	}
}

#endif
