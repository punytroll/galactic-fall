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

#include <algorithm>

#include "engine.h"
#include "mesh_manager.h"
#include "model_manager.h"
#include "scene.h"
#include "texture_manager.h"

Graphics::Engine::Engine(void) :
	_MeshManager(new Graphics::MeshManager()),
	_ModelManager(new Graphics::ModelManager()),
	_TextureManager(new Graphics::TextureManager())
{
}

Graphics::Engine::~Engine(void)
{
	assert(_Scenes.empty() == true);
	delete _MeshManager;
	_MeshManager = 0;
	delete _ModelManager;
	_ModelManager = 0;
	delete _TextureManager;
	_TextureManager = 0;
}

void Graphics::Engine::AddScene(Graphics::Scene * Scene)
{
	assert(Scene != 0);
	assert(Scene->GetEngine() == 0);
	_Scenes.push_back(Scene);
	Scene->_SetEngine(this);
}

void Graphics::Engine::RemoveScene(Graphics::Scene * Scene)
{
	assert(Scene != 0);
	assert(Scene->GetEngine() == this);
	_Scenes.erase(std::find(_Scenes.begin(), _Scenes.end(), Scene));
}
