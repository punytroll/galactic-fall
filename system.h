#ifndef SYSTEM_H
#define SYSTEM_H

#include <list>
#include <string>

#include <math3d/vector2f.h>

#include "position.h"

class Cargo;
class Planet;
class Ship;
class Shot;
class Star;

class System : public Position
{
public:
	System(const std::string & Identifier);
	const std::string & GetIdentifier(void) const;
	const std::string & GetName(void) const;
	const std::list< Planet * > GetPlanets(void) const;
	const std::list< System * > GetLinkedSystems(void) const;
	const std::list< Ship * > GetShips(void) const;
	const std::list< Cargo * > GetCargos(void) const;
	std::list< Shot * > & GetShots(void);
	const Star * GetStar(void) const;
	bool IsLinkedToSystem(const System * System) const;
	void SetName(const std::string & Name);
	Planet * CreatePlanet(const std::string & Identifier);
	Star * CreateStar(void);
	void AddLinkedSystem(System * LinkedSystem);
	void AddShip(Ship * Ship);
	void AddCargo(Cargo * Cargo);
	void AddShot(Shot * Shot);
	void ClearShips(void);
	void ClearCargos(void);
	void ClearShots(void);
	void RemoveCargo(Cargo * RemoveCargo);
private:
	std::string m_Identifier;
	std::string m_Name;
	Star * m_Star;
	std::list< Planet * > m_Planets;
	std::list< System * > m_LinkedSystems;
	std::list< Ship * > m_Ships;
	std::list< Cargo * > m_Cargos;
	std::list< Shot * > m_Shots;
};

inline const std::string & System::GetIdentifier(void) const
{
	return m_Identifier;
}

inline const std::string & System::GetName(void) const
{
	return m_Name;
}

inline const std::list< Planet * > System::GetPlanets(void) const
{
	return m_Planets;
}

inline const std::list< System * > System::GetLinkedSystems(void) const
{
	return m_LinkedSystems;
}

inline const std::list< Ship * > System::GetShips(void) const
{
	return m_Ships;
}

inline const std::list< Cargo * > System::GetCargos(void) const
{
	return m_Cargos;
}

inline std::list< Shot * > & System::GetShots(void)
{
	return m_Shots;
}

inline const Star * System::GetStar(void) const
{
	return m_Star;
}

#endif
