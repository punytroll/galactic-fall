/**
 * galactic-fall
 * Copyright (C) 2018-2019  Hagen Möbius
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

#ifndef PHYSICS_ENERGY_DEVICE_H
#define PHYSICS_ENERGY_DEVICE_H

namespace Physics
{
	namespace Energy
	{
		class Network;
		
		class Device
		{
		public:
			friend class Network;
		public:
			Device(void);
			virtual ~Device(void);
			// dynamic properties for second to second simulation
			// getters
			// in Joule
			virtual float GetMaximumEnergyInput(float Seconds) const;
			// in Joule
			virtual float GetMaximumEnergyOutput(float Seconds) const;
			// setters
			// in Joule
			virtual void EnergyDelta(float EnergyDelta);
			// static properties as basis for and result of simulation
			// getters
			// in Joule
			float GetEnergy(void) const;
			// in Joule
			float GetEnergyCapacity(void) const;
			// in Watts
			float GetMaximumPowerInput(void) const;
			// in Watts
			float GetMaximumPowerOutput(void) const;
			// setters
			// in Joule
			void SetEnergy(float Energy);
			// in Joule
			void SetEnergyCapacity(float EnergyCapacity);
			// in Watts
			void SetMaximumPowerInput(float MaximumPowerInput);
			// in Watts
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
			Physics::Energy::Network * _Network;
		};
	}
}

inline float Physics::Energy::Device::GetEnergy(void) const
{
	return _Energy;
}

inline float Physics::Energy::Device::GetEnergyCapacity(void) const
{
	return _EnergyCapacity;
}

inline float Physics::Energy::Device::GetMaximumPowerInput(void) const
{
	return _MaximumPowerInput;
}

inline float Physics::Energy::Device::GetMaximumPowerOutput(void) const
{
	return _MaximumPowerOutput;
}

inline void Physics::Energy::Device::SetMaximumPowerInput(float MaximumPowerInput)
{
	_MaximumPowerInput = MaximumPowerInput;
}

inline void Physics::Energy::Device::SetMaximumPowerOutput(float MaximumPowerOutput)
{
	_MaximumPowerOutput = MaximumPowerOutput;
}

#endif
