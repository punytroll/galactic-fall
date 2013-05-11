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

#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H

namespace Graphics
{
	class Camera;
	class Engine;
	class Light;
	class Node;
	
	class Scene
	{
		friend class Graphics::Engine;
	public:
		Scene(void);
		~Scene(void);
		// getters
		Graphics::Camera * GetCamera(void);
		Graphics::Engine * GetEngine(void);
		Graphics::Light * GetLight(void);
		Graphics::Node * GetRootNode(void);
		// setters
		void SetRootNode(Graphics::Node * RootNode);
		// modifiers
		void ActivateLight(void);
		void DeactivateLight(void);
		void Render(void);
		void Update(float Seconds);
	private:
		void _SetEngine(Graphics::Engine * Engine);
		void _Update(Graphics::Node * Node, float Seconds);
		Graphics::Camera * _Camera;
		Graphics::Engine * _Engine;
		Graphics::Light * _Light;
		Graphics::Node * _RootNode;
	};
	
	inline Graphics::Camera * Scene::GetCamera(void)
	{
		return _Camera;
	}
	
	inline Graphics::Engine * Scene::GetEngine(void)
	{
		return _Engine;
	}
	
	inline Graphics::Light * Scene::GetLight(void)
	{
		return _Light;
	}
	
	inline Graphics::Node * Scene::GetRootNode(void)
	{
		return _RootNode;
	}
}

#endif
