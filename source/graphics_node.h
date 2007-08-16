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

#include <vector>

#include "math/quaternion.h"
#include "math/vector3f.h"

namespace Graphics
{
	class Node
	{
	public:
		/**
		 * @brief The default constructor.
		 **/
		Node(void);
		
		/**
		 * @brief The destoructor for a graphics node object.
		 * 
		 * The destructor will also delete all content nodes.
		 **/
		virtual ~Node(void);
		
		/**
		 * @brief Recursive operation to render all nodes.
		 * 
		 * Calls the virtual function Draw() to draw @em this node.
		 **/
		void Render(void);
		
		/**
		 * @brief Draws @em this node.
		 * 
		 * The default implementation is empty.
		 **/
		virtual void Draw(void);
		
		/**
		 * @brief Adds a content node to @em this node.
		 * 
		 * Using this function will move the memory management responsibility for the @a Content node to @em this node.
		 **/
		void AddNode(Graphics::Node * Content);
		
		/**
		 * @brief Removes a node from the container.
		 * 
		 * Using this function the memory management responsibility for the @a Content node is transported to the caller.
		 **/
		void RemoveNode(Graphics::Node * Content);
		
		/**
		 * @brief Deletes all content nodes.
		 **/
		void Clear(void);
		
		Graphics::Node * GetContainer(void);
		const Graphics::Node * GetContainer(void) const;
		const Quaternion & GetOrientation(void) const;
		const Vector3f & GetPosition(void) const;
		void SetOrientation(const Quaternion & Orientation);
		void SetPosition(const Vector3f & Position);
	private:
		/**
		 * @brief Sets the container node for @em this node.
		 * 
		 * This function is only used internally, which is, why it is private.
		 **/
		void SetContainer(Node * Container);
		
		Node * m_Container;
		std::vector< Graphics::Node * > m_Content;
		Quaternion m_Orientation;
		Vector3f m_Position;
	};
}

inline Graphics::Node * Graphics::Node::GetContainer(void)
{
	return m_Container;
}

inline const Graphics::Node * Graphics::Node::GetContainer(void) const
{
	return m_Container;
}

inline const Quaternion & Graphics::Node::GetOrientation(void) const
{
	return m_Orientation;
}

inline const Vector3f & Graphics::Node::GetPosition(void) const
{
	return m_Position;
}

inline void Graphics::Node::SetOrientation(const Quaternion & Orientation)
{
	m_Orientation = Orientation;
}

inline void Graphics::Node::SetPosition(const Vector3f & Position)
{
	m_Position = Position;
}

inline void Graphics::Node::SetContainer(Graphics::Node * Container)
{
	m_Container = Container;
}

#endif
