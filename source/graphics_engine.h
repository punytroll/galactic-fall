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

#include "callbacks.h"

namespace Graphics
{
	class Node;
	class Scene;
	
	class Engine
	{
	public:
		void AddScene(Graphics::Scene * Scene);
		void RemoveScene(Graphics::Scene * Scene);
		void SetOnDestroyCallback(Callback1< void, Graphics::Node * > * Callback);
		void UnsetOnDestroyCallback(void);
		void OnDestroy(Graphics::Node * Node);
		Callback1< void, Graphics::Node * > * GetOnDestroyCallback(void);
	private:
		std::vector< Graphics::Scene * > m_Scenes;
		Callback1< void, Graphics::Node * > * m_OnDestroyCallback;
	};
}

inline void Graphics::Engine::SetOnDestroyCallback(Callback1< void, Graphics::Node * > * Callback)
{
	assert(m_OnDestroyCallback == 0);
	m_OnDestroyCallback = Callback;
}

inline void Graphics::Engine::UnsetOnDestroyCallback(void)
{
	m_OnDestroyCallback = 0;
}

inline void Graphics::Engine::OnDestroy(Graphics::Node * Node)
{
	if(m_OnDestroyCallback != 0)
	{
		(*m_OnDestroyCallback)(Node);
	}
}

inline Callback1< void, Graphics::Node * > * Graphics::Engine::GetOnDestroyCallback(void)
{
	return m_OnDestroyCallback;
}

#endif
