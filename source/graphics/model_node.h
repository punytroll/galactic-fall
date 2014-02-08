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

#ifndef GRAPHICS_MODEL_NODE_H
#define GRAPHICS_MODEL_NODE_H

#include "node.h"

class Color;

namespace Graphics
{
	class Material;
	class Model;
	
	class ModelNode : public Graphics::Node
	{
	public:
		// constructor & destructor
		ModelNode(void);
		virtual ~ModelNode(void);
		// setters
		void SetModel(const Graphics::Model * Model);
		// modifiers
		bool AddMaterial(const std::string & MeshIdentifier, Graphics::Material * Material);
		virtual void Draw(void);
	private:
		const Graphics::Model * _Model;
		std::map< std::string, Graphics::Material * > _Materials;
	};
	
	inline void ModelNode::SetModel(const Graphics::Model * Model)
	{
		_Model = Model;
	}
}

#endif
