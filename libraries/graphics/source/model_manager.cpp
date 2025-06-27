/**
 * galactic-fall
 * Copyright (C) 2006-2025  Hagen Möbius
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

#include <graphics/model.h>
#include <graphics/model_manager.h>

Graphics::ModelManager::~ModelManager(void)
{
	while(m_Models.size() > 0)
	{
		Destroy(m_Models.begin()->first);
	}
}

const Graphics::Model * Graphics::ModelManager::Get(const std::string & Identifier) const
{
	std::map< std::string, Graphics::Model * >::const_iterator ModelIterator(m_Models.find(Identifier));
	
	if(ModelIterator == m_Models.end())
	{
		return 0;
	}
	else
	{
		return ModelIterator->second;
	}
}

Graphics::Model * Graphics::ModelManager::Create(const std::string & Identifier)
{
	if(m_Models.find(Identifier) != m_Models.end())
	{
		return 0;
	}
	else
	{
		Model * NewModel(new Graphics::Model(Identifier));
		
		m_Models[NewModel->GetIdentifier()] = NewModel;
		
		return NewModel;
	}
}

void Graphics::ModelManager::Destroy(const std::string & Identifier)
{
	std::map< std::string, Graphics::Model * >::iterator ModelIterator(m_Models.find(Identifier));
	
	if(ModelIterator != m_Models.end())
	{
		delete ModelIterator->second;
		m_Models.erase(ModelIterator);
	}
}
