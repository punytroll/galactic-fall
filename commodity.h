#ifndef COMMODITY_H
#define COMMODITY_H

#include <string>

class Commodity
{
public:
	Commodity(const std::string & Identifier);
	const std::string & GetIdentifier(void) const;
	const std::string & GetName(void) const;
	float GetBasePrice(void) const;
	void SetName(const std::string & Name);
	void SetBasePrice(float BasePrice);
private:
	std::string m_Identifier;
	std::string m_Name;
	float m_BasePrice;
};

inline const std::string & Commodity::GetIdentifier(void) const
{
	return m_Identifier;
}

inline const std::string & Commodity::GetName(void) const
{
	return m_Name;
}

inline float Commodity::GetBasePrice(void) const
{
	return m_BasePrice;
}

#endif
