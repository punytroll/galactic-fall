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
	_VisualizationPrototype(nullptr)
{
}

ObjectAspectVisualization::~ObjectAspectVisualization(void)
{
	assert(_Visualizations.empty() == true);
	delete _VisualizationPrototype;
	_VisualizationPrototype = nullptr;
}

void ObjectAspectVisualization::AddGraphics(Graphics::Node * Graphics)
{
	assert(Graphics != nullptr);
	
	auto NewVisualization(new Visualization());
	
	NewVisualization->SetGraphics(Graphics);
	_Visualizations.push_back(NewVisualization);
}

Visualization * ObjectAspectVisualization::GetVisualization(void)
{
	if(_Visualizations.empty() == true)
	{
		return nullptr;
	}
	else
	{
		assert(_Visualizations.front()->GetGraphics() != nullptr);
		return _Visualizations.front();
	}
}

void ObjectAspectVisualization::SetVisualizationPrototype(VisualizationPrototype * VisualizationPrototype)
{
	delete _VisualizationPrototype;
	_VisualizationPrototype = VisualizationPrototype;
}

void ObjectAspectVisualization::Destroy(void)
{
	while(_Visualizations.empty() == false)
	{
		auto OldCount(_Visualizations.size());
		auto Visualization(_Visualizations.front());
		
		assert(Visualization->GetGraphics() != nullptr);
		Visualization->GetGraphics()->Destroy();
		assert(_Visualizations.size() + 1 == OldCount);
	}
}

void ObjectAspectVisualization::DestroyVisualization(Graphics::Node * Container)
{
	assert(Container != nullptr);
	
	auto Found(false);
	
	for(auto VisualizationIterator = _Visualizations.begin(); VisualizationIterator != _Visualizations.end(); ++VisualizationIterator)
	{
		assert((*VisualizationIterator)->GetGraphics() != nullptr);
		if((*VisualizationIterator)->GetGraphics()->GetContainer() == Container)
		{
			Found = true;
			
			auto OldCount(_Visualizations.size());
			
			(*VisualizationIterator)->GetGraphics()->Destroy();
			assert(_Visualizations.size() + 1 == OldCount);
			
			break;
		}
	}
	assert(Found == true);
}

void ObjectAspectVisualization::RemoveGraphics(Graphics::Node * Graphics)
{
	assert(Graphics != nullptr);
	
	auto OldCount(_Visualizations.size());
	
	for(auto VisualizationIterator = _Visualizations.begin(); VisualizationIterator != _Visualizations.end(); ++VisualizationIterator)
	{
		if((*VisualizationIterator)->GetGraphics() == Graphics)
		{
			(*VisualizationIterator)->SetGraphics(nullptr);
			delete *VisualizationIterator;
			_Visualizations.erase(VisualizationIterator);
			
			break;
		}
	}
	assert(_Visualizations.size() + 1 == OldCount);
}
