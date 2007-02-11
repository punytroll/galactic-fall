#ifndef COMMODITY_H
#define COMMODITY_H

#include <string>

class Color;

class Commodity
{
public:
	Commodity(const std::string & Identifier);
	~Commodity(void);
	const std::string & GetIdentifier(void) const;
	const std::string & GetName(void) const;
	float GetBasePrice(void) const;
	void SetName(const std::string & Name);
	void SetBasePrice(float BasePrice);
	const Color * GetColor(void) const;
	void SetColor(const Color & Color);
private:
	std::string m_Identifier;
	std::string m_Name;
	float m_BasePrice;
	Color * m_Color;
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

inline const Color * Commodity::GetColor(void) const
{
	return m_Color;
}

#endif
