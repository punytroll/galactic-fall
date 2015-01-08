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

#ifndef GRAPHICS_SHADING_MANAGER_H
#define GRAPHICS_SHADING_MANAGER_H

#include <map>
#include <string>
#include <vector>

namespace Graphics
{
	class Program;
	class Shader;

	class ShadingManager
	{
	public:
		// destructor
		~ShadingManager(void);
		// getters
		const Graphics::Program * GetProgram(const std::string & ProgramIdentifier) const;
		std::vector< std::string > GetProgramIdentifiers(void) const;
		const Graphics::Shader * GetShader(const std::string & ShaderIdentifier) const;
		// modifiers
		Graphics::Program * CreateProgram(const std::string & ProgramIdentifier);
		Graphics::Shader * CreateShader(const std::string & ShaderIdentifier);
		void DestroyProgram(const std::string & ProgramIdentifier);
		void DestroyShader(const std::string & ShaderIdentifier);
	private:
		std::map< std::string, Graphics::Program * > _Programs;
		std::map< std::string, Graphics::Shader * > _Shaders;
	};
}

#endif
