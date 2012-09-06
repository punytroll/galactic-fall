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

#include "node.h"

class Color;

namespace Graphics
{
	class Material;
	class Model;
	
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
		
		virtual void Begin(void);
		
		/**
		 * @brief Draws @em this node.
		 * 
		 * The default implementation is empty.
		 **/
		virtual void Draw(void);
		
		virtual void End(void);
		
		/**
		 * @brief Set whether drawing this node will clear the depth buffer.
		 **/
		void SetClearDepthBuffer(bool ClearDepthBuffer);
		
		/**
		 * @brief Set the model to draw at this object.
		 **/
		void SetModel(const Graphics::Model * Model);
		
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
		
		/**
		 * @brief Set whether blending should be enabled for @em this node.
		 **/
		void SetUseBlending(bool UseBlending);
		
		/**
		 * @brief Set whether lighting is enabled for @em this object.
		 **/
		void SetUseLighting(bool UseLighting);
		
		/**
		 * @brief Adds a material for a specific mesh of the model.
		 * 
		 * This functions passes ownership of the @a Material to the ModelObject instance.
		 **/
		bool AddMaterial(const std::string & MeshIdentifier, Graphics::Material * Material);
	private:
		bool m_ClearDepthBuffer;
		const Graphics::Model * m_Model;
		bool m_Normalize;
		float m_Scale;
		bool m_UseBlending;
		bool m_UseLighting;
		std::map< std::string, Graphics::Material * > m_Materials;
	};
	
	inline void ModelObject::SetClearDepthBuffer(bool ClearDepthBuffer)
	{
		m_ClearDepthBuffer = ClearDepthBuffer;
	}
	
	inline void ModelObject::SetModel(const Graphics::Model * Model)
	{
		m_Model = Model;
	}
	
	inline void ModelObject::SetNormalize(bool Normalize)
	{
		m_Normalize = Normalize;
	}
	
	inline void ModelObject::SetScale(float Scale)
	{
		m_Scale = Scale;
	}
	
	inline void ModelObject::SetUseBlending(bool UseBlending)
	{
		m_UseBlending = UseBlending;
	}
	
	inline void ModelObject::SetUseLighting(bool UseLighting)
	{
		m_UseLighting = UseLighting;
	}
}

#endif
