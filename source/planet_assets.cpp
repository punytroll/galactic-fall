/**
 * galactic-fall
 * Copyright (C) 2018  Hagen Möbius
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

#include "globals.h"
#include "object_factory.h"
#include "planet_assets.h"

PlanetAssets::PlanetAssets(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) :
	_BasePrice(g_ObjectFactory->GetBasePrice(TypeIdentifier, SubTypeIdentifier)),
	_BasePriceModifier(1.0f),
	_SubTypeIdentifier(SubTypeIdentifier),
	_TypeIdentifier(TypeIdentifier)
{
}

PlanetAssets::~PlanetAssets(void)
{
}

const std::string & PlanetAssets::GetDescription(void) const
{
	return g_ObjectFactory->GetDescription(_TypeIdentifier, _SubTypeIdentifier);
}

const std::string & PlanetAssets::GetName(void) const
{
	return g_ObjectFactory->GetName(_TypeIdentifier, _SubTypeIdentifier);
}

std::uint32_t PlanetAssets::GetPrice(void) const
{
	return static_cast< std::uint32_t >(_BasePrice * _BasePriceModifier);
}

std::uint32_t PlanetAssets::GetSpaceRequirement(void) const
{
	return g_ObjectFactory->GetSpaceRequirement(_TypeIdentifier, _SubTypeIdentifier);
}

const VisualizationPrototype * PlanetAssets::GetVisualizationPrototype(void) const
{
	return g_ObjectFactory->GetVisualizationPrototype(_TypeIdentifier, _SubTypeIdentifier);
}
