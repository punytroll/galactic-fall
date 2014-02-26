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

#include "gl.h"
#include "light.h"
#include "node.h"
#include "particle_system.h"
#include "scene.h"

Graphics::Scene::Scene(void) :
	_Engine(0),
	_Light(0),
	_RootNode(0)
{
}

Graphics::Scene::~Scene(void)
{
	if(_Light != 0)
	{
		delete _Light;
		_Light = 0;
	}
	if(_RootNode != 0)
	{
		_RootNode->Destroy();
		_RootNode = 0;
	}
}

void Graphics::Scene::_SetEngine(Graphics::Engine * Engine)
{
	if(Engine == 0)
	{
		assert(_Engine != 0);
		_Engine = 0;
	}
	else
	{
		assert(_Engine == 0);
		_Engine = Engine;
	}
}

void Graphics::Scene::SetRootNode(Graphics::Node * RootNode)
{
	assert(_RootNode == 0);
	assert(RootNode != 0);
	assert(RootNode->GetScene() == 0);
	_RootNode = RootNode;
	_RootNode->_SetScene(this);
}

void Graphics::Scene::ActivateLight(void)
{
	if(_Light == 0)
	{
		_Light = new Graphics::Light();
	}
}

void Graphics::Scene::DeactivateLight(void)
{
	if(_Light != 0)
	{
		delete _Light;
		_Light = 0;
	}
}

void Graphics::Scene::OnDestroy(Graphics::Node * Node)
{
	assert(Node->GetContainer() == 0);
	assert(Node->GetScene() == 0);
	assert(Node->GetContent().empty() == true);
	if(_DestroyCallback)
	{
		_DestroyCallback(Node);
	}
}

void Graphics::Scene::Render(void)
{
	assert(_RootNode != 0);
	if(_Light != 0)
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
	_RootNode->Begin();
	_RootNode->Draw();
	_RootNode->End();
}
