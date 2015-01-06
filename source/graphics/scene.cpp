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

#include <stack>

#include "gl.h"
#include "light.h"
#include "node.h"
#include "particle_system.h"
#include "scene.h"

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
		Item->CalculateModelMatrix();
		for(auto Content : Item->GetContent())
		{
			ToDo.push(Content);
		}
	}
}

void Graphics::Scene::Render(void)
{
	assert(_RootNode != nullptr);
	if(_Light != nullptr)
	{
		GLEnable(GL_LIGHT0);
		GLLightfv(GL_LIGHT0, GL_POSITION, _Light->GetPosition().GetPointer());
		GLLightfv(GL_LIGHT0, GL_DIFFUSE, _Light->GetDiffuseColor().GetPointer());
	}
	else
	{
		GLDisable(GL_LIGHT0);
	}
	// disable lighting by default, nodes have to activate it if they want it
	GLDisable(GL_LIGHTING);
	
	std::stack< Graphics::Node * > ToDo;
	
	ToDo.push(_RootNode);
	while(ToDo.empty() == false)
	{
		auto Item(ToDo.top());
		
		ToDo.pop();
		Item->Begin();
		Item->Draw();
		Item->End();
		for(auto ContentIterator = Item->GetContent().rbegin(); ContentIterator != Item->GetContent().rend(); ++ContentIterator)
		{
			ToDo.push(*ContentIterator);
		}
	}
}
