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

#include "battery.h"

Battery::Battery(void) :
	_Energy{0.0f},
	_EnergyCapacity{0.0f},
	_MaximumPowerInput{0.0f},
	_MaximumPowerOutput{0.0f}
{
	// initialize object aspects
	AddAspectAccessory();
	AddAspectName();
	AddAspectPhysical();
}

Battery::~Battery(void)
{
}

float Battery::GetMaximumEnergyInput(float Seconds) const
{
	auto Result{Seconds * _MaximumPowerInput};
	
	if(Result + _Energy > _EnergyCapacity)
	{
		Result = _EnergyCapacity - _Energy;
	}
	
	return Result;
}

float Battery::GetMaximumEnergyOutput(float Seconds) const
{
	auto Result{Seconds * _MaximumPowerOutput};
	
	if(Result > _Energy)
	{
		Result = _Energy;
	}
	
	return Result;
}

void Battery::EnergyDelta(float EnergyDelta)
{
	_Energy += EnergyDelta;
	assert((_Energy >= 0.0f) && (_Energy <= _EnergyCapacity));
}

void Battery::SetEnergy(float Energy)
{
	assert(Energy <= _EnergyCapacity);
	_Energy = Energy;
}

void Battery::SetEnergyCapacity(float EnergyCapacity)
{
	assert(EnergyCapacity >= _Energy);
	_EnergyCapacity = EnergyCapacity;
}
