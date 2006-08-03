#ifndef SYSTEM_H
#define SYSTEM_H

#include <list>
#include <string>

#include <math3d/vector2f.h>

class Planet;
class Ship;

class System
{
public:
	System(const std::string & Identifier);
	const std::string & GetIdentifier(void) const;
	const std::string & GetName(void) const;
	const math3d::vector2f & GetPosition(void) const;
	const std::list< Planet * > GetPlanets(void) const;
	const std::list< System * > GetLinkedSystems(void) const;
	const std::list< Ship * > GetShips(void) const;
	bool IsLinkedToSystem(const System * System) const;
	void SetName(const std::string & Name);
	void SetPosition(const math3d::vector2f & Position);
	Planet * CreatePlanet(const std::string & Identifier);
	void AddLinkedSystem(System * LinkedSystem);
	void AddShip(Ship * Ship);
	void ClearShips(void);
private:
	std::string m_Identifier;
	std::string m_Name;
	math3d::vector2f m_Position;
	std::list< Planet * > m_Planets;
	std::list< System * > m_LinkedSystems;
	std::list< Ship * > m_Ships;
};

inline const std::string & System::GetIdentifier(void) const
{
	return m_Identifier;
}

inline const std::string & System::GetName(void) const
{
	return m_Name;
}

inline const math3d::vector2f & System::GetPosition(void) const
{
	return m_Position;
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

#endif
