/**
 * galactic-fall
 * Copyright (C) 2006-2018  Hagen Möbius
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

#include <algorithm>

#include "battery.h"
#include "character.h"
#include "faction.h"
#include "globals.h"
#include "graphics/node.h"
#include "hangar.h"
#include "message.h"
#include "message_dispatcher.h"
#include "object_aspect_object_container.h"
#include "object_aspect_physical.h"
#include "object_aspect_position.h"
#include "object_aspect_visualization.h"
#include "object_factory.h"
#include "planet.h"
#include "planet_assets.h"
#include "ship.h"
#include "visualization.h"

Planet::Planet(void) :
	_Faction(nullptr),
	_LandingFeePerSpace(0.0f),
	_OffersRecharging(false),
	_OffersRepairing(false),
	_RechargingFeePerEnergy(0.0f),
	_RepairingFeePerHull(0.0f)
{
	// initialize object aspects
	AddAspectName();
	AddAspectObjectContainer();
	AddAspectPhysical();
	AddAspectPosition();
	AddAspectVisualization();
	GetAspectVisualization()->SetUpdateVisualizationCallback(std::bind(&Planet::_UpdateVisualization, this, std::placeholders::_1));
}

Planet::~Planet(void)
{
	while(_PlanetAssets.size() > 0)
	{
		delete _PlanetAssets.back();
		_PlanetAssets.pop_back();
	}
	if(_Faction != nullptr)
	{
		assert(_FactionDestroyingConnection.IsValid() == true);
		_FactionDestroyingConnection.Disconnect();
		_Faction = nullptr;
	}
	assert(_FactionDestroyingConnection.IsValid() == false);
}

Hangar * Planet::GetHangar(Character * Character)
{
	Hangar * Result(nullptr);
	
	assert(Character != nullptr);
	assert(GetAspectObjectContainer() != nullptr);
	for(auto Content : GetAspectObjectContainer()->GetContent())
	{
		if(Content->GetTypeIdentifier() == "hangar")
		{
			auto AHangar(dynamic_cast< Hangar * >(Content));
			
			assert(AHangar != nullptr);
			if(AHangar->GetCharacter() == Character)
			{
				Result = AHangar;
				
				break;
			}
		}
	}
	
	return Result;
}

PlanetAssets * Planet::GetPlanetAssets(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier) const
{
	auto PlanetAssetsIterator{std::find_if(_PlanetAssets.begin(), _PlanetAssets.end(), [&TypeIdentifier, &SubTypeIdentifier](PlanetAssets * PlanetAssets) -> auto { return (PlanetAssets->GetTypeIdentifier() == TypeIdentifier) && (PlanetAssets->GetSubTypeIdentifier() == SubTypeIdentifier); })};
	
	if(PlanetAssetsIterator != _PlanetAssets.end())
	{
		return *PlanetAssetsIterator;
	}
	else
	{
		return nullptr;
	}
}

void Planet::SetDescription(const std::string & Description)
{
	_Description = Description;
}

void Planet::SetFaction(Faction * Faction)
{
	if(_Faction != nullptr)
	{
		assert(_FactionDestroyingConnection.IsValid() == true);
		_FactionDestroyingConnection.Disconnect();
		assert(_FactionDestroyingConnection.IsValid() == false);
	}
	_Faction = Faction;
	if(_Faction != nullptr)
	{
		assert(_FactionDestroyingConnection.IsValid() == false);
		_FactionDestroyingConnection = _Faction->ConnectDestroyingCallback(std::bind(&Planet::_OnFactionDestroying, this));
		assert(_FactionDestroyingConnection.IsValid() == true);
	}
}

void Planet::SetSize(const float & Size)
{
	_Size = Size;
	assert(GetAspectPhysical() != nullptr);
	GetAspectPhysical()->SetRadialSize(_Size / 2.0f);
}

PlanetAssets * Planet::CreatePlanetAssets(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier)
{
	assert(GetPlanetAssets(TypeIdentifier, SubTypeIdentifier) == nullptr);
	_PlanetAssets.push_back(new PlanetAssets(TypeIdentifier, SubTypeIdentifier));
	
	return _PlanetAssets.back();
}

void Planet::Land(Ship * Ship, Character * Character)
{
	assert(Character != nullptr);
	assert(Ship != nullptr);
	Ship->SetVelocity(Vector3f::CreateZero());
	assert(Ship->GetContainer() != nullptr);
	assert(Ship->GetContainer()->GetAspectObjectContainer() != nullptr);
	Ship->GetContainer()->GetAspectObjectContainer()->RemoveContent(Ship);
	
	auto Hangar(GetHangar(Character));
	
	if(Hangar == nullptr)
	{
		Hangar = _CreateHangar(Character);
		assert(Hangar != nullptr);
		assert(GetAspectObjectContainer() != nullptr);
		GetAspectObjectContainer()->AddContent(Hangar);
	}
	assert(Hangar->GetAspectObjectContainer() != nullptr);
	Hangar->GetAspectObjectContainer()->AddContent(Ship);
	assert(Ship->GetAspectObjectContainer() != nullptr);
	for(auto Content : Ship->GetAspectObjectContainer()->GetContent())
	{
		if(Content->GetTypeIdentifier() == "character")
		{
			g_MessageDispatcher->PushMessage(new Message("landed", this, Content));
		}
	}
}

void Planet::Recharge(Ship * Ship, Character * Character)
{
	if(_OffersRecharging == true)
	{
		assert(Ship != nullptr);
		for(auto Content : Ship->GetAspectObjectContainer()->GetContent())
		{
			auto TheBattery{dynamic_cast< Battery * >(Content)};
			
			if(TheBattery != nullptr)
			{
				assert(Character != nullptr);
				
				float CanBuy{Character->GetCredits() / _RechargingFeePerEnergy};
				float Need{TheBattery->GetEnergyCapacity() - TheBattery->GetEnergy()};
				float Buy{(CanBuy > Need) ? (Need) : (CanBuy)};
				
				TheBattery->SetEnergy(TheBattery->GetEnergy() + Buy);
				Character->RemoveCredits(static_cast< std::uint32_t >(Buy * _RechargingFeePerEnergy));
			}
		}
	}
}

void Planet::Refuel(Ship * Ship, Character * Character)
{
	auto FuelPlanetAssets{GetPlanetAssets("commodity", "fuel")};
	
	if(FuelPlanetAssets != nullptr)
	{
		auto FuelPrice{FuelPlanetAssets->GetPrice()};
		
		assert(Character != nullptr);
		
		auto CanBuy{Character->GetCredits() / FuelPrice};
		
		assert(Ship != nullptr);
		
		auto Need{Ship->GetFuelCapacity() - Ship->GetFuel()};
		auto Buy{(CanBuy > Need) ? (Need) : (CanBuy)};
		
		Ship->SetFuel(Ship->GetFuel() + Buy);
		Character->RemoveCredits(static_cast< std::uint32_t >(Buy * FuelPrice));
	}
}

void Planet::Repair(Ship * Ship, Character * Character)
{
	if(_OffersRepairing == true)
	{
		assert(Character != nullptr);
		
		float CanBuy(Character->GetCredits() / _RepairingFeePerHull);
		
		assert(Ship != nullptr);
		
		float Need(Ship->GetHullCapacity() - Ship->GetHull());
		float Buy((CanBuy > Need) ? (Need) : (CanBuy));
		
		Ship->SetHull(Ship->GetHull() + Buy);
		Character->RemoveCredits(static_cast< std::uint32_t >(Buy * _RepairingFeePerHull));
	}
}

void Planet::TakeOff(Ship * Ship, Character * Character)
{
	assert(Character != nullptr);
	assert(Ship != nullptr);
	
	auto Hangar(GetHangar(Character));
	
	assert(Hangar != nullptr);
	assert(Hangar->GetAspectObjectContainer() != nullptr);
	Hangar->GetAspectObjectContainer()->RemoveContent(Ship);
	if(Hangar->GetAspectObjectContainer()->GetContent().size() == 0)
	{
		Hangar->Destroy();
		delete Hangar;
	}
	assert(GetContainer() != nullptr);
	assert(GetContainer()->GetAspectObjectContainer() != nullptr);
	GetContainer()->GetAspectObjectContainer()->AddContent(Ship);
	assert(Ship->GetAspectObjectContainer() != nullptr);
	for(auto Content : Ship->GetAspectObjectContainer()->GetContent())
	{
		if(Content->GetTypeIdentifier() == "character")
		{
			g_MessageDispatcher->PushMessage(new Message("taken_off", this, Content));
		}
	}
}

Hangar * Planet::_CreateHangar(Character * Character)
{
	assert(Character != nullptr);
	assert(g_ObjectFactory != nullptr);
	
	auto Result(dynamic_cast< Hangar * >(g_ObjectFactory->Create("hangar", "", true)));
	
	Result->SetCharacter(Character);
	
	return Result;
}

void Planet::_OnFactionDestroying(void)
{
	assert(_Faction != nullptr);
	assert(_FactionDestroyingConnection.IsValid() == true);
	_FactionDestroyingConnection.Disconnect();
	assert(_FactionDestroyingConnection.IsValid() == false);
	_Faction = nullptr;
}

void Planet::_UpdateVisualization(Visualization * Visualization)
{
	assert(Visualization != nullptr);
	assert(Visualization->GetGraphics() != nullptr);
	assert(GetAspectPosition() != nullptr);
	if(Visualization->GetUpdateOrientation() == true)
	{
		Visualization->GetGraphics()->SetOrientation(GetAspectPosition()->GetOrientation());
	}
	if(Visualization->GetUpdatePosition() == true)
	{
		Visualization->GetGraphics()->SetPosition(GetAspectPosition()->GetPosition());
	}
}
