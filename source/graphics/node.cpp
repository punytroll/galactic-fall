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

#include <assert.h>

#include <algorithm>
#include <deque>

#include <algebra/matrix4f.h>

#include "gl.h"
#include "node.h"
#include "render_context.h"
#include "scene.h"

Graphics::Node::Node(void) :
	_BlendFunctionDestinationFactor(GL_ZERO),
	_BlendFunctionSourceFactor(GL_ONE),
	_ClearColorBuffer(false),
	_ClearDepthBuffer(false),
	_Container(nullptr),
	_Normalize(false),
	_Orientation(true),
	_Position(0.0f, 0.0f, 0.0f),
	_Scale(1.0f),
	_Scene(nullptr),
	_UseBlending(false),
	_UseClipPlane0(false),
	_UseClipPlane1(false),
	_UseClipPlane2(false),
	_UseClipPlane3(false),
	_UseDepthTest(false),
	_UseLighting(false),
	_Use2DTexture(false)
{
}

Graphics::Node::~Node(void)
{
	assert(_Container == nullptr);
	assert(_Content.empty() == true);
}

void Graphics::Node::AddNode(Graphics::Node * Content)
{
	assert(Content->_Container == nullptr);
	if(_Scene != nullptr)
	{
		if(Content->_Content.empty() == true)
		{
			assert(Content->_Scene == nullptr);
			Content->_Scene = _Scene;
		}
		else
		{
			std::deque< Graphics::Node * > Todo;
			
			Todo.push_back(Content);
			while(Todo.empty() == false)
			{
				auto Node(Todo.back());
				
				Todo.pop_back();
				Todo.insert(Todo.end(), Node->_Content.begin(), Node->_Content.end());
				assert(Node->_Scene == nullptr);
				Node->_Scene = _Scene;
			}
		}
	}
	else
	{
		assert(Content->_Scene == nullptr);
	}
	Content->_Container = this;
	_Content.push_back(Content);
}

void Graphics::Node::Begin(Graphics::RenderContext * RenderContext)
{
	if(_ClearDepthBuffer == true)
	{
		GLClear(GL_DEPTH_BUFFER_BIT);
	}
	if(_ClearColorBuffer == true)
	{
		GLClear(GL_COLOR_BUFFER_BIT);
	}
	GLPushMatrix();
	GLMultMatrixf(_ModelMatrix.GetPointer());
	if(_Scale != 1.0f)
	{
		GLScalef(_Scale, _Scale, _Scale);
	}
	GLPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
	if(_Normalize == true)
	{
		GLEnable(GL_NORMALIZE);
	}
	else
	{
		GLDisable(GL_NORMALIZE);
	}
	if(_UseBlending == true)
	{
		GLEnable(GL_BLEND);
	}
	else
	{
		GLDisable(GL_BLEND);
	}
	if(_UseClipPlane0 == true)
	{
		GLEnable(GL_CLIP_PLANE0);
	}
	else
	{
		GLDisable(GL_CLIP_PLANE0);
	}
	if(_UseClipPlane1 == true)
	{
		GLEnable(GL_CLIP_PLANE1);
	}
	else
	{
		GLDisable(GL_CLIP_PLANE1);
	}
	if(_UseClipPlane2 == true)
	{
		GLEnable(GL_CLIP_PLANE2);
	}
	else
	{
		GLDisable(GL_CLIP_PLANE2);
	}
	if(_UseClipPlane3 == true)
	{
		GLEnable(GL_CLIP_PLANE3);
	}
	else
	{
		GLDisable(GL_CLIP_PLANE3);
	}
	if(_UseDepthTest == true)
	{
		GLEnable(GL_DEPTH_TEST);
	}
	else
	{
		GLDisable(GL_DEPTH_TEST);
	}
	if(_UseLighting == true)
	{
		GLEnable(GL_LIGHTING);
	}
	else
	{
		GLDisable(GL_LIGHTING);
	}
	if(_Use2DTexture == true)
	{
		GLEnable(GL_TEXTURE_2D);
	}
	else
	{
		GLDisable(GL_TEXTURE_2D);
	}
	GLBlendFunc(_BlendFunctionSourceFactor, _BlendFunctionDestinationFactor);
}

void Graphics::Node::CalculateModelMatrix(void)
{
	if(_Container != nullptr)
	{
		_ModelMatrix = _Container->_ModelMatrix.Translated(_Position).Rotate(_Orientation);
	}
	else
	{
		_ModelMatrix = Matrix4f(true).Translate(_Position).Rotate(_Orientation);
	}
}

void Graphics::Node::Destroy(void)
{
	assert(_Scene != nullptr);
	
	auto Scene(_Scene);
	
	Graphics::Node::_Destroy(this);
	Scene->OnDestroy(this);
}

void Graphics::Node::Draw(Graphics::RenderContext * RenderContext)
{
}

void Graphics::Node::End(Graphics::RenderContext * RenderContext)
{
	GLPopAttrib();
	GLPopMatrix();
}

void Graphics::Node::_Destroy(Graphics::Node * Node)
{
	while(Node->_Content.empty() == false)
	{
		Node->_Content.back()->Destroy();
	}
	if(Node->_Container != nullptr)
	{
		Node->_Container->_Content.erase(std::find(Node->_Container->_Content.begin(), Node->_Container->_Content.end(), Node));
		Node->_Container = nullptr;
	}
	Node->_Scene = nullptr;
}
