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

#ifndef GRAPHICS_SHADER_H
#define GRAPHICS_SHADER_H

#include <string>

namespace Graphics
{
	class Shader
	{
	public:
		enum class Type
		{
			FragmentShader,
			VertexShader
		};
		
		// constructor
		Shader(const std::string & Identifier);
		// getters
		const std::string & GetIdentifier(void) const;
		const std::string & GetSource(void) const;
		Graphics::Shader::Type GetType(void) const;
		// setters
		void SetSource(const std::string & Source);
		void SetType(Graphics::Shader::Type Type);
	private:
		std::string _Identifier;
		std::string _Source;
		Graphics::Shader::Type _Type;
	};
	
	inline const std::string & Shader::GetIdentifier(void) const
	{
		return _Identifier;
	}
	
	inline const std::string & Shader::GetSource(void) const
	{
		return _Source;
	}
	
	inline Graphics::Shader::Type Shader::GetType(void) const
	{
		return _Type;
	}
	
	inline void Shader::SetSource(const std::string & Source)
	{
		_Source = Source;
	}
	
	inline void Shader::SetType(Graphics::Shader::Type Type)
	{
		_Type = Type;
	}
}

#endif
