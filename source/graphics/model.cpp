/**
 * galactic-fall
 * Copyright (C) 2006-2018  Hagen Möbius
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

#include "mesh.h"
#include "model.h"

Graphics::Model::Model(const std::string & Identifier) :
	_Identifier(Identifier),
	_RadialSize(-1.0f)
{
}

Graphics::Model::~Model(void)
{
	while(_Parts.empty() == false)
	{
		delete _Parts.begin()->second;
		_Parts.erase(_Parts.begin());
	}
}

const Vector3f * Graphics::Model::GetMarkerPosition(const std::string & PartIdentifier, const std::string MarkerIdentifier) const
{
	auto PartIterator{_Parts.find(PartIdentifier)};
	
	assert(PartIterator != _Parts.end());
	
	return PartIterator->second->Mesh->GetMarkerPosition(MarkerIdentifier);
}

float Graphics::Model::GetRadialSize(void) const
{
	if(_RadialSize < 0.0f)
	{
		for(auto & PartIterator : _Parts)
		{
			float RadialSize(PartIterator.second->Position.Length() + PartIterator.second->Mesh->GetRadialSize());
			
			if(_RadialSize < RadialSize)
			{
				_RadialSize = RadialSize;
			}
		}
	}
	
	return _RadialSize;
}

void Graphics::Model::AddPart(const std::string & PartIdentifier, const Graphics::Mesh * PartMesh, const Vector3f & PartPosition, const Quaternion & PartOrientation)
{
	assert(_Parts.find(PartIdentifier) == _Parts.end());
	
	auto Part(new Graphics::Model::Part());
	
	Part->Identifier = PartIdentifier;
	Part->Mesh = PartMesh;
	Part->Position = PartPosition;
	Part->Orientation = PartOrientation;
	_Parts[PartIdentifier] = Part;
}
