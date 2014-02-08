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
#include "object_aspect_object_container.h"
#include "object_aspect_visualization.h"
#include "planet.h"
#include "ship.h"
#include "shot.h"
#include "star.h"
#include "system.h"
#include "visualizations.h"

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
	for(auto LinkedSystemIterator = _LinkedSystems.begin(); LinkedSystemIterator != _LinkedSystems.end(); ++LinkedSystemIterator)
	{
		if(*LinkedSystemIterator == LinkedSystem)
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
		if(g_CommodityLayer != nullptr)
		{
			VisualizeObject(TheCommodity, g_CommodityLayer);
		}
	}
	else if(Content->GetTypeIdentifier() == "planet")
	{
		auto ThePlanet(dynamic_cast< Planet * >(Content));
		
		assert(ThePlanet != nullptr);
		for(auto PlanetIterator = _Planets.begin(); PlanetIterator != _Planets.end(); ++PlanetIterator)
		{
			if((*PlanetIterator)->GetIdentifier() == ThePlanet->GetClassIdentifier())
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
		if(g_ShipLayer != nullptr)
		{
			VisualizeObject(TheShip, g_ShipLayer);
		}
	}
	else if(Content->GetTypeIdentifier() == "shot")
	{
		auto TheShot(dynamic_cast< Shot * >(Content));
		
		assert(TheShot != nullptr);
		_Shots.push_back(TheShot);
		if(g_ShotLayer != nullptr)
		{
			VisualizeObject(TheShot, g_ShotLayer);
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
		assert(Content->GetAspectVisualization() != nullptr);
		if(Content->GetAspectVisualization()->GetVisualization() != nullptr)
		{
			Content->GetAspectVisualization()->DestroyVisualization(g_CommodityLayer);
		}
	}
	else if(Content->GetTypeIdentifier() == "planet")
	{
		for(auto PlanetIterator = _Planets.begin(); PlanetIterator != _Planets.end(); ++PlanetIterator)
		{
			if((*PlanetIterator)->GetIdentifier() == Content->GetClassIdentifier())
			{
				_Planets.erase(PlanetIterator);
				
				break;
			}
		}
	}
	else if(Content->GetTypeIdentifier() == "ship")
	{
		auto ShipIterator(std::find(_Ships.begin(), _Ships.end(), Content));
		
		assert(ShipIterator != _Ships.end());
		_Ships.erase(ShipIterator);
	}
	else if(Content->GetTypeIdentifier() == "shot")
	{
		auto ShotIterator(std::find(_Shots.begin(), _Shots.end(), Content));
		
		assert(ShotIterator != _Shots.end());
		_Shots.erase(ShotIterator);
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
			ShipIterator->UnsetTarget();
		}
	}
}

void System::AddLinkedSystem(System * LinkedSystem)
{
	_LinkedSystems.push_back(LinkedSystem);
}
