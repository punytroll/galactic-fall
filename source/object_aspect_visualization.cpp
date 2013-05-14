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

#include <assert.h>

#include "graphics/node.h"
#include "object_aspect_visualization.h"
#include "visualization_prototype.h"

ObjectAspectVisualization::ObjectAspectVisualization(void) :
	m_VisualizationPrototype(0)
{
}

ObjectAspectVisualization::~ObjectAspectVisualization(void)
{
	assert(m_Visualization.IsValid() == false);
	delete m_VisualizationPrototype;
	m_VisualizationPrototype = 0;
}

void ObjectAspectVisualization::SetVisualization(Reference< Graphics::Node > & Visualization)
{
	assert(Visualization.IsValid() == true);
	assert(m_Visualization.IsValid() == false);
	m_Visualization = Visualization;
}

void ObjectAspectVisualization::SetVisualizationPrototype(VisualizationPrototype * VisualizationPrototype)
{
	delete m_VisualizationPrototype;
	m_VisualizationPrototype = VisualizationPrototype;
}

void ObjectAspectVisualization::Destroy(void)
{
	if(m_Visualization.IsValid() == true)
	{
		Reference< Graphics::Node > Visualization(GetVisualization());
		
		UnsetVisualization();
		Visualization->Destroy();
	}
}

void ObjectAspectVisualization::UnsetVisualization(void)
{
	assert(m_Visualization.IsValid() == true);
	m_Visualization.Clear();
}
