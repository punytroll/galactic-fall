/**
 * galactic-fall
 * Copyright (C) 2007-2025  Hagen Möbius
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

#include <cassert>

#include <graphics/mesh.h>
#include <graphics/mesh_manager.h>

Graphics::MeshManager::~MeshManager(void)
{
	while(_Meshes.empty() == false)
	{
		Destroy(_Meshes.begin()->first);
	}
}

const Graphics::Mesh * Graphics::MeshManager::Get(const std::string & Identifier) const
{
	auto MeshIterator(_Meshes.find(Identifier));
	
	if(MeshIterator == _Meshes.end())
	{
		return nullptr;
	}
	else
	{
		return MeshIterator->second;
	}
}

Graphics::Mesh * Graphics::MeshManager::Create(const std::string & Identifier)
{
	assert(_Meshes.find(Identifier) == _Meshes.end());
	
	auto Mesh(new Graphics::Mesh(Identifier));
	
	_Meshes[Mesh->GetIdentifier()] = Mesh;
	
	return Mesh;
}

void Graphics::MeshManager::Destroy(const std::string & Identifier)
{
	auto MeshIterator(_Meshes.find(Identifier));
	
	assert(MeshIterator != _Meshes.end());
	delete MeshIterator->second;
	_Meshes.erase(MeshIterator);
}
