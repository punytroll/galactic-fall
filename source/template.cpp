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

#include "template.h"
#include "visualization_prototype.h"

Template::Template(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) :
	_SubTypeIdentifier(SubTypeIdentifier),
	_TypeIdentifier(TypeIdentifier)
{
}

Template::~Template(void)
{
}

const std::experimental::any & Template::GetField(const std::string & FieldIdentifier) const
{
	return _Fields.at(FieldIdentifier);
}

float Template::GetFieldAsFloat(const std::string & FieldIdentifier) const
{
	return std::experimental::any_cast< float >(_Fields.at(FieldIdentifier));
}

const std::string & Template::GetFieldAsString(const std::string & FieldIdentifier) const
{
	return std::experimental::any_cast< const std::string & >(_Fields.at(FieldIdentifier));
}

std::uint32_t Template::GetFieldAsUnsignedInteger32Bit(const std::string & FieldIdentifier) const
{
	return std::experimental::any_cast< std::uint32_t >(_Fields.at(FieldIdentifier));
}

const Vector3f & Template::GetFieldAsVector3f(const std::string & FieldIdentifier) const
{
	return std::experimental::any_cast< const Vector3f & >(_Fields.at(FieldIdentifier));
}

const VisualizationPrototype & Template::GetFieldAsVisualizationPrototype(const std::string & FieldIdentifier) const
{
	return std::experimental::any_cast< const VisualizationPrototype & >(_Fields.at(FieldIdentifier));
}
