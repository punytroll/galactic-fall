/**
 * galactic-fall
 * Copyright (C) 2018-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <cassert>

#include "device.h"

Physics::Energy::Device::Device() :
    m_Energy{0.0f},
    m_EnergyCapacity{0.0f},
    m_MaximumPowerInput{0.0f},
    m_MaximumPowerOutput{0.0f},
    m_Network(nullptr)
{
}

Physics::Energy::Device::~Device()
{
    assert(m_Network == nullptr);
}

auto Physics::Energy::Device::EnergyDelta(float EnergyDelta) -> void
{
    m_Energy += EnergyDelta;
    assert((m_Energy >= 0.0f) && (m_Energy <= m_EnergyCapacity));
}

auto Physics::Energy::Device::GetMaximumEnergyInput(float Seconds) const -> float
{
    auto Result = Seconds * m_MaximumPowerInput;
    
    if(Result + m_Energy > m_EnergyCapacity)
    {
        Result = m_EnergyCapacity - m_Energy;
    }
    
    return Result;
}

auto Physics::Energy::Device::GetMaximumEnergyOutput(float Seconds) const -> float
{
    auto Result = Seconds * m_MaximumPowerOutput;
    
    if(Result > m_Energy)
    {
        Result = m_Energy;
    }
    
    return Result;
}

auto Physics::Energy::Device::SetEnergy(float Energy) -> void
{
    assert(Energy <= m_EnergyCapacity);
    m_Energy = Energy;
}

auto Physics::Energy::Device::SetEnergyCapacity(float EnergyCapacity) -> void
{
    assert(EnergyCapacity >= m_Energy);
    m_EnergyCapacity = EnergyCapacity;
}
