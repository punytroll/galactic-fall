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

#include <float.h>

#include <algorithm>

#include "commodity.h"
#include "globals.h"
#include "graphics/system_node.h"
#include "object_aspect_object_container.h"
#include "object_aspect_visualization.h"
#include "planet.h"
#include "ship.h"
#include "shot.h"
#include "star.h"
#include "system.h"
#include "visualization.h"
#include "visualizations.h"

namespace std
{
	template< typename Type1, typename Type2 >
	std::list< std::pair< Type1, Type2 > > tuples(const std::list< Type1 > & One, const std::list< Type2 > & Two)
	{
		std::list< std::pair< Type1, Type2 > > Result;
		
		for(auto ValueOne : One)
		{
			for(auto ValueTwo : Two)
			{
				Result.push_back(std::make_pair(ValueOne, ValueTwo));
			}
		}
		
		return Result;
	}
}

System::System(const std::string & Identifier) :
	_Identifier(Identifier),
	_TrafficDensity(FLT_MAX),
	_Star(nullptr)
{
	// initialize object aspects
	AddAspectName();
	AddAspectObjectContainer();
	GetAspectObjectContainer()->SetOnAddedCallback(Callback(this, &System::_OnAdded));
	GetAspectObjectContainer()->SetOnRemovedCallback(Callback(this, &System::_OnRemoved));
	AddAspectPosition();
	AddAspectVisualization();
}

System::~System(void)
{
	assert(_Commodities.empty() == true);
	assert(_Planets.empty() == true);
	assert(_Ships.empty() == true);
	assert(_Shots.empty() == true);
	assert(_Star == nullptr);
}

bool System::IsLinkedToSystem(const System * LinkedSystem) const
{
	for(auto LinkedSystem : _LinkedSystems)
	{
		if(LinkedSystem == LinkedSystem)
		{
			return true;
		}
	}
	
	return false;
}

void System::_OnAdded(Object * Content)
{
	assert(Content != nullptr);
	if(Content->GetTypeIdentifier() == "commodity")
	{
		auto TheCommodity(dynamic_cast< Commodity * >(Content));
		
		assert(TheCommodity != nullptr);
		_Commodities.push_back(TheCommodity);
		assert(GetAspectVisualization() != nullptr);
		for(auto Visualization : GetAspectVisualization()->GetVisualizations())
		{
			assert(Visualization->GetGraphics() != nullptr);
			VisualizeObject(TheCommodity, (static_cast< Graphics::SystemNode * >(Visualization->GetGraphics()))->GetCommodityLayer());
		}
	}
	else if(Content->GetTypeIdentifier() == "planet")
	{
		auto ThePlanet(dynamic_cast< Planet * >(Content));
		
		assert(ThePlanet != nullptr);
		for(auto Planet : _Planets)
		{
			if(Planet->GetIdentifier() == ThePlanet->GetClassIdentifier())
			{
				assert(false);
			}
		}
		_Planets.push_back(ThePlanet);
	}
	else if(Content->GetTypeIdentifier() == "ship")
	{
		auto TheShip(dynamic_cast< Ship * >(Content));
		
		assert(TheShip != nullptr);
		_Ships.push_back(TheShip);
		assert(GetAspectVisualization() != nullptr);
		for(auto Visualization : GetAspectVisualization()->GetVisualizations())
		{
			assert(Visualization->GetGraphics() != nullptr);
			VisualizeObject(TheShip, (static_cast< Graphics::SystemNode * >(Visualization->GetGraphics()))->GetShipLayer());
		}
	}
	else if(Content->GetTypeIdentifier() == "shot")
	{
		auto TheShot(dynamic_cast< Shot * >(Content));
		
		assert(TheShot != nullptr);
		_Shots.push_back(TheShot);
		assert(GetAspectVisualization() != nullptr);
		for(auto Visualization : GetAspectVisualization()->GetVisualizations())
		{
			assert(Visualization->GetGraphics() != nullptr);
			VisualizeObject(TheShot, (static_cast< Graphics::SystemNode * >(Visualization->GetGraphics()))->GetShotLayer());
		}
	}
	else if(Content->GetTypeIdentifier() == "star")
	{
		auto TheStar(dynamic_cast< Star * >(Content));
		
		assert(TheStar != nullptr);
		assert(_Star == nullptr);
		_Star = TheStar;
	}
	else
	{
		assert(false);
	}
}

