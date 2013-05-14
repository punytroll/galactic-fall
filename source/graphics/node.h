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

#ifndef GRAPHICS_NODE_H
#define GRAPHICS_NODE_H

#include <assert.h>

#include <vector>

#include "../math/quaternion.h"
#include "../math/vector3f.h"

namespace Graphics
{
	class Scene;
	
	class Node
	{
		friend class Graphics::Scene;
	public:
		/**
		 * @brief The default constructor.
		 **/
		Node(void);
		
		/**
		 * @brief The destoructor for a graphics node object.
		 **/
		virtual ~Node(void);
		
		/**
		 * @brief Destroys the node and all of its content.
		 **/
		void Destroy(void);
		
		/**
		 * @brief Recursive operation to render all nodes.
		 * 
		 * Calls the virtual function Draw() to draw @em this node.
		 **/
		void Render(void);
		
		virtual void Begin(void);
		
		/**
		 * @brief Draws @em this node.
		 * 
		 * The default implementation is empty.
		 **/
		virtual void Draw(void);
		
		virtual void End(void);
		
		/**
		 * @brief Adds a content node to @em this node.
		 **/
		void AddNode(Graphics::Node * Content);
		
		Graphics::Node * GetContainer(void);
		const Graphics::Node * GetContainer(void) const;
		std::vector< Graphics::Node * > & GetContent(void);
		const Quaternion & GetOrientation(void) const;
		const Vector3f & GetPosition(void) const;
		Graphics::Scene * GetScene(void);
		// setters
		void SetOrientation(const Quaternion & Orientation);
		void SetPosition(const Vector3f & Position);
	private:
		/**
		 * @brief Sets the container node for @em this node.
		 * 
		 * This function is only used internally, which is, why it is private.
		 **/
		void _SetContainer(Graphics::Node * Container);
		void _SetScene(Graphics::Scene * Scene);
		
		Graphics::Node * _Container;
		std::vector< Graphics::Node * > _Content;
		Quaternion _Orientation;
		Vector3f _Position;
		Graphics::Scene * _Scene;
	};
	
	inline Graphics::Node * Node::GetContainer(void)
	{
		return _Container;
	}
	
	inline const Graphics::Node * Node::GetContainer(void) const
	{
		return _Container;
	}
	
	inline std::vector< Graphics::Node * > & Node::GetContent(void)
	{
		return _Content;
	}
	
	inline Graphics::Scene* Node::GetScene(void)
	{
		return _Scene;
	}
	
	inline const Quaternion & Node::GetOrientation(void) const
	{
		return _Orientation;
	}
	
	inline const Vector3f & Node::GetPosition(void) const
	{
		return _Position;
	}
	
	inline void Node::SetOrientation(const Quaternion & Orientation)
	{
		_Orientation = Orientation;
	}
	
	inline void Node::SetPosition(const Vector3f & Position)
	{
		_Position = Position;
	}
	
	inline void Node::_SetContainer(Graphics::Node * Container)
	{
		_Container = Container;
	}
	
	inline void Node::_SetScene(Graphics::Scene * Scene)
	{
		_Scene = Scene;
	}
}

#endif
