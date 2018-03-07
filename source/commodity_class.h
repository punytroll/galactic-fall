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

class VisualizationPrototype;

class CommodityClass
{
public:
	CommodityClass(const std::string & Identifier);
	~CommodityClass(void);
	// getters
	const std::string & GetIdentifier(void) const;
	const std::string & GetName(void) const;
	std::uint32_t GetSpaceRequirement(void) const;
	const VisualizationPrototype * GetVisualizationPrototype(void) const;
	// setters
	void SetName(const std::string & Name);
	void SetSpaceRequirement(std::uint32_t SpaceRequirement);
	void SetVisualizationPrototype(const VisualizationPrototype & VisualizationPrototype);
private:
	std::string _Identifier;
	std::string _Name;
	std::uint32_t _SpaceRequirement;
	VisualizationPrototype * _VisualizationPrototype;
};

inline const std::string & CommodityClass::GetIdentifier(void) const
{
	return _Identifier;
}

inline const std::string & CommodityClass::GetName(void) const
{
	return _Name;
}

inline std::uint32_t CommodityClass::GetSpaceRequirement(void) const
{
	return _SpaceRequirement;
}

inline const VisualizationPrototype * CommodityClass::GetVisualizationPrototype(void) const
{
	return _VisualizationPrototype;
}

inline void CommodityClass::SetName(const std::string & Name)
{
	_Name = Name;
}

inline void CommodityClass::SetSpaceRequirement(std::uint32_t SpaceRequirement)
{
	_SpaceRequirement = SpaceRequirement;
}

#endif
