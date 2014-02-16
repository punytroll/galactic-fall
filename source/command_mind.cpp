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

#include <assert.h>

#include <algorithm>

#include "character.h"
#include "command_mind.h"
#include "commodity.h"
#include "object_aspect_position.h"
#include "planet.h"
#include "ship.h"
#include "system.h"

void CommandMind::HandleMessage(Message * Message)
{
	// intentionally left empty
}

void CommandMind::Update(void)
{
	// intentionally left empty
}

void CommandMind::EnableAccelerate(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	GetCharacter()->GetShip()->SetAccelerate(true);
}

void CommandMind::DisableAccelerate(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	GetCharacter()->GetShip()->SetAccelerate(false);
}

void CommandMind::EnableTurnLeft(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	GetCharacter()->GetShip()->SetTurnLeft(1.0f);
}

void CommandMind::DisableTurnLeft(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	GetCharacter()->GetShip()->SetTurnLeft(0.0f);
}

void CommandMind::EnableTurnRight(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	GetCharacter()->GetShip()->SetTurnRight(1.0f);
}

void CommandMind::DisableTurnRight(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	GetCharacter()->GetShip()->SetTurnRight(0.0f);
}

void CommandMind::EnableFire(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	GetCharacter()->GetShip()->SetFire(true);
}

void CommandMind::DisableFire(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	GetCharacter()->GetShip()->SetFire(false);
}

void CommandMind::Jettison(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	GetCharacter()->GetShip()->SetJettison(true);
}

void CommandMind::Jump(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	GetCharacter()->GetShip()->SetJump(true);
}

void CommandMind::Land(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	GetCharacter()->GetShip()->SetLand(true);
}

void CommandMind::Refuel(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	GetCharacter()->GetShip()->SetRefuel(true);
}

void CommandMind::Scoop(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	GetCharacter()->GetShip()->SetScoop(true);
}

void CommandMind::SelectLinkedSystem(System * LinkedSystem)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	GetCharacter()->GetShip()->SetLinkedSystemTarget(LinkedSystem);
}

void CommandMind::SelectNextLinkedSystem(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	
	auto CurrentSystem(GetCharacter()->GetShip()->GetSystem());
	
	assert(CurrentSystem != nullptr);
	
	const std::list< System * > & LinkedSystems(CurrentSystem->GetLinkedSystems());
	
	if(GetCharacter()->GetShip()->GetLinkedSystemTarget() != nullptr)
	{
		std::list< System * >::const_iterator SystemIterator(find(LinkedSystems.begin(), LinkedSystems.end(), GetCharacter()->GetShip()->GetLinkedSystemTarget()));
		
		if(SystemIterator == LinkedSystems.end())
		{
			GetCharacter()->GetShip()->SetLinkedSystemTarget(nullptr);
		}
		else
		{
			++SystemIterator;
			if(SystemIterator == LinkedSystems.end())
			{
				GetCharacter()->GetShip()->SetLinkedSystemTarget(nullptr);
			}
			else
			{
				GetCharacter()->GetShip()->SetLinkedSystemTarget(*SystemIterator);
			}
		}
	}
	else
	{
		if(LinkedSystems.size() > 0)
		{
			GetCharacter()->GetShip()->SetLinkedSystemTarget(LinkedSystems.front());
		}
	}
}

void CommandMind::TakeOff(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	GetCharacter()->GetShip()->SetTakeOff(true);
}

void CommandMind::TargetPreviousCargo(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	
	auto CurrentSystem(GetCharacter()->GetShip()->GetSystem());
	
	assert(CurrentSystem != nullptr);
	
	const std::list< Commodity * > & Commodities(CurrentSystem->GetCommodities());
	const Commodity * SelectedCommodity(dynamic_cast< const Commodity * >(GetCharacter()->GetShip()->GetTarget().Get()));
	
	if(SelectedCommodity == nullptr)
	{
		if(Commodities.size() > 0)
		{
			GetCharacter()->GetShip()->SetTarget(Commodities.back()->GetReference());
		}
	}
	else
	{
		std::list< Commodity * >::const_iterator CommodityIterator(find(Commodities.begin(), Commodities.end(), SelectedCommodity));
		
		if(CommodityIterator == Commodities.begin())
		{
			GetCharacter()->GetShip()->SetTarget(0);
		}
		else
		{
			--CommodityIterator;
			GetCharacter()->GetShip()->SetTarget((*CommodityIterator)->GetReference());
		}
	}
}

