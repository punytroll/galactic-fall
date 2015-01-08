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

#include "engine.h"
#include "mesh_manager.h"
#include "model_manager.h"
#include "particle_system.h"
#include "shading_manager.h"
#include "texture_manager.h"
#include "view.h"

Graphics::Engine::Engine(void) :
	_MeshManager(new Graphics::MeshManager()),
	_ModelManager(new Graphics::ModelManager()),
	_ShadingManager(new Graphics::ShadingManager()),
	_TextureManager(new Graphics::TextureManager())
{
}

Graphics::Engine::~Engine(void)
{
	assert(_Views.empty() == true);
	assert(_MeshManager != nullptr);
	delete _MeshManager;
	_MeshManager = nullptr;
	assert(_ModelManager != nullptr);
	delete _ModelManager;
	_ModelManager = nullptr;
	assert(_ShadingManager != nullptr);
	delete _ShadingManager;
	_ShadingManager = nullptr;
	assert(_TextureManager != nullptr);
	delete _TextureManager;
	_TextureManager = nullptr;
}

void Graphics::Engine::AddView(Graphics::View * View)
{
	assert(View != nullptr);
	assert(View->GetEngine() == nullptr);
	_Views.push_back(View);
	View->_SetEngine(this);
}

void Graphics::Engine::AddParticleSystem(Graphics::ParticleSystem * ParticleSystem)
{
	_ParticleSystems.insert(ParticleSystem);
}

void Graphics::Engine::RemoveParticleSystem(Graphics::ParticleSystem * ParticleSystem)
{
	_ParticleSystems.erase(ParticleSystem);
}

void Graphics::Engine::RemoveView(Graphics::View * View)
{
	assert(View != nullptr);
	assert(View->GetEngine() == this);
	View->_SetEngine(nullptr);
	_Views.erase(std::find(_Views.begin(), _Views.end(), View));
}

void Graphics::Engine::Update(float Seconds)
{
	auto ParticleSystemIterator(_ParticleSystems.begin());
	
	while(ParticleSystemIterator != _ParticleSystems.end())
	{
		auto ParticleSystem(*ParticleSystemIterator);
		
		ParticleSystem->Update(Seconds);
		if(ParticleSystem->IsDone() == true)
		{
			_ParticleSystems.erase(ParticleSystemIterator++);
			delete ParticleSystem;
		}
		else
		{
			++ParticleSystemIterator;
		}
	}
}
