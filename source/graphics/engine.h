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

#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H

#include <assert.h>

#include <vector>

#include "../callbacks/callbacks.h"

namespace Graphics
{
	class MeshManager;
	class ModelManager;
	class Node;
	class Scene;
	class TextureManager;
	
	class Engine
	{
	public:
		// constructor & destructor
		Engine(void);
		~Engine(void);
		// getters
		Callback1< void, Graphics::Node * > GetDestroyCallback(void);
		Graphics::MeshManager * GetMeshManager(void);
		Graphics::ModelManager * GetModelManager(void);
		Graphics::TextureManager * GetTextureManager(void);
		// modifiers
		void AddScene(Graphics::Scene * Scene);
		void RemoveScene(Graphics::Scene * Scene);
		void SetDestroyCallback(Callback1< void, Graphics::Node * > Callback);
		void UnsetDestroyCallback(void);
		void OnDestroy(Graphics::Node * Node);
	private:
		Callback1< void, Graphics::Node * > _DestroyCallback;
		Graphics::MeshManager * _MeshManager;
		Graphics::ModelManager * _ModelManager;
		std::vector< Graphics::Scene * > _Scenes;
		Graphics::TextureManager * _TextureManager;
	};
	
	inline Callback1< void, Graphics::Node * > Engine::GetDestroyCallback(void)
	{
		return _DestroyCallback;
	}
	
	inline Graphics::MeshManager * Engine::GetMeshManager(void)
	{
		return _MeshManager;
	}
	
	inline Graphics::ModelManager * Engine::GetModelManager(void)
	{
		return _ModelManager;
	}
	
	inline Graphics::TextureManager * Engine::GetTextureManager(void)
	{
		return _TextureManager;
	}
	
	inline void Engine::SetDestroyCallback(Callback1< void, Graphics::Node * > Callback)
	{
		_DestroyCallback = Callback;
	}
}

#endif
