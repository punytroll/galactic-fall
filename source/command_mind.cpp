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

#include "character.h"
#include "command_mind.h"
#include "commodity.h"
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
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	GetCharacter()->GetShip()->SetAccelerate(true);
}

void CommandMind::DisableAccelerate(void)
{
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	GetCharacter()->GetShip()->SetAccelerate(false);
}

void CommandMind::EnableTurnLeft(void)
{
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	GetCharacter()->GetShip()->SetTurnLeft(1.0f);
}

void CommandMind::DisableTurnLeft(void)
{
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	GetCharacter()->GetShip()->SetTurnLeft(0.0f);
}

void CommandMind::EnableTurnRight(void)
{
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	GetCharacter()->GetShip()->SetTurnRight(1.0f);
}

void CommandMind::DisableTurnRight(void)
{
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	GetCharacter()->GetShip()->SetTurnRight(0.0f);
}

void CommandMind::EnableFire(void)
{
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	GetCharacter()->GetShip()->SetFire(true);
}

void CommandMind::DisableFire(void)
{
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	GetCharacter()->GetShip()->SetFire(false);
}

void CommandMind::Jettison(void)
{
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	GetCharacter()->GetShip()->m_Jettison = true;
}

void CommandMind::Jump(void)
{
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	GetCharacter()->GetShip()->m_Jump = true;
}

void CommandMind::Land(void)
{
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	GetCharacter()->GetShip()->m_Land = true;
}

void CommandMind::Refuel(void)
{
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	GetCharacter()->GetShip()->SetRefuel(true);
}

void CommandMind::Scoop(void)
{
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	GetCharacter()->GetShip()->m_Scoop = true;
}

void CommandMind::SelectLinkedSystem(System * LinkedSystem)
{
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	GetCharacter()->GetShip()->SetLinkedSystemTarget(LinkedSystem);
}

void CommandMind::SelectNextLinkedSystem(void)
{
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	const std::list< System * > & LinkedSystems(GetCharacter()->GetShip()->GetCurrentSystem()->GetLinkedSystems());
	
	if(GetCharacter()->GetShip()->GetLinkedSystemTarget() != 0)
	{
		std::list< System * >::const_iterator SystemIterator(find(LinkedSystems.begin(), LinkedSystems.end(), GetCharacter()->GetShip()->GetLinkedSystemTarget()));
		
		if(SystemIterator == LinkedSystems.end())
		{
			GetCharacter()->GetShip()->SetLinkedSystemTarget(0);
		}
		else
		{
			++SystemIterator;
			if(SystemIterator == LinkedSystems.end())
			{
				GetCharacter()->GetShip()->SetLinkedSystemTarget(0);
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
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	GetCharacter()->GetShip()->SetTakeOff(true);
}

void CommandMind::TargetPreviousCargo(void)
{
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	const std::list< Commodity * > & Commodities(GetCharacter()->GetShip()->GetCurrentSystem()->GetCommodities());
	const Commodity * SelectedCommodity(dynamic_cast< const Commodity * >(GetCharacter()->GetShip()->GetTarget().Get()));
	
	if(SelectedCommodity == 0)
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
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	const std::list< Commodity * > & Commodities(GetCharacter()->GetShip()->GetCurrentSystem()->GetCommodities());
	float MinimumDistance(0.0f);
	Commodity * MinimumCargo(0);
	
	for(std::list< Commodity * >::const_iterator CommodityIterator = Commodities.begin(); CommodityIterator != Commodities.end(); ++CommodityIterator)
	{
		if(MinimumCargo == 0)
		{
			MinimumCargo = *CommodityIterator;
			MinimumDistance = (MinimumCargo->GetPosition() - GetCharacter()->GetShip()->GetPosition()).SquaredLength();
		}
		else
		{
			float Distance(((*CommodityIterator)->GetPosition() - GetCharacter()->GetShip()->GetPosition()).SquaredLength());
			
			if(Distance < MinimumDistance)
			{
				MinimumCargo = *CommodityIterator;
				MinimumDistance = Distance;
			}
		}
	}
	if(MinimumCargo != 0)
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
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	const std::list< Commodity * > & Commodities(GetCharacter()->GetShip()->GetCurrentSystem()->GetCommodities());
	const Commodity * SelectedCommodity(dynamic_cast< const Commodity * >(GetCharacter()->GetShip()->GetTarget().Get()));
	
	if(SelectedCommodity == 0)
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
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	const std::vector< Planet * > & Planets(GetCharacter()->GetShip()->GetCurrentSystem()->GetPlanets());
	const Planet * SelectedPlanet(dynamic_cast< const Planet * >(GetCharacter()->GetShip()->GetTarget().Get()));
	
	if(SelectedPlanet == 0)
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
	const std::vector< Planet * > & Planets(GetCharacter()->GetShip()->GetCurrentSystem()->GetPlanets());
	float MinimumDistance(0.0f);
	Planet * MinimumPlanet(0);
	
	for(std::vector< Planet * >::const_iterator PlanetIterator = Planets.begin(); PlanetIterator != Planets.end(); ++PlanetIterator)
	{
		if(MinimumPlanet == 0)
		{
			MinimumPlanet = *PlanetIterator;
			MinimumDistance = (MinimumPlanet->GetPosition() - GetCharacter()->GetShip()->GetPosition()).SquaredLength();
		}
		else
		{
			float Distance(((*PlanetIterator)->GetPosition() - GetCharacter()->GetShip()->GetPosition()).SquaredLength());
			
			if(Distance < MinimumDistance)
			{
				MinimumPlanet = *PlanetIterator;
				MinimumDistance = Distance;
			}
		}
	}
	if(MinimumPlanet != 0)
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
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	const std::vector< Planet * > & Planets(GetCharacter()->GetShip()->GetCurrentSystem()->GetPlanets());
	const Planet * SelectedPlanet(dynamic_cast< const Planet * >(GetCharacter()->GetShip()->GetTarget().Get()));
	
	if(SelectedPlanet == 0)
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
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	const std::list< Ship * > & Ships(GetCharacter()->GetShip()->GetCurrentSystem()->GetShips());
	const Ship * SelectedShip(dynamic_cast< const Ship * >(GetCharacter()->GetShip()->GetTarget().Get()));
	
	if(SelectedShip == 0)
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
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	const std::list< Ship * > & Ships(GetCharacter()->GetShip()->GetCurrentSystem()->GetShips());
	const Ship * SelectedShip(dynamic_cast< const Ship * >(GetCharacter()->GetShip()->GetTarget().Get()));
	
	if(SelectedShip == 0)
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

void CommandMind::TargetPhysicalObject(Reference< PhysicalObject > & PhysicalObject)
{
	assert(GetCharacter() != 0);
	assert(GetCharacter()->GetShip() != 0);
	
	GetCharacter()->GetShip()->SetTarget(PhysicalObject);
}
