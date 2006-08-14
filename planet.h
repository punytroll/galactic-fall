#ifndef PLANET_H
#define PLANET_H

#include <string>
#include <vector>

#include "object.h"

class Commodity;

class PlanetCommodity
{
public:
	PlanetCommodity(Commodity * Commodity);
	const Commodity * GetCommodity(void) const;
	float GetPrice(void) const;
	void SetBasePriceModifier(const float & BasePriceModifier);
private:
	Commodity * m_Commodity;
	float m_BasePriceModifier;
};

class Planet : public Object
{
public:
	Planet(const std::string & Identifier);
	Planet::~Planet(void);
	const std::string & GetIdentifier(void) const;
	const std::string & GetName(void) const;
	const std::string & GetDescription(void) const;
	float GetSize(void) const;
	const std::vector< PlanetCommodity * > & GetCommodities(void) const;
	void SetName(const std::string & Name);
	void SetDescription(const std::string & Description);
	void SetSize(const float & Size);
	virtual void Draw(void) const;
	PlanetCommodity * CreateCommodity(Commodity * Commodity);
private:
	std::string m_Identifier;
	std::string m_Name;
	std::string m_Description;
	float m_Size;
	std::vector< PlanetCommodity * > m_Commodities;
};

inline const Commodity * PlanetCommodity::GetCommodity(void) const
{
	return m_Commodity;
}

inline const std::string & Planet::GetIdentifier(void) const
{
	return m_Identifier;
}

inline const std::string & Planet::GetName(void) const
{
	return m_Name;
}

inline const std::string & Planet::GetDescription(void) const
{
	return m_Description;
}

inline float Planet::GetSize(void) const
{
	return m_Size;
}

inline const std::vector< PlanetCommodity * > & Planet::GetCommodities(void) const
{
	return m_Commodities;
}

#endif