void System::_OnRemoved(Object * Content)
{
	assert(Content != nullptr);
	if(Content->GetTypeIdentifier() == "commodity")
	{
		auto CommodityIterator(std::find(_Commodities.begin(), _Commodities.end(), Content));
		
		assert(CommodityIterator != _Commodities.end());
		_Commodities.erase(CommodityIterator);
		assert(GetAspectVisualization() != nullptr);
		assert(Content->GetAspectVisualization() != nullptr);
		for(auto Visualizations : std::tuples(Content->GetAspectVisualization()->GetVisualizations(), GetAspectVisualization()->GetVisualizations()))
		{
			if(Visualizations.first->GetGraphics()->GetContainer() == (static_cast< Graphics::SystemNode * >(Visualizations.second->GetGraphics()))->GetCommodityLayer())
			{
				Content->GetAspectVisualization()->DestroyVisualization(Visualizations.first);
			}
		}
	}
	else if(Content->GetTypeIdentifier() == "planet")
	{
		auto PlanetIterator(std::find(_Planets.begin(), _Planets.end(), Content));
		
		assert(PlanetIterator != _Planets.end());
		_Planets.erase(PlanetIterator);
		assert(GetAspectVisualization() != nullptr);
		assert(Content->GetAspectVisualization() != nullptr);
		for(auto Visualizations : std::tuples(Content->GetAspectVisualization()->GetVisualizations(), GetAspectVisualization()->GetVisualizations()))
		{
			if(Visualizations.first->GetGraphics()->GetContainer() == (static_cast< Graphics::SystemNode * >(Visualizations.second->GetGraphics()))->GetPlanetLayer())
			{
				Content->GetAspectVisualization()->DestroyVisualization(Visualizations.first);
			}
		}
	}
	else if(Content->GetTypeIdentifier() == "ship")
	{
		auto ShipIterator(std::find(_Ships.begin(), _Ships.end(), Content));
		
		assert(ShipIterator != _Ships.end());
		_Ships.erase(ShipIterator);
		assert(GetAspectVisualization() != nullptr);
		assert(Content->GetAspectVisualization() != nullptr);
		for(auto Visualizations : std::tuples(Content->GetAspectVisualization()->GetVisualizations(), GetAspectVisualization()->GetVisualizations()))
		{
			if(Visualizations.first->GetGraphics()->GetContainer() == (static_cast< Graphics::SystemNode * >(Visualizations.second->GetGraphics()))->GetShipLayer())
			{
				Content->GetAspectVisualization()->DestroyVisualization(Visualizations.first);
			}
		}
	}
	else if(Content->GetTypeIdentifier() == "shot")
	{
		auto ShotIterator(std::find(_Shots.begin(), _Shots.end(), Content));
		
		assert(ShotIterator != _Shots.end());
		_Shots.erase(ShotIterator);
		assert(GetAspectVisualization() != nullptr);
		assert(Content->GetAspectVisualization() != nullptr);
		for(auto Visualizations : std::tuples(Content->GetAspectVisualization()->GetVisualizations(), GetAspectVisualization()->GetVisualizations()))
		{
			if(Visualizations.first->GetGraphics()->GetContainer() == (static_cast< Graphics::SystemNode * >(Visualizations.second->GetGraphics()))->GetShotLayer())
			{
				Content->GetAspectVisualization()->DestroyVisualization(Visualizations.first);
			}
		}
	}
	else if(Content->GetTypeIdentifier() == "star")
	{
		assert(_Star == Content);
		_Star = nullptr;
	}
	for(auto ShipIterator : _Ships)
	{
		if(ShipIterator->GetTarget() == Content)
		{
			ShipIterator->SetTarget(nullptr);
		}
	}
}

void System::AddLinkedSystem(System * LinkedSystem)
{
	_LinkedSystems.push_back(LinkedSystem);
}
