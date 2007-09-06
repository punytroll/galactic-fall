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

#include "graphics_particle_system.h"
#include "graphics_scene.h"

void Graphics::Scene::Update(float Seconds)
{
	Update(this, Seconds);
}

void Graphics::Scene::Update(Graphics::Node * Node, float Seconds)
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
			Update(Content[ContentIndex], Seconds);
			++ContentIndex;
		}
	}
}
