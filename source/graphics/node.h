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

#include <algebra/matrix4f.h>
#include <algebra/quaternion.h>
#include <algebra/vector3f.h>

namespace Graphics
{
	class RenderContext;
	class Scene;
	
	class Node
	{
		friend class Graphics::Scene;
	public:
		// constructor & destructor
		Node(void);
		virtual ~Node(void);
		// getters
		Graphics::Node * GetContainer(void);
		const Graphics::Node * GetContainer(void) const;
		std::vector< Graphics::Node * > & GetContent(void);
		const Quaternion & GetOrientation(void) const;
		const Vector3f & GetPosition(void) const;
		float GetScale() const;
		Graphics::Scene * GetScene(void);
		bool GetUseLighting(void) const;
		// setters
		void SetBlendFunction(int SourceFactor, int DestinationFactor);
		void SetClearColorBuffer(bool ClearColorBuffer);
		void SetClearDepthBuffer(bool ClearDepthBuffer);
		void SetNormalize(bool Normalize);
		void SetOrientation(const Quaternion & Orientation);
		void SetPosition(const Vector3f & Position);
		void SetScale(float Scale);
		void SetUseBlending(bool UseBlending);
		void SetUseClipPlane0(bool UseClipPlane0);
		void SetUseClipPlane1(bool UseClipPlane1);
		void SetUseClipPlane2(bool UseClipPlane2);
		void SetUseClipPlane3(bool UseClipPlane3);
		void SetUseDepthTest(bool UseDepthTest);
		void SetUseLighting(bool UseLighting);
		void SetUse2DTexture(bool Use2DTexture);
		// modifiers
		void AddNode(Graphics::Node * Content);
		virtual void Begin(Graphics::RenderContext * RenderContext);
		void CalculateModelMatrix(void);
		virtual void Destroy(void);
		virtual void Draw(Graphics::RenderContext * RenderContext);
		virtual void End(Graphics::RenderContext * RenderContext);
	protected:
		static void _Destroy(Graphics::Node * Node);
	private:
		void _SetContainer(Graphics::Node * Container);
		void _SetScene(Graphics::Scene * Scene);
		
		int _BlendFunctionDestinationFactor;
		int _BlendFunctionSourceFactor;
		bool _ClearColorBuffer;
		bool _ClearDepthBuffer;
		Graphics::Node * _Container;
		std::vector< Graphics::Node * > _Content;
		Matrix4f _ModelMatrix;
		bool _Normalize;
		Quaternion _Orientation;
		Vector3f _Position;
		float _Scale;
		Graphics::Scene * _Scene;
		bool _UseBlending;
		bool _UseClipPlane0;
		bool _UseClipPlane1;
		bool _UseClipPlane2;
		bool _UseClipPlane3;
		bool _UseDepthTest;
		bool _UseLighting;
		bool _Use2DTexture;
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
	
	inline const Quaternion & Node::GetOrientation(void) const
	{
		return _Orientation;
	}
	
	inline const Vector3f & Node::GetPosition(void) const
	{
		return _Position;
	}
	
	inline float Node::GetScale(void) const
	{
		return _Scale;
	}
	
	inline Graphics::Scene * Node::GetScene(void)
	{
		return _Scene;
	}
	
	inline bool Node::GetUseLighting(void) const
	{
		return _UseLighting;
	}
	
	inline void Node::SetBlendFunction(int SourceFactor, int DestinationFactor)
	{
		_BlendFunctionSourceFactor = SourceFactor;
		_BlendFunctionDestinationFactor = DestinationFactor;
	}
	
	inline void Node::SetClearColorBuffer(bool ClearColorBuffer)
	{
		_ClearColorBuffer = ClearColorBuffer;
	}
	
	inline void Node::SetClearDepthBuffer(bool ClearDepthBuffer)
	{
		_ClearDepthBuffer = ClearDepthBuffer;
	}
	
	inline void Node::SetNormalize(bool Normalize)
	{
		_Normalize = Normalize;
	}
	
	inline void Node::SetOrientation(const Quaternion & Orientation)
	{
		_Orientation = Orientation;
	}
	
	inline void Node::SetPosition(const Vector3f & Position)
	{
		_Position = Position;
	}
	
	inline void Node::SetScale(float Scale)
	{
		_Scale = Scale;
	}
	
	inline void Node::_SetContainer(Graphics::Node * Container)
	{
		_Container = Container;
	}
	
	inline void Node::_SetScene(Graphics::Scene * Scene)
	{
		_Scene = Scene;
	}
	
	inline void Node::SetUseBlending(bool UseBlending)
	{
		_UseBlending = UseBlending;
	}
	
	inline void Node::SetUseClipPlane0(bool UseClipPlane0)
	{
		_UseClipPlane0 = UseClipPlane0;
	}
	
	inline void Node::SetUseClipPlane1(bool UseClipPlane1)
	{
		_UseClipPlane1 = UseClipPlane1;
	}
	
	inline void Node::SetUseClipPlane2(bool UseClipPlane2)
	{
		_UseClipPlane2 = UseClipPlane2;
	}
	
	inline void Node::SetUseClipPlane3(bool UseClipPlane3)
	{
		_UseClipPlane3 = UseClipPlane3;
	}
	
	inline void Node::SetUseDepthTest(bool UseDepthTest)
	{
		_UseDepthTest = UseDepthTest;
	}
	
	inline void Node::SetUseLighting(bool UseLighting)
	{
		_UseLighting = UseLighting;
	}
	
	inline void Node::SetUse2DTexture(bool Use2DTexture)
	{
		_Use2DTexture = Use2DTexture;
	}
}

#endif
