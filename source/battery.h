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
#include "physics/energy/device.h"

class Battery : public Object, public Physics::Energy::Device
{
public:
	Battery(void);
	virtual ~Battery(void) override;
	// Physics::Energy::Device implementation
	virtual float GetMaximumEnergyInput(float Seconds) const;
	virtual float GetMaximumEnergyOutput(float Seconds) const;
	virtual void EnergyDelta(float EnergyDelta);
	// getters
	float GetEnergy(void) const;
	float GetEnergyCapacity(void) const;
	float GetMaximumPowerInput(void) const;
	float GetMaximumPowerOutput(void) const;
	// setters
	void SetEnergy(float Energy);
	void SetEnergyCapacity(float EnergyCapacity);
	void SetMaximumPowerInput(float MaximumPowerInput);
	void SetMaximumPowerOutput(float MaximumPowerOutput);
private:
	// in Joule
	float _Energy;
	// in Joule
	float _EnergyCapacity;
	// in Watts
	float _MaximumPowerInput;
	// in Watts
	float _MaximumPowerOutput;
};

inline float Battery::GetEnergy(void) const
{
	return _Energy;
}

inline float Battery::GetEnergyCapacity(void) const
{
	return _EnergyCapacity;
}

inline float Battery::GetMaximumPowerInput(void) const
{
	return _MaximumPowerInput;
}

inline float Battery::GetMaximumPowerOutput(void) const
{
	return _MaximumPowerOutput;
}

inline void Battery::SetMaximumPowerInput(float MaximumPowerInput)
{
	_MaximumPowerInput = MaximumPowerInput;
}

inline void Battery::SetMaximumPowerOutput(float MaximumPowerOutput)
{
	_MaximumPowerOutput = MaximumPowerOutput;
}

#endif
