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
			// in Joule
			virtual float GetMaximumEnergyInput(float Seconds) const = 0;
			// in Joule
			virtual float GetMaximumEnergyOutput(float Seconds) const = 0;
			// in Joule
			virtual void EnergyDelta(float EnergyDelta) = 0;
		private:
			Physics::Energy::Network * _Network;
		};
	}
}

#endif
