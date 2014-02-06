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
#include "visualization.h"
#include "visualization_prototype.h"

ObjectAspectVisualization::ObjectAspectVisualization(void) :
	_Visualization(nullptr),
	_VisualizationPrototype(nullptr)
{
}

ObjectAspectVisualization::~ObjectAspectVisualization(void)
{
	assert(_Visualization == nullptr);
	delete _VisualizationPrototype;
	_VisualizationPrototype = nullptr;
}

void ObjectAspectVisualization::AddGraphics(Graphics::Node * Graphics)
{
	assert(Graphics != nullptr);
	assert(_Visualization == nullptr);
	_Visualization = new Visualization();
	_Visualization->SetGraphics(Graphics);
}

Visualization * ObjectAspectVisualization::GetVisualization(void)
{
	assert((_Visualization == nullptr) || (_Visualization->GetGraphics() != nullptr));
	
	return _Visualization;
}

void ObjectAspectVisualization::SetVisualizationPrototype(VisualizationPrototype * VisualizationPrototype)
{
	delete _VisualizationPrototype;
	_VisualizationPrototype = VisualizationPrototype;
}

void ObjectAspectVisualization::Destroy(void)
{
	if(_Visualization != nullptr)
	{
		assert(_Visualization->GetGraphics() != nullptr);
		_Visualization->GetGraphics()->Destroy();
		assert(_Visualization == nullptr);
	}
}

void ObjectAspectVisualization::DestroyVisualization(Graphics::Node * Container)
{
	assert((Container != nullptr) && (_Visualization != nullptr) && (_Visualization->GetGraphics() != nullptr) && (_Visualization->GetGraphics()->GetContainer() == Container));
	_Visualization->GetGraphics()->Destroy();
	delete _Visualization;
	_Visualization = nullptr;
}

void ObjectAspectVisualization::RemoveGraphics(Graphics::Node * Graphics)
{
	assert((Graphics != nullptr) && (_Visualization != nullptr) && (_Visualization->GetGraphics() == Graphics));
	_Visualization->SetGraphics(nullptr);
	delete _Visualization;
	_Visualization = nullptr;
}
