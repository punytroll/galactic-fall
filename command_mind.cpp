#include "cargo.h"
#include "character.h"
#include "command_mind.h"
#include "planet.h"
#include "ship.h"
#include "system.h"

void CommandMind::Update(void)
{
	// intentionally left empty
}

void CommandMind::EnableAccelerate(void)
{
	if((GetCharacter() != 0) && (GetCharacter()->GetShip() != 0))
	{
		GetCharacter()->GetShip()->m_Accelerate = true;
	}
}

void CommandMind::DisableAccelerate(void)
{
	if((GetCharacter() != 0) && (GetCharacter()->GetShip() != 0))
	{
		GetCharacter()->GetShip()->m_Accelerate = false;
	}
}

void CommandMind::EnableTurnLeft(void)
{
	if((GetCharacter() != 0) && (GetCharacter()->GetShip() != 0))
	{
		GetCharacter()->GetShip()->m_TurnLeft = true;
	}
}

void CommandMind::DisableTurnLeft(void)
{
	if((GetCharacter() != 0) && (GetCharacter()->GetShip() != 0))
	{
		GetCharacter()->GetShip()->m_TurnLeft = false;
	}
}

void CommandMind::EnableTurnRight(void)
{
	if((GetCharacter() != 0) && (GetCharacter()->GetShip() != 0))
	{
		GetCharacter()->GetShip()->m_TurnRight = true;
	}
}

void CommandMind::DisableTurnRight(void)
{
	if((GetCharacter() != 0) && (GetCharacter()->GetShip() != 0))
	{
		GetCharacter()->GetShip()->m_TurnRight = false;
	}
}

void CommandMind::EnableFire(void)
{
	if((GetCharacter() != 0) && (GetCharacter()->GetShip() != 0))
	{
		GetCharacter()->GetShip()->m_Fire = true;
	}
}

void CommandMind::DisableFire(void)
{
	if((GetCharacter() != 0) && (GetCharacter()->GetShip() != 0))
	{
		GetCharacter()->GetShip()->m_Fire = false;
	}
}

void CommandMind::Jump(void)
{
	if((GetCharacter() != 0) && (GetCharacter()->GetShip() != 0))
	{
		GetCharacter()->GetShip()->m_Jump = true;
	}
}

