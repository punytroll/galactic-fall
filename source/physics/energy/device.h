/**
 * galactic-fall
 * Copyright (C) 2018-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__PHYSICS__ENERGY__DEVICE_H
#define GALACTIC_FALL__SOURCE__PHYSICS__ENERGY__DEVICE_H

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
            Device();
            virtual ~Device();
            
            /**
             * @brief The maximum amout of energy that can flow into this device in the next given amount of time.
             * @note Returns in Joule.
             * 
             * This function is called by the energy network to calculate the potential input of this device.
             * By default, it returns the maximum power input scaled by the time, but limited, so that there is no overflow of the energy capacity, if the input is fully satisfied.
             * You can override this function for more advanced calculations.
             **/
            virtual auto GetMaximumEnergyInput(float Seconds) const -> float;
            
            /**
             * @brief The maximum amout of energy that can flow out of this device in the next given amount of time.
             * @note Returns in Joule.
             * 
             * This function is called by the energy network to calculate the potential output of this device.
             * By default, it returns the maximum power output scaled by the time, but limited, so that there is no underflow of the energy, if the output is fully extracted.
             * You can override this function for more advanced calculations.
             **/
            virtual auto GetMaximumEnergyOutput(float Seconds) const -> float;
            
            /**
             * @brief After calculating all the inputs and outputs of devices, the network uses this function to manipulate the energy stored in this device.
             * @note in Joule
             * 
             * You can override this function for custom functionality.
             **/
            virtual auto EnergyDelta(float EnergyDelta) -> void;
            
            /**
             * @brief Get the energy level of this device.
             * @note in Joule
             **/
            auto GetEnergy() const -> float;
            
            /**
             * @brief Get the energy capacity of this device.
             * @note in Joule
             **/
            auto GetEnergyCapacity() const -> float;
            
            /**
             * @brief Get the maximum power input of the device.
             * @note in Watts
             **/
            auto GetMaximumPowerInput() const -> float;
            
            /**
             * @brief Get the maximum power output of the device.
             * @note in Watts
             **/
            auto GetMaximumPowerOutput() const -> float;
            
            /**
             * @brief Set the energy level of this device.
             * @note in Joule
             * 
             * This directly sets the energy level, without adhering to any simulation results, for example when deserialising the device's state.
             **/
            auto SetEnergy(float Energy) -> void;
            
            /**
             * @brief Set the energy capacity of this device.
             * @note in Joule
             **/
            auto SetEnergyCapacity(float EnergyCapacity) -> void;
            
            /**
             * @brief Set the maximum power input of the device.
             * @note in Watts
             **/
            auto SetMaximumPowerInput(float MaximumPowerInput) -> void;
            
            /**
             * @brief Set the maximum power output of the device.
             * @note in Watts
             **/
            auto SetMaximumPowerOutput(float MaximumPowerOutput) -> void;
            
        private:
            /**
             * @brief The energy stored in this device.
             * @note in Joule
             **/
            float m_Energy;
            
            /**
             * @brief The maximum amout of energy that can be stored in this device.
             * @note in Joule
             **/
            float m_EnergyCapacity;
            
            /**
             * @brief The maximum amount of energy flow into this device.
             * @note in Watts
             **/
            float m_MaximumPowerInput;
            
            /**
             * @brief The maximum amout of energy flow out of this device.
             * @note in Watts
             **/
            float m_MaximumPowerOutput;
            
            /**
             * @brief The energy network that this device belongs to.
             **/
            Physics::Energy::Network * m_Network;
        };
    }
}

inline auto Physics::Energy::Device::GetEnergy() const -> float
{
    return m_Energy;
}

inline auto Physics::Energy::Device::GetEnergyCapacity() const -> float
{
    return m_EnergyCapacity;
}

inline auto Physics::Energy::Device::GetMaximumPowerInput() const -> float
{
    return m_MaximumPowerInput;
}

inline auto Physics::Energy::Device::GetMaximumPowerOutput() const -> float
{
    return m_MaximumPowerOutput;
}

inline auto Physics::Energy::Device::SetMaximumPowerInput(float MaximumPowerInput) -> void
{
    m_MaximumPowerInput = MaximumPowerInput;
}

inline auto Physics::Energy::Device::SetMaximumPowerOutput(float MaximumPowerOutput) -> void
{
    m_MaximumPowerOutput = MaximumPowerOutput;
}

#endif
