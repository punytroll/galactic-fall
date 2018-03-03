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

#ifndef GRAPHICS_MODEL_H
#define GRAPHICS_MODEL_H

#include <map>
#include <string>

#include <algebra/quaternion.h>
#include <algebra/vector3f.h>

namespace Graphics
{
	class Mesh;
	
	class Model
	{
	public:
		struct Part
		{
			std::string Identifier;
			const Graphics::Mesh * Mesh;
			Vector3f Position;
			Quaternion Orientation;
		};
		
		// constructor
		Model(const std::string & Identifier);
		~Model(void);
		// getters
		const std::string & GetIdentifier(void) const;
		const std::map< std::string, const Graphics::Model::Part * > & GetParts(void) const;
		float GetRadialSize(void) const;
		const Vector3f * GetMarkerPosition(const std::string & PartIdentifier, const std::string MarkerIdentifier) const;
		// modifiers
		void AddPart(const std::string & PartIdentifier, const Graphics::Mesh * PartMesh, const Vector3f & PartPosition, const Quaternion & PartOrientation);
	private:
		std::string _Identifier;
		std::map< std::string, const Graphics::Model::Part * > _Parts;
		mutable float _RadialSize;
	};
	
	inline const std::string & Model::GetIdentifier(void) const
	{
		return _Identifier;
	}
	
	inline const std::map< std::string, const Graphics::Model::Part * > & Model::GetParts(void) const
	{
		return _Parts;
	}
}

#endif
