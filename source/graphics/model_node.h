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
		/**
		 * @brief The default constructor.
		 **/
		ModelNode(void);
		
		/**
		 * @brief The destoructor for a graphics node object.
		 * 
		 * The destructor will also delete all materials.
		 **/
		virtual ~ModelNode(void);
		
		virtual void Begin(void);
		
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
		bool _ClearDepthBuffer;
		const Graphics::Model * _Model;
		bool _Normalize;
		float _Scale;
		bool _UseBlending;
		bool _UseLighting;
		std::map< std::string, Graphics::Material * > _Materials;
	};
	
	inline void ModelNode::SetClearDepthBuffer(bool ClearDepthBuffer)
	{
		_ClearDepthBuffer = ClearDepthBuffer;
	}
	
	inline void ModelNode::SetModel(const Graphics::Model * Model)
	{
		_Model = Model;
	}
	
	inline void ModelNode::SetNormalize(bool Normalize)
	{
		_Normalize = Normalize;
	}
	
	inline void ModelNode::SetScale(float Scale)
	{
		_Scale = Scale;
	}
	
	inline void ModelNode::SetUseBlending(bool UseBlending)
	{
		_UseBlending = UseBlending;
	}
	
	inline void ModelNode::SetUseLighting(bool UseLighting)
	{
		_UseLighting = UseLighting;
	}
}

#endif
