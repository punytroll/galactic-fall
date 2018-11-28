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

#include <cassert>
#include <cmath>
#include <unordered_map>

#include "device.h"
#include "network.h"

std::unordered_set< Physics::Energy::Network * > Physics::Energy::Network::_Networks;

Physics::Energy::Network::Network(void)
{
	_Networks.insert(this);
}

Physics::Energy::Network::~Network(void)
{
	assert(_Devices.size() == 0);
	_Networks.erase(this);
}

void Physics::Energy::Network::AddDevice(Physics::Energy::Device * Device)
{
	assert(Device->_Network == nullptr);
	assert(_Devices.find(Device) == _Devices.end());
	Device->_Network = this;
	_Devices.insert(Device);
}

void Physics::Energy::Network::RemoveDevice(Physics::Energy::Device * Device)
{
	assert(Device->_Network == this);
	assert(_Devices.find(Device) != _Devices.end());
	Device->_Network = nullptr;
	_Devices.erase(Device);
}

void Physics::Energy::Network::Update(float Seconds)
{
	std::unordered_map< Physics::Energy::Device *, std::pair< float, float > > Devices;
	auto TotalMaximumEnergyInput{0.0f};
	auto TotalMaximumEnergyOutput{0.0f};
	auto TotalMaximumEnergyStorageInput{0.0f};
	auto TotalMaximumEnergyStorageOutput{0.0f};
	
	for(auto Device : _Devices)
	{
		auto MaximumEnergyInput{Device->GetMaximumEnergyInput(Seconds)};
		auto MaximumEnergyOutput{Device->GetMaximumEnergyOutput(Seconds)};
		
		Devices.emplace(Device, std::make_pair(MaximumEnergyInput, MaximumEnergyOutput));
		if((MaximumEnergyInput > 0.0f) && (MaximumEnergyOutput > 0.0f))
		{
			TotalMaximumEnergyStorageInput += MaximumEnergyInput;
			TotalMaximumEnergyStorageOutput += MaximumEnergyOutput;
		}
		else
		{
			TotalMaximumEnergyInput += MaximumEnergyInput;
			TotalMaximumEnergyOutput += MaximumEnergyOutput;
		}
	}
	if(TotalMaximumEnergyOutput >= TotalMaximumEnergyInput)
	{
		// we have enough energy production to satisfy all consumers
		if(TotalMaximumEnergyOutput >= TotalMaximumEnergyInput + TotalMaximumEnergyStorageInput)
		{
			// we also have enough energy production for maximum flow to the storages
			// the energy production can be reduced by the overhang
			auto ProductionRatio{(TotalMaximumEnergyInput + TotalMaximumEnergyStorageInput) / TotalMaximumEnergyOutput};
			
			for(auto Device : Devices)
			{
				if(Device.second.first > 0.0f)
				{
					// storages and consumers
					Device.first->EnergyDelta(Device.second.first);
				}
				else if(Device.second.second > 0.0f)
				{
					// producers
					assert(std::isnan(ProductionRatio) == false);
					Device.first->EnergyDelta(-(Device.second.second * ProductionRatio));
				}
			}
		}
		else
		{
			// we don't have enough energy to satisfy storage input
			// storages are filled proportional to their maximum energy input
			auto StorageRatio{(TotalMaximumEnergyOutput - TotalMaximumEnergyInput) / TotalMaximumEnergyStorageInput};
			
			for(auto Device : Devices)
			{
				if(Device.second.first > 0.0f)
				{
					if(Device.second.second > 0.0f)
					{
						// storages
						Device.first->EnergyDelta(Device.second.first * StorageRatio);
					}
					else
					{
						// consumers
						Device.first->EnergyDelta(Device.second.first);
					}
				}
				else if(Device.second.second > 0.0f)
				{
					// producers
					Device.first->EnergyDelta(-Device.second.second);
				}
			}
		}
	}
	else
	{
		// we don't have enough energy production to satisfy all consumers
		if(TotalMaximumEnergyOutput + TotalMaximumEnergyStorageOutput >= TotalMaximumEnergyInput)
		{
			// with energy from the generators and storages, we can satisfy all consumers
			// we draw from the storages proportional to their maximum energy output
			auto StorageRatio{(TotalMaximumEnergyInput - TotalMaximumEnergyOutput) / TotalMaximumEnergyStorageOutput};
			
			for(auto Device : Devices)
			{
				if(Device.second.first > 0.0f)
				{
					if(Device.second.second > 0.0f)
					{
						// storages
						Device.first->EnergyDelta(-(Device.second.second * StorageRatio));
					}
					else
					{
						// consumers
						Device.first->EnergyDelta(Device.second.first);
					}
				}
				else if(Device.second.second > 0.0f)
				{
					// producers
					Device.first->EnergyDelta(-Device.second.second);
				}
			}
		}
		else
		{
			// even when drawing full output from the storages we cannot satisfy the consumers
			// we feed to the consumers proportional to their maximum energy input
			auto ConsumerRatio{(TotalMaximumEnergyOutput + TotalMaximumEnergyStorageOutput) / TotalMaximumEnergyInput};
			
			for(auto Device : Devices)
			{
				if(Device.second.first > 0.0f)
				{
					if(Device.second.second > 0.0f)
					{
						// storages
						Device.first->EnergyDelta(-Device.second.second);
					}
					else
					{
						// consumers
						Device.first->EnergyDelta(Device.second.first * ConsumerRatio);
					}
				}
				else
				{
					// producers
					Device.first->EnergyDelta(-Device.second.second);
				}
			}
		}
	}
}

void Physics::Energy::Network::UpdateNetworks(float Seconds)
{
	for(auto Network : _Networks)
	{
		Network->Update(Seconds);
	}
}
