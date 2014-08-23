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

#ifndef BATTERY_H
#define BATTERY_H

#include "object.h"

class Battery : public Object
{
public:
	Battery(void);
	virtual ~Battery(void) override;
	// getters
	float GetEnergy(void) const;
	float GetEnergyCapacity(void) const;
	// setters
	void SetEnergy(float Energy);
	void SetEnergyCapacity(float EnergyCapacity);
private:
	float _Energy;
	float _EnergyCapacity;
};

inline float Battery::GetEnergy(void) const
{
	return _Energy;
}

inline float Battery::GetEnergyCapacity(void) const
{
	return _EnergyCapacity;
}

inline void Battery::SetEnergy(float Energy)
{
	_Energy = Energy;
}

inline void Battery::SetEnergyCapacity(float EnergyCapacity)
{
	_EnergyCapacity = EnergyCapacity;
}

#endif
