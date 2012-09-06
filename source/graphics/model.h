/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#ifndef GRAPHICS_MODEL_H
#define GRAPHICS_MODEL_H

#include <map>
#include <string>

namespace Graphics
{
	class Mesh;
	
	class Model
	{
	public:
		Model(const std::string & Identifier);
		const std::string & GetIdentifier(void) const;
		const std::map< std::string, const Graphics::Mesh * > & GetMeshes(void) const;
		float GetRadialSize(void) const;
		void AddMesh(const std::string & MeshIdentifier, const Graphics::Mesh * Mesh);
	private:
		std::string m_Identifier;
		std::map< std::string, const Graphics::Mesh * > m_Meshes;
		mutable float m_RadialSize;
	};
	
	inline const std::string & Model::GetIdentifier(void) const
	{
		return m_Identifier;
	}
	
	inline const std::map< std::string, const Graphics::Mesh * > & Model::GetMeshes(void) const
	{
		return m_Meshes;
	}
}

#endif
