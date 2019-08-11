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

#ifndef GRAPHICS_NODE_H
#define GRAPHICS_NODE_H

#include <cassert>
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
		const Matrix4f & GetSpacialMatrix(void) const;
		const Quaternion & GetOrientation(void) const;
		const Vector3f & GetPosition(void) const;
		float GetScale() const;
		Graphics::Scene * GetScene(void);
		// setters
		void SetBlendFunction(int SourceFactor, int DestinationFactor);
		void SetClearColorBuffer(bool ClearColorBuffer);
		void SetClearDepthBuffer(bool ClearDepthBuffer);
		void SetOrientation(const Quaternion & Orientation);
		void SetPosition(const Vector3f & Position);
		void SetScale(float Scale);
		void SetUseBlending(bool UseBlending);
		void SetUseDepthTest(bool UseDepthTest);
		// modifiers
		void AddNode(Graphics::Node * Content);
		virtual void Begin(Graphics::RenderContext * RenderContext);
		void CalculateSpacialMatrix(void);
		virtual void Destroy(void);
		virtual void Draw(Graphics::RenderContext * RenderContext);
		virtual void End(Graphics::RenderContext * RenderContext);
	protected:
		static void _Destroy(Graphics::Node * Node);
	private:
		void _SetScene(Graphics::Scene * Scene);
		
		int _BlendFunctionDestinationFactor;
		int _BlendFunctionSourceFactor;
		bool _ClearColorBuffer;
		bool _ClearDepthBuffer;
		Graphics::Node * _Container;
		std::vector< Graphics::Node * > _Content;
		Quaternion _Orientation;
		Vector3f _Position;
		float _Scale;
		Graphics::Scene * _Scene;
		Matrix4f _SpacialMatrix;
		bool _UseBlending;
		bool _UseDepthTest;
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
	
	inline const Matrix4f & Node::GetSpacialMatrix(void) const
	{
		return _SpacialMatrix;
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
	
	inline void Node::SetUseBlending(bool UseBlending)
	{
		_UseBlending = UseBlending;
	}
	
	inline void Node::SetUseDepthTest(bool UseDepthTest)
	{
		_UseDepthTest = UseDepthTest;
	}
}

#endif
