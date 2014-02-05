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
	_Visualization(0),
	_VisualizationPrototype(0)
{
}

ObjectAspectVisualization::~ObjectAspectVisualization(void)
{
	delete _Visualization;
	_Visualization = 0;
	delete _VisualizationPrototype;
	_VisualizationPrototype = 0;
}

Visualization * ObjectAspectVisualization::GetVisualization(void)
{
	if((_Visualization != 0) && (_Visualization->GetGraphics() != 0))
	{
		return _Visualization;
	}
	else
	{
		return 0;
	}
}

void ObjectAspectVisualization::SetGraphics(Graphics::Node * Graphics)
{
	assert(Graphics != 0);
	delete _Visualization;
	_Visualization = new Visualization();
	_Visualization->SetGraphics(Graphics);
}

void ObjectAspectVisualization::SetVisualizationPrototype(VisualizationPrototype * VisualizationPrototype)
{
	delete _VisualizationPrototype;
	_VisualizationPrototype = VisualizationPrototype;
}

void ObjectAspectVisualization::Destroy(void)
{
	if(_Visualization != 0)
	{
		if(_Visualization->GetGraphics() != 0)
		{
			DestroyVisualization();
		}
		else
		{
			delete _Visualization;
			_Visualization = 0;
		}
	}
}

void ObjectAspectVisualization::DestroyVisualization(void)
{
	assert((_Visualization != 0) && (_Visualization->GetGraphics() != 0));
	_Visualization->GetGraphics()->Destroy();
	delete _Visualization;
	_Visualization = 0;
}

void ObjectAspectVisualization::UnsetVisualization(void)
{
	assert((_Visualization != 0) && (_Visualization->GetGraphics() != 0));
	_Visualization->SetGraphics(0);
	delete _Visualization;
	_Visualization = 0;
}
