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

#include "texture.h"
#include "texture_manager.h"

Graphics::TextureManager::~TextureManager(void)
{
	while(m_Managed.empty() == false)
	{
		Destroy(m_Managed.begin()->first);
	}
}

const Graphics::Texture* Graphics::TextureManager::Get(const std::string & Identifier) const
{
	std::map< std::string, Graphics::Texture * >::const_iterator ManagedIterator(m_Managed.find(Identifier));
	
	if(ManagedIterator == m_Managed.end())
	{
		return 0;
	}
	else
	{
		return ManagedIterator->second;
	}
}

Graphics::Texture * Graphics::TextureManager::Create(const std::string & Identifier)
{
	if(m_Managed.find(Identifier) != m_Managed.end())
	{
		return 0;
	}
	else
	{
		Graphics::Texture * Managed(new Graphics::Texture(Identifier));
		
		m_Managed[Managed->GetIdentifier()] = Managed;
		
		return Managed;
	}
}

void Graphics::TextureManager::Destroy(const std::string & Identifier)
{
	std::map< std::string, Graphics::Texture * >::iterator ManagedIterator(m_Managed.find(Identifier));
	
	if(ManagedIterator != m_Managed.end())
	{
		delete ManagedIterator->second;
		m_Managed.erase(ManagedIterator);
	}
}
