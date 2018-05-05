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

#include "properties.h"
#include "visualization_prototype.h"

Properties::Properties(void)
{
}

Properties::~Properties(void)
{
}

bool Properties::GetPropertyAsBoolean(const std::string & PropertyIdentifier) const
{
	return std::experimental::any_cast< bool >(_Properties.at(PropertyIdentifier));
}

float Properties::GetPropertyAsFloat(const std::string & PropertyIdentifier) const
{
	return std::experimental::any_cast< float >(_Properties.at(PropertyIdentifier));
}

const std::list< Properties > & Properties::GetPropertyAsList(const std::string & PropertyIdentifier) const
{
	return std::experimental::any_cast< const std::list< Properties > & >(_Properties.at(PropertyIdentifier));
}

const Quaternion & Properties::GetPropertyAsQuaternion(const std::string & PropertyIdentifier) const
{
	return std::experimental::any_cast< const Quaternion & >(_Properties.at(PropertyIdentifier));
}

const SlotClass * Properties::GetPropertyAsSlotClass(const std::string & PropertyIdentifier) const
{
	return std::experimental::any_cast< const SlotClass * >(_Properties.at(PropertyIdentifier));
}

const std::string & Properties::GetPropertyAsString(const std::string & PropertyIdentifier) const
{
	return std::experimental::any_cast< const std::string & >(_Properties.at(PropertyIdentifier));
}

std::uint32_t Properties::GetPropertyAsUnsignedInteger32Bit(const std::string & PropertyIdentifier) const
{
	return std::experimental::any_cast< std::uint32_t >(_Properties.at(PropertyIdentifier));
}

const Vector3f & Properties::GetPropertyAsVector3f(const std::string & PropertyIdentifier) const
{
	return std::experimental::any_cast< const Vector3f & >(_Properties.at(PropertyIdentifier));
}

const VisualizationPrototype & Properties::GetPropertyAsVisualizationPrototype(const std::string & PropertyIdentifier) const
{
	return std::experimental::any_cast< const VisualizationPrototype & >(_Properties.at(PropertyIdentifier));
}