void CommandMind::SelectNextLinkedSystem(void)
{
	if((GetCharacter() != 0) && (GetCharacter()->GetShip() != 0))
	{
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
}

void CommandMind::TargetPreviousCargo(void)
{
	if((GetCharacter() != 0) && (GetCharacter()->GetShip() != 0))
	{
		const std::list< Cargo * > & Cargos(GetCharacter()->GetShip()->GetCurrentSystem()->GetCargos());
		Cargo * SelectedCargo(dynamic_cast< Cargo * >(GetCharacter()->GetShip()->GetTarget()));
		
		if(SelectedCargo == 0)
		{
			if(Cargos.size() > 0)
			{
				GetCharacter()->GetShip()->SetTarget(Cargos.back());
			}
		}
		else
		{
			std::list< Cargo * >::const_iterator CargoIterator(find(Cargos.begin(), Cargos.end(), SelectedCargo));
			
			if(CargoIterator == Cargos.begin())
			{
				GetCharacter()->GetShip()->SetTarget(0);
			}
			else
			{
				--CargoIterator;
				GetCharacter()->GetShip()->SetTarget(*CargoIterator);
			}
		}
	}
}

void CommandMind::TargetNextCargo(void)
{
	if((GetCharacter() != 0) && (GetCharacter()->GetShip() != 0))
	{
		const std::list< Cargo * > & Cargos(GetCharacter()->GetShip()->GetCurrentSystem()->GetCargos());
		Cargo * SelectedCargo(dynamic_cast< Cargo * >(GetCharacter()->GetShip()->GetTarget()));
		
		if(SelectedCargo == 0)
		{
			if(Cargos.size() > 0)
			{
				GetCharacter()->GetShip()->SetTarget(Cargos.front());
			}
		}
		else
		{
			std::list< Cargo * >::const_iterator CargoIterator(find(Cargos.begin(), Cargos.end(), SelectedCargo));
			
			if(CargoIterator == Cargos.end())
			{
				GetCharacter()->GetShip()->SetTarget(0);
			}
			else
			{
				++CargoIterator;
				if(CargoIterator == Cargos.end())
				{
					GetCharacter()->GetShip()->SetTarget(0);
				}
				else
				{
					GetCharacter()->GetShip()->SetTarget(*CargoIterator);
				}
			}
		}
	}
}

void CommandMind::TargetPreviousPlanet(void)
{
	if((GetCharacter() != 0) && (GetCharacter()->GetShip() != 0))
	{
		const std::list< Planet * > & Planets(GetCharacter()->GetShip()->GetCurrentSystem()->GetPlanets());
		Planet * SelectedPlanet(dynamic_cast< Planet * >(GetCharacter()->GetShip()->GetTarget()));
		
		if(SelectedPlanet == 0)
		{
			if(Planets.size() > 0)
			{
				GetCharacter()->GetShip()->SetTarget(Planets.back());
			}
		}
		else
		{
			std::list< Planet * >::const_iterator PlanetIterator(find(Planets.begin(), Planets.end(), SelectedPlanet));
			
			if(PlanetIterator == Planets.begin())
			{
				GetCharacter()->GetShip()->SetTarget(0);
			}
			else
			{
				--PlanetIterator;
				GetCharacter()->GetShip()->SetTarget(*PlanetIterator);
			}
		}
	}
}

void CommandMind::TargetNextPlanet(void)
{
	if((GetCharacter() != 0) && (GetCharacter()->GetShip() != 0))
	{
		const std::list< Planet * > & Planets(GetCharacter()->GetShip()->GetCurrentSystem()->GetPlanets());
		Planet * SelectedPlanet(dynamic_cast< Planet * >(GetCharacter()->GetShip()->GetTarget()));
		
		if(SelectedPlanet == 0)
		{
			if(Planets.size() > 0)
			{
				GetCharacter()->GetShip()->SetTarget(Planets.front());
			}
		}
		else
		{
			std::list< Planet * >::const_iterator PlanetIterator(find(Planets.begin(), Planets.end(), SelectedPlanet));
			
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
					GetCharacter()->GetShip()->SetTarget(*PlanetIterator);
				}
			}
		}
	}
}

void CommandMind::TargetNextShip(void)
{
	if((GetCharacter() != 0) && (GetCharacter()->GetShip() != 0))
	{
		const std::list< Ship * > & Ships(GetCharacter()->GetShip()->GetCurrentSystem()->GetShips());
		Ship * SelectedShip(dynamic_cast< Ship * >(GetCharacter()->GetShip()->GetTarget()));
		
		if(SelectedShip == 0)
		{
			if(Ships.size() > 0)
			{
				GetCharacter()->GetShip()->SetTarget(Ships.front());
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
					GetCharacter()->GetShip()->SetTarget(*ShipIterator);
				}
			}
		}
	}
}

void CommandMind::TargetPreviousShip(void)
{
	if((GetCharacter() != 0) && (GetCharacter()->GetShip() != 0))
	{
		const std::list< Ship * > & Ships(GetCharacter()->GetShip()->GetCurrentSystem()->GetShips());
		Ship * SelectedShip(dynamic_cast< Ship * >(GetCharacter()->GetShip()->GetTarget()));
		
		if(SelectedShip == 0)
		{
			if(Ships.size() > 0)
			{
				GetCharacter()->GetShip()->SetTarget(Ships.back());
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
				GetCharacter()->GetShip()->SetTarget(*ShipIterator);
			}
		}
	}
}
