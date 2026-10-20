/**
 * galactic-fall
 * Copyright (C) 2018-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__PHYSICS__ENERGY__NETWORK_H
#define GALACTIC_FALL__SOURCE__PHYSICS__ENERGY__NETWORK_H

#include <unordered_set>

namespace Physics
{
    namespace Energy
    {
        class Device;
        
        class Network
        {
        public:
            /**
             * @brief The constructor places the new network into the static (global) networks set.
             **/
            Network();
            
            /**
             * @brief The destructor removed the network from the static (global) networks set.
             **/
            ~Network();
            
            /**
             * @brief Adds a new energy device to the network.
             **/
            auto AddDevice(Physics::Energy::Device & Device) -> void;
            
            /**
             * @brief Removes an energy device from the network.
             **/
            auto RemoveDevice(Physics::Energy::Device & Device) -> void;
            
            /**
             * @brief Advances the energy network by a given amount of time.
             **/
            auto Update(float Seconds) -> void;
            
            /**
             * @brief Advance all energy networks by a given amount of time.
             **/
            static auto UpdateNetworks(float Seconds) -> void;
            
        private:
            /**
             * @brief A set of all the devices in the energy network.
             **/
            std::unordered_set<Physics::Energy::Device *> m_Devices;
            
            /**
             * @brief A set of all the energy networks.
             **/
            static std::unordered_set<Physics::Energy::Network *> m_Networks;
        };
    }
}

#endif
