/**
 * galactic-fall
 * Copyright (C) 2008  Hagen Möbius
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

#include "graphics_material.h"
#include "visualization_prototype.h"

VisualizationPrototype::VisualizationPrototype(void) :
	m_Model(0)
{
}

VisualizationPrototype::VisualizationPrototype(const VisualizationPrototype * VisualizationPrototype) :
	m_Model(VisualizationPrototype->m_Model)
{
	for(std::map< std::string, Graphics::Material * >::const_iterator PartMaterialIterator = VisualizationPrototype->m_PartMaterials.begin(); PartMaterialIterator != VisualizationPrototype->m_PartMaterials.end(); ++PartMaterialIterator)
	{
		m_PartMaterials[PartMaterialIterator->first] = new Graphics::Material(PartMaterialIterator->second);
	}
}

VisualizationPrototype::~VisualizationPrototype(void)
{
	m_Model = 0;
	while(m_PartMaterials.empty() == false)
	{
		delete m_PartMaterials.begin()->second;
		m_PartMaterials.erase(m_PartMaterials.begin());
	}
}

void VisualizationPrototype::SetPartMaterial(const std::string & PartIdentifier, Graphics::Material * PartMaterial)
{
	std::map< std::string, Graphics::Material * >::iterator PartIterator(m_PartMaterials.find(PartIdentifier));
	
	if(PartIterator != m_PartMaterials.end())
	{
		delete PartIterator->second;
		PartIterator->second = PartMaterial;
	}
	else
	{
		m_PartMaterials[PartIdentifier] = PartMaterial;
	}
}
