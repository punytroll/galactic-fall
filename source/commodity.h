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

#ifndef COMMODITY_H
#define COMMODITY_H

#include <string>

class Color;
class Model;

class Commodity
{
public:
	Commodity(const std::string & Identifier);
	~Commodity(void);
	// getters
	float GetBasePrice(void) const;
	const Color * GetColor(void) const;
	const std::string & GetIdentifier(void) const;
	const Model * GetModel(void) const;
	const std::string & GetName(void) const;
	// setters
	void SetBasePrice(float BasePrice);
	void SetColor(const Color & Color);
	void SetModel(const Model * Model);
	void SetName(const std::string & Name);
private:
	float m_BasePrice;
	Color * m_Color;
	std::string m_Identifier;
	std::string m_Name;
	const Model * m_Model;
};

inline float Commodity::GetBasePrice(void) const
{
	return m_BasePrice;
}

inline const Color * Commodity::GetColor(void) const
{
	return m_Color;
}

inline const std::string & Commodity::GetIdentifier(void) const
{
	return m_Identifier;
}

inline const Model * Commodity::GetModel(void) const
{
	return m_Model;
}

inline const std::string & Commodity::GetName(void) const
{
	return m_Name;
}

inline void Commodity::SetBasePrice(float BasePrice)
{
	m_BasePrice = BasePrice;
}

inline void Commodity::SetModel(const Model * Model)
{
	m_Model = Model;
}

inline void Commodity::SetName(const std::string & Name)
{
	m_Name = Name;
}

#endif
