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

#include "particle_system.h"
#include "scene.h"

Graphics::Scene::Scene(void) :
	_Engine(0),
	_RootNode(0)
{
}

Graphics::Scene::~Scene(void)
{
	if(_RootNode != 0)
	{
		_RootNode->Destroy();
		_RootNode = 0;
	}
}

void Graphics::Scene::Render(void)
{
	assert(_RootNode != 0);
	_RootNode->Render();
}

void Graphics::Scene::_SetEngine(Graphics::Engine * Engine)
{
	assert(_Engine == 0);
	assert(Engine != 0);
	assert(_RootNode == 0);
	_Engine = Engine;
}

void Graphics::Scene::SetRootNode(Graphics::Node * RootNode)
{
	assert(_Engine != 0);
	assert(_RootNode == 0);
	assert(RootNode != 0);
	assert(RootNode->GetEngine() == 0);
	_RootNode = RootNode;
	_RootNode->SetEngine(_Engine);
}

void Graphics::Scene::Update(float Seconds)
{
	assert(_RootNode != 0);
	_Update(_RootNode, Seconds);
}

void Graphics::Scene::_Update(Graphics::Node * Node, float Seconds)
{
	std::vector< Graphics::Node * > & Content(Node->GetContent());
	std::vector< Graphics::Node * >::size_type ContentIndex(0);
	
	while(ContentIndex < Content.size())
	{
		Graphics::ParticleSystem * ParticleSystem(dynamic_cast< Graphics::ParticleSystem * >(Content[ContentIndex]));
		
		if((ParticleSystem != 0) && (ParticleSystem->Update(Seconds) == false))
		{
			Node->RemoveNode(ParticleSystem);
			ParticleSystem->Destroy();
		}
		else
		{
			_Update(Content[ContentIndex], Seconds);
			++ContentIndex;
		}
	}
}
