/**
 * galactic-fall
 * Copyright (C) 2007-2019  Hagen Möbius
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

#ifndef GRAPHICS_MESH_NODE_H
#define GRAPHICS_MESH_NODE_H

#include <graphics/node.h>

namespace Graphics
{
	class Mesh;
	class RenderContext;
	class Style;
	
	class MeshNode : public Graphics::Node
	{
	public:
		// constructor & destructor
		MeshNode(void);
		virtual ~MeshNode(void) override;
		// getters
		const Graphics::Mesh * GetMesh(void) const;
		// setters
		void SetMesh(const Graphics::Mesh * Mesh);
		/**
		 * @note This passes the memory responsibility of the style to the mesh node.
		 **/
		void SetStyle(Graphics::Style * Style);
		// modifiers
		virtual void Draw(Graphics::RenderContext * RenderContext) override;
	private:
		const Graphics::Mesh * _Mesh;
		Graphics::Style * _Style;
	};
	
	inline const Graphics::Mesh * MeshNode::GetMesh(void) const
	{
		return _Mesh;
	}
	
	inline void MeshNode::SetMesh(const Graphics::Mesh * Mesh)
	{
		_Mesh = Mesh;
	}
}

#endif
