/**
 * galactic-fall
 * Copyright (C) 2007-2025  Hagen Möbius
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

#include <algorithm>
#include <cassert>
#include <stack>

#include <algebra/matrix4f.h>

#include <graphics/gl.h>
#include <graphics/node.h>
#include <graphics/render_context.h>
#include <graphics/scene.h>

Graphics::Node::Node(void) :
	_BlendFunctionDestinationFactor(GL_ZERO),
	_BlendFunctionSourceFactor(GL_ONE),
	_ClearColorBuffer(false),
	_ClearDepthBuffer(false),
	_Container(nullptr),
	_Orientation(Quaternion::CreateIdentity()),
	_Position(Vector3f::CreateZero()),
	_Scale(1.0f),
	_Scene(nullptr),
	_UseBlending(false),
	_UseDepthTest(false)
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
		Content->_SetScene(_Scene);
	}
	else
	{
		assert(Content->_Scene == nullptr);
	}
	Content->_Container = this;
	_Content.push_back(Content);
}

void Graphics::Node::Begin([[maybe_unused]] Graphics::RenderContext * RenderContext)
{
	if(_ClearDepthBuffer == true)
	{
		GLClear(GL_DEPTH_BUFFER_BIT);
	}
	if(_ClearColorBuffer == true)
	{
		GLClear(GL_COLOR_BUFFER_BIT);
	}
	if(_UseBlending == true)
	{
		GLEnable(GL_BLEND);
	}
	else
	{
		GLDisable(GL_BLEND);
	}
	if(_UseDepthTest == true)
	{
		GLEnable(GL_DEPTH_TEST);
	}
	else
	{
		GLDisable(GL_DEPTH_TEST);
	}
	GLBlendFunc(_BlendFunctionSourceFactor, _BlendFunctionDestinationFactor);
}

void Graphics::Node::CalculateSpacialMatrix(void)
{
	if(_Container != nullptr)
	{
		_SpacialMatrix = _Container->_SpacialMatrix.Translated(_Position).Rotate(_Orientation);
	}
	else
	{
		_SpacialMatrix = Matrix4f::CreateTranslation(_Position).Rotate(_Orientation);
	}
}

void Graphics::Node::Destroy(void)
{
	assert(_Scene != nullptr);
	
	auto Scene(_Scene);
	
	Graphics::Node::_Destroy(this);
	Scene->OnDestroy(this);
}

void Graphics::Node::Draw([[maybe_unused]] Graphics::RenderContext * RenderContext)
{
}

void Graphics::Node::End([[maybe_unused]] Graphics::RenderContext * RenderContext)
{
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
	
void Graphics::Node::_SetScene(Graphics::Scene * Scene)
{
	assert(Scene != nullptr);
	
	std::stack< Graphics::Node * > ToDo;
	
	ToDo.push(this);
	while(ToDo.empty() == false)
	{
		auto Node(ToDo.top());
		
		ToDo.pop();
		assert(Node->_Scene == nullptr);
		for(auto ContentNode : Node->_Content)
		{
			ToDo.push(ContentNode);
		}
		Node->_Scene = Scene;
	}
}
