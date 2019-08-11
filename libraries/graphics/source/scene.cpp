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

#include <cassert>
#include <stack>

#include <graphics/gl.h>
#include <graphics/light.h>
#include <graphics/node.h>
#include <graphics/particle_system.h>
#include <graphics/render_context.h>
#include <graphics/scene.h>

Graphics::Scene::Scene(void) :
	_Engine(nullptr),
	_Light(nullptr),
	_RootNode(nullptr)
{
}

Graphics::Scene::~Scene(void)
{
	if(_Light != nullptr)
	{
		delete _Light;
		_Light = nullptr;
	}
	if(_RootNode != nullptr)
	{
		_RootNode->Destroy();
		_RootNode = nullptr;
	}
}

void Graphics::Scene::_SetEngine(Graphics::Engine * Engine)
{
	if(Engine == nullptr)
	{
		assert(_Engine != nullptr);
		_Engine = nullptr;
	}
	else
	{
		assert(_Engine == nullptr);
		_Engine = Engine;
	}
}

void Graphics::Scene::SetRootNode(Graphics::Node * RootNode)
{
	assert(_RootNode == nullptr);
	assert(RootNode != nullptr);
	assert(RootNode->GetScene() == nullptr);
	_RootNode = RootNode;
	_RootNode->_SetScene(this);
}

void Graphics::Scene::ActivateLight(void)
{
	if(_Light == nullptr)
	{
		_Light = new Graphics::Light();
	}
}

void Graphics::Scene::DeactivateLight(void)
{
	if(_Light != nullptr)
	{
		delete _Light;
		_Light = nullptr;
	}
}

void Graphics::Scene::OnDestroy(Graphics::Node * Node)
{
	assert(Node->GetContainer() == nullptr);
	assert(Node->GetScene() == nullptr);
	assert(Node->GetContent().empty() == true);
	if(_DestroyCallback)
	{
		_DestroyCallback(Node);
	}
}

void Graphics::Scene::Update(void)
{
	std::stack< Graphics::Node * > ToDo;
	
	ToDo.push(_RootNode);
	while(ToDo.empty() == false)
	{
		auto & Item(ToDo.top());
		
		ToDo.pop();
		Item->CalculateSpacialMatrix();
		for(auto Content : Item->GetContent())
		{
			ToDo.push(Content);
		}
	}
}

void Graphics::Scene::Render(Graphics::RenderContext * RenderContext)
{
	RenderContext->SetLight(_Light);
	
	std::stack< Graphics::Node * > ToDo;
	
	ToDo.push(_RootNode);
	while(ToDo.empty() == false)
	{
		auto Item(ToDo.top());
		
		ToDo.pop();
		RenderContext->SetNode(Item);
		assert(Item != nullptr);
		Item->Begin(RenderContext);
		Item->Draw(RenderContext);
		Item->End(RenderContext);
		RenderContext->SetNode(nullptr);
		for(auto ContentIterator = Item->GetContent().rbegin(); ContentIterator != Item->GetContent().rend(); ++ContentIterator)
		{
			ToDo.push(*ContentIterator);
		}
	}
	RenderContext->SetLight(nullptr);
}
