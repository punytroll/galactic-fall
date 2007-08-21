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

#ifndef GRAPHICS_MODEL_OBJECT_H
#define GRAPHICS_MODEL_OBJECT_H

#include "graphics_node.h"

class Color;
class Model;

namespace Graphics
{
	class ModelObject : public Graphics::Node
	{
	public:
		/**
		 * @brief The default constructor.
		 **/
		ModelObject(void);
		
		/**
		 * @brief The destoructor for a graphics node object.
		 * 
		 * The destructor will also delete all content nodes.
		 **/
		virtual ~ModelObject(void);
		
		/**
		 * @brief Draws @em this node.
		 * 
		 * The default implementation is empty.
		 **/
		virtual void Draw(void);
		
		/**
		 * @brief Set whether blending should be enabled for @em this node.
		 **/
		void SetBlending(bool Blending);
		
		/**
		 * @brief Set whether drawing this node will clear the depth buffer.
		 **/
		void SetClearDepthBuffer(bool ClearDepthBuffer);
		
		/**
		 * @brief Set the color with which to draw @em this object.
		 **/
		void SetColor(const Color & Color);
		
		/**
		 * @brief Set whether lighting is enabled for @em this object.
		 **/
		void SetLighting(bool Lighting);
		
		/**
		 * @brief Set the model to draw at this object.
		 **/
		void SetModel(const Model * Model);
		
		/**
		 * @brief Enables or disables the normalization of the model normals.
		 * 
		 * Setting this to true is mainly useful for some Scale other than 1.0f.
		 **/
		void SetNormalize(bool Normalize);
		
		/**
		 * @brief Set the scale at which to draw at this object.
		 **/
		void SetScale(float Scale);
	private:
		bool m_Blending;
		bool m_ClearDepthBuffer;
		Color * m_Color;
		bool m_Lighting;
		const Model * m_Model;
		bool m_Normalize;
		float m_Scale;
	};
}

inline void Graphics::ModelObject::SetBlending(bool Blending)
{
	m_Blending = Blending;
}

inline void Graphics::ModelObject::SetClearDepthBuffer(bool ClearDepthBuffer)
{
	m_ClearDepthBuffer = ClearDepthBuffer;
}

inline void Graphics::ModelObject::SetLighting(bool Lighting)
{
	m_Lighting = Lighting;
}

inline void Graphics::ModelObject::SetModel(const Model * Model)
{
	m_Model = Model;
}

inline void Graphics::ModelObject::SetNormalize(bool Normalize)
{
	m_Normalize = Normalize;
}

inline void Graphics::ModelObject::SetScale(float Scale)
{
	m_Scale = Scale;
}

#endif
