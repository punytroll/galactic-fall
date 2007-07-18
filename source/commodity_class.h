/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
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

#ifndef COMMODITY_CLASS_H
#define COMMODITY_CLASS_H

#include <string>

#include "type_definitions.h"

class Color;
class Model;

class CommodityClass
{
public:
	CommodityClass(const std::string & Identifier);
	~CommodityClass(void);
	// getters
	const Color * GetColor(void) const;
	const std::string & GetIdentifier(void) const;
	const Model * GetModel(void) const;
	const std::string & GetName(void) const;
	unsigned_numeric GetSpaceRequirement(void) const;
	// setters
	void SetColor(const Color & Color);
	void SetModel(const Model * Model);
	void SetName(const std::string & Name);
	void SetSpaceRequirement(unsigned_numeric SpaceRequirement);
private:
	std::string m_Identifier;
	std::string m_Name;
	const Model * m_Model;
	Color * m_Color;
	unsigned_numeric m_SpaceRequirement;
};

inline const Color * CommodityClass::GetColor(void) const
{
	return m_Color;
}

inline const std::string & CommodityClass::GetIdentifier(void) const
{
	return m_Identifier;
}

inline const Model * CommodityClass::GetModel(void) const
{
	return m_Model;
}

inline const std::string & CommodityClass::GetName(void) const
{
	return m_Name;
}

inline unsigned_numeric CommodityClass::GetSpaceRequirement(void) const
{
	return m_SpaceRequirement;
}

inline void CommodityClass::SetModel(const Model * Model)
{
	m_Model = Model;
}

inline void CommodityClass::SetName(const std::string & Name)
{
	m_Name = Name;
}

inline void CommodityClass::SetSpaceRequirement(unsigned_numeric SpaceRequirement)
{
	m_SpaceRequirement = SpaceRequirement;
}

#endif
