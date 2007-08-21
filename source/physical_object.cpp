/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#include "graphics_node.h"
#include "physical_object.h"

std::map< Graphics::Node *, PhysicalObject * > PhysicalObject::m_VisualizationBackReferences;

PhysicalObject::~PhysicalObject(void)
{
	while(m_Visualizations.empty() == false)
	{
		std::map< Graphics::Node *, PhysicalObject * >::iterator VisualizationBackReferenceIterator(m_VisualizationBackReferences.find(m_Visualizations.back()));
		
		if(VisualizationBackReferenceIterator != m_VisualizationBackReferences.end())
		{
			m_VisualizationBackReferences.erase(VisualizationBackReferenceIterator);
		}
		m_Visualizations.back()->Remove();
		m_Visualizations.back()->Destroy();
		m_Visualizations.pop_back();
	}
}
