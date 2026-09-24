/**
 * galactic-fall
 * Copyright (C) 2018-2026  Hagen Möbius
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

#ifndef BLUEPRINT_H
#define BLUEPRINT_H

#include <string>

#include "properties.h"

class Blueprint : public Properties
{
public:
	Blueprint(std::string TypeIdentifier, std::string SubTypeIdentifier);
	auto GetSubTypeIdentifier() const -> std::string const &;
	auto GetTypeIdentifier() const -> std::string const &;
    
private:
	std::string m_SubTypeIdentifier;
	std::string m_TypeIdentifier;
};

inline auto Blueprint::GetSubTypeIdentifier() const -> std::string const &
{
	return m_SubTypeIdentifier;
}

inline auto Blueprint::GetTypeIdentifier() const -> std::string const &
{
	return m_TypeIdentifier;
}

#endif
