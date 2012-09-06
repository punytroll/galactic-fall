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

#include <assert.h>

#include <GL/gl.h>

#include "mesh.h"
#include "model.h"

Graphics::Model::Model(const std::string & Identifier) :
	m_Identifier(Identifier),
	m_RadialSize(-1.0f)
{
}

float Graphics::Model::GetRadialSize(void) const
{
	if(m_RadialSize < 0.0f)
	{
		for(std::map< std::string, const Graphics::Mesh * >::const_iterator MeshIterator = m_Meshes.begin(); MeshIterator != m_Meshes.end(); ++MeshIterator)
		{
			float RadialSize(MeshIterator->second->GetRadialSize());
			
			if(m_RadialSize < RadialSize)
			{
				m_RadialSize = RadialSize;
			}
		}
	}
	
	return m_RadialSize;
}

void Graphics::Model::AddMesh(const std::string & MeshIdentifier, const Graphics::Mesh * Mesh)
{
	assert(m_Meshes.find(MeshIdentifier) == m_Meshes.end());
	
	m_Meshes[MeshIdentifier] = Mesh;
}
