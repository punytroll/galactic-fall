/**
 * galactic-fall
 * Copyright (C) 2018-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <cassert>
#include <cmath>
#include <unordered_map>

#include "device.h"
#include "network.h"

std::unordered_set<Physics::Energy::Network *> Physics::Energy::Network::m_Networks;

Physics::Energy::Network::Network()
{
    m_Networks.insert(this);
}

Physics::Energy::Network::~Network()
{
    assert(m_Devices.size() == 0);
    m_Networks.erase(this);
}

auto Physics::Energy::Network::AddDevice(Physics::Energy::Device & Device) -> void
{
    assert(Device.m_Network == nullptr);
    assert(m_Devices.find(std::addressof(Device)) == m_Devices.end());
    Device.m_Network = this;
    m_Devices.insert(std::addressof(Device));
}

auto Physics::Energy::Network::RemoveDevice(Physics::Energy::Device & Device) -> void
{
    assert(Device.m_Network == this);
    assert(m_Devices.find(std::addressof(Device)) != m_Devices.end());
    Device.m_Network = nullptr;
    m_Devices.erase(std::addressof(Device));
}

auto Physics::Energy::Network::Update(float Seconds) -> void
{
    auto Devices = std::unordered_map<Physics::Energy::Device *, std::pair<float, float>>{};
    auto TotalMaximumEnergyInput = 0.0f;
    auto TotalMaximumEnergyOutput = 0.0f;
    auto TotalMaximumEnergyStorageInput = 0.0f;
    auto TotalMaximumEnergyStorageOutput = 0.0f;
    
    for(auto Device : m_Devices)
    {
        auto MaximumEnergyInput = Device->GetMaximumEnergyInput(Seconds);
        
        assert(MaximumEnergyInput >= 0.0f);
        
        auto MaximumEnergyOutput = Device->GetMaximumEnergyOutput(Seconds);
        
        assert(MaximumEnergyOutput >= 0.0f);
        
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
            auto ProductionRatio = (TotalMaximumEnergyInput + TotalMaximumEnergyStorageInput) / TotalMaximumEnergyOutput;
            
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
            auto StorageRatio = (TotalMaximumEnergyOutput - TotalMaximumEnergyInput) / TotalMaximumEnergyStorageInput;
            
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
            auto StorageRatio = (TotalMaximumEnergyInput - TotalMaximumEnergyOutput) / TotalMaximumEnergyStorageOutput;
            
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
            auto ConsumerRatio = (TotalMaximumEnergyOutput + TotalMaximumEnergyStorageOutput) / TotalMaximumEnergyInput;
            
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

auto Physics::Energy::Network::UpdateNetworks(float Seconds) -> void
{
    for(auto Network : m_Networks)
    {
        Network->Update(Seconds);
    }
}
