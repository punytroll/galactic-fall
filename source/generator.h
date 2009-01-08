/**
 * galactic-fall
 * Copyright (C) 2009  Hagen Möbius
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

#ifndef GENERATOR_H
#define GENERATOR_H

#include "object.h"

class Generator : public Object
{
public:
	Generator(void);
	// getters
	float GetEnergyProvisionPerSecond(void) const;
	// setters
	void SetEnergyProvisionPerSecond(float EnergyProvisionPerSecond);
private:
	float m_EnergyProvisionPerSecond;
};

inline float Generator::GetEnergyProvisionPerSecond(void) const
{
	return m_EnergyProvisionPerSecond;
}

inline void Generator::SetEnergyProvisionPerSecond(float EnergyProvisionPerSecond)
{
	m_EnergyProvisionPerSecond = EnergyProvisionPerSecond;
}

#endif
