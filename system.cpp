#include "planet.h"
#include "system.h"

System::System(const std::string & Identifier) :
	m_Identifier(Identifier)
{
}

void System::SetName(const std::string & Name)
{
	m_Name = Name;
}

void System::SetPosition(const math3d::vector2f & Position)
{
	m_Position = Position;
}

bool System::IsLinkedToSystem(const System * LinkedSystem) const
{
	for(std::list< System * >::const_iterator LinkedSystemIterator = m_LinkedSystems.begin(); LinkedSystemIterator != m_LinkedSystems.end(); ++LinkedSystemIterator)
	{
		if(*LinkedSystemIterator == LinkedSystem)
		{
			return true;
		}
	}
	
	return false;
}

Planet * System::CreatePlanet(const std::string & Identifier)
{
	m_Planets.push_back(new Planet(Identifier));
	
	return m_Planets.back();
}

void System::AddLinkedSystem(System * LinkedSystem)
{
	m_LinkedSystems.push_back(LinkedSystem);
}

void System::AddShip(Ship * Ship)
{
	m_Ships.push_back(Ship);
}

void System::AddCargo(Cargo * Cargo)
{
	m_Cargos.push_back(Cargo);
}

void System::ClearShips(void)
{
	m_Ships.clear();
}

void System::ClearCargos(void)
{
	m_Cargos.clear();
}

void System::RemoveCargo(Cargo * RemoveCargo)
{
	std::list< Cargo * >::iterator CargoIterator(find(m_Cargos.begin(), m_Cargos.end(), RemoveCargo));
	
	if(CargoIterator != m_Cargos.end())
	{
		m_Cargos.erase(CargoIterator);
	}
}