void CommandMind::TargetNearestCargo(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	
	auto CurrentSystem(GetCharacter()->GetShip()->GetSystem());
	
	assert(CurrentSystem != nullptr);
	
	const std::list< Commodity * > & Commodities(CurrentSystem->GetCommodities());
	float MinimumDistance(0.0f);
	Commodity * MinimumCargo(nullptr);
	
	for(std::list< Commodity * >::const_iterator CommodityIterator = Commodities.begin(); CommodityIterator != Commodities.end(); ++CommodityIterator)
	{
		if(MinimumCargo == nullptr)
		{
			MinimumCargo = *CommodityIterator;
			MinimumDistance = (MinimumCargo->GetAspectPosition()->GetPosition() - GetCharacter()->GetShip()->GetAspectPosition()->GetPosition()).SquaredLength();
		}
		else
		{
			float Distance(((*CommodityIterator)->GetAspectPosition()->GetPosition() - GetCharacter()->GetShip()->GetAspectPosition()->GetPosition()).SquaredLength());
			
			if(Distance < MinimumDistance)
			{
				MinimumCargo = *CommodityIterator;
				MinimumDistance = Distance;
			}
		}
	}
	if(MinimumCargo != nullptr)
	{
		GetCharacter()->GetShip()->SetTarget(MinimumCargo->GetReference());
	}
	else
	{
		GetCharacter()->GetShip()->SetTarget(0);
	}
}

void CommandMind::TargetNextCargo(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	
	auto CurrentSystem(GetCharacter()->GetShip()->GetSystem());
	
	assert(CurrentSystem != nullptr);
	
	const std::list< Commodity * > & Commodities(CurrentSystem->GetCommodities());
	const Commodity * SelectedCommodity(dynamic_cast< const Commodity * >(GetCharacter()->GetShip()->GetTarget().Get()));
	
	if(SelectedCommodity == nullptr)
	{
		if(Commodities.size() > 0)
		{
			GetCharacter()->GetShip()->SetTarget(Commodities.front()->GetReference());
		}
	}
	else
	{
		std::list< Commodity * >::const_iterator CommodityIterator(find(Commodities.begin(), Commodities.end(), SelectedCommodity));
		
		if(CommodityIterator == Commodities.end())
		{
			GetCharacter()->GetShip()->SetTarget(0);
		}
		else
		{
			++CommodityIterator;
			if(CommodityIterator == Commodities.end())
			{
				GetCharacter()->GetShip()->SetTarget(0);
			}
			else
			{
				GetCharacter()->GetShip()->SetTarget((*CommodityIterator)->GetReference());
			}
		}
	}
}

void CommandMind::TargetPreviousPlanet(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	
	auto CurrentSystem(GetCharacter()->GetShip()->GetSystem());
	
	assert(CurrentSystem != nullptr);
	
	const std::vector< Planet * > & Planets(CurrentSystem->GetPlanets());
	const Planet * SelectedPlanet(dynamic_cast< const Planet * >(GetCharacter()->GetShip()->GetTarget().Get()));
	
	if(SelectedPlanet == nullptr)
	{
		if(Planets.size() > 0)
		{
			GetCharacter()->GetShip()->SetTarget(Planets.back()->GetReference());
		}
	}
	else
	{
		std::vector< Planet * >::const_iterator PlanetIterator(find(Planets.begin(), Planets.end(), SelectedPlanet));
		
		if(PlanetIterator == Planets.begin())
		{
			GetCharacter()->GetShip()->SetTarget(0);
		}
		else
		{
			--PlanetIterator;
			GetCharacter()->GetShip()->SetTarget((*PlanetIterator)->GetReference());
		}
	}
}

