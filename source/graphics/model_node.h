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
	class Model;
	class Style;
	
	class ModelNode : public Graphics::Node
	{
	public:
		// constructor & destructor
		ModelNode(void);
		virtual ~ModelNode(void);
		// setters
		void SetModel(const Graphics::Model * Model);
		// modifiers
		/**
		 * @note This passes the memory responsibility of the material to the model node.
		 **/
		void AddStyle(const std::string & MeshIdentifier, Graphics::Style * Style);
		virtual void Draw(Graphics::RenderContext * RenderContext) override;
	private:
		const Graphics::Model * _Model;
		std::map< std::string, Graphics::Style * > _Styles;
	};
	
	inline void ModelNode::SetModel(const Graphics::Model * Model)
	{
		_Model = Model;
	}
}

#endif