void CommandMind::TargetNearestPlanet(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	
	auto CurrentSystem(GetCharacter()->GetShip()->GetSystem());
	
	assert(CurrentSystem != nullptr);
	
	const std::vector< Planet * > & Planets(CurrentSystem->GetPlanets());
	float MinimumDistance(0.0f);
	Planet * MinimumPlanet(nullptr);
	
	for(std::vector< Planet * >::const_iterator PlanetIterator = Planets.begin(); PlanetIterator != Planets.end(); ++PlanetIterator)
	{
		if(MinimumPlanet == nullptr)
		{
			MinimumPlanet = *PlanetIterator;
			MinimumDistance = (MinimumPlanet->GetAspectPosition()->GetPosition() - GetCharacter()->GetShip()->GetAspectPosition()->GetPosition()).SquaredLength();
		}
		else
		{
			float Distance(((*PlanetIterator)->GetAspectPosition()->GetPosition() - GetCharacter()->GetShip()->GetAspectPosition()->GetPosition()).SquaredLength());
			
			if(Distance < MinimumDistance)
			{
				MinimumPlanet = *PlanetIterator;
				MinimumDistance = Distance;
			}
		}
	}
	if(MinimumPlanet != nullptr)
	{
		GetCharacter()->GetShip()->SetTarget(MinimumPlanet->GetReference());
	}
	else
	{
		GetCharacter()->GetShip()->SetTarget(0);
	}
}

void CommandMind::TargetNextPlanet(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	
	auto CurrentSystem(GetCharacter()->GetShip()->GetSystem());
	
	assert(CurrentSystem != nullptr);
	
	const std::vector< Planet * > & Planets(CurrentSystem->GetPlanets());
	const Planet * SelectedPlanet(dynamic_cast< const Planet * >(GetCharacter()->GetShip()->GetTarget().Get()));
	
	if(SelectedPlanet == nullptr)
	{
		if(Planets.size() > 0)
		{
			GetCharacter()->GetShip()->SetTarget(Planets.front()->GetReference());
		}
	}
	else
	{
		std::vector< Planet * >::const_iterator PlanetIterator(find(Planets.begin(), Planets.end(), SelectedPlanet));
		
		if(PlanetIterator == Planets.end())
		{
			GetCharacter()->GetShip()->SetTarget(0);
		}
		else
		{
			++PlanetIterator;
			if(PlanetIterator == Planets.end())
			{
				GetCharacter()->GetShip()->SetTarget(0);
			}
			else
			{
				GetCharacter()->GetShip()->SetTarget((*PlanetIterator)->GetReference());
			}
		}
	}
}

void CommandMind::TargetNextShip(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	
	auto CurrentSystem(GetCharacter()->GetShip()->GetSystem());
	
	assert(CurrentSystem != nullptr);
	
	const std::list< Ship * > & Ships(CurrentSystem->GetShips());
	const Ship * SelectedShip(dynamic_cast< const Ship * >(GetCharacter()->GetShip()->GetTarget().Get()));
	
	if(SelectedShip == nullptr)
	{
		if(Ships.size() > 0)
		{
			GetCharacter()->GetShip()->SetTarget(Ships.front()->GetReference());
		}
	}
	else
	{
		std::list< Ship * >::const_iterator ShipIterator(find(Ships.begin(), Ships.end(), SelectedShip));
		
		if(ShipIterator == Ships.end())
		{
			GetCharacter()->GetShip()->SetTarget(0);
		}
		else
		{
			++ShipIterator;
			if(ShipIterator == Ships.end())
			{
				GetCharacter()->GetShip()->SetTarget(0);
			}
			else
			{
				GetCharacter()->GetShip()->SetTarget((*ShipIterator)->GetReference());
			}
		}
	}
}

void CommandMind::TargetPreviousShip(void)
{
	assert(GetCharacter() != nullptr);
	assert(GetCharacter()->GetShip() != nullptr);
	
	auto CurrentSystem(GetCharacter()->GetShip()->GetSystem());
	
	assert(CurrentSystem != nullptr);
	
	const std::list< Ship * > & Ships(CurrentSystem->GetShips());
	const Ship * SelectedShip(dynamic_cast< const Ship * >(GetCharacter()->GetShip()->GetTarget().Get()));
	
	if(SelectedShip == nullptr)
	{
		if(Ships.size() > 0)
		{
			GetCharacter()->GetShip()->SetTarget(Ships.back()->GetReference());
		}
	}
	else
	{
		std::list< Ship * >::const_iterator ShipIterator(find(Ships.begin(), Ships.end(), SelectedShip));
		
		if(ShipIterator == Ships.begin())
		{
			GetCharacter()->GetShip()->SetTarget(0);
		}
		else
		{
			--ShipIterator;
			GetCharacter()->GetShip()->SetTarget((*ShipIterator)->GetReference());
		}
	}
}
