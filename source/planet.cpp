/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#include "asset_class.h"
#include "battery.h"
#include "character.h"
#include "color.h"
#include "globals.h"
#include "hangar.h"
#include "message.h"
#include "message_dispatcher.h"
#include "object_aspect_object_container.h"
#include "object_aspect_physical.h"
#include "object_factory.h"
#include "planet.h"
#include "ship.h"

PlanetAssetClass::PlanetAssetClass(const AssetClass * AssetClass) :
	_AssetClass(AssetClass),
	_BasePriceModifier(1.0f)
{
}

PlanetAssetClass::~PlanetAssetClass(void)
{
}

unsigned_numeric PlanetAssetClass::GetPrice(void) const
{
	return static_cast< unsigned_numeric >(_AssetClass->GetBasePrice() * _BasePriceModifier);
}

Planet::Planet(const std::string & Identifier) :
	_Identifier(Identifier),
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
}

Planet::~Planet(void)
{
	while(_PlanetAssetClasses.size() > 0)
	{
		delete _PlanetAssetClasses.back();
		_PlanetAssetClasses.pop_back();
	}
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

void Planet::SetDescription(const std::string & Description)
{
	_Description = Description;
}

void Planet::SetSize(const float & Size)
{
	_Size = Size;
	assert(GetAspectPhysical() != nullptr);
	GetAspectPhysical()->SetRadialSize(_Size / 2.0f);
}

PlanetAssetClass * Planet::CreatePlanetAssetClass(const AssetClass * AssetClass)
{
	/// @todo check whether the planet asset class already exists
	_PlanetAssetClasses.push_back(new PlanetAssetClass(AssetClass));
	
	return _PlanetAssetClasses.back();
}

void Planet::Land(Ship * Ship, Character * Character)
{
	assert(Character != nullptr);
	assert(Ship != nullptr);
	Ship->SetVelocity(Vector3f(0.0f, 0.0f, 0.0f));
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
			g_MessageDispatcher->PushMessage(new Message("landed", GetReference(), Content->GetReference()));
		}
	}
}

void Planet::Recharge(Ship * Ship, Character * Character)
{
	if(_OffersRecharging == true)
	{
		assert(Ship != nullptr);
		if(Ship->GetBattery() != nullptr)
		{
			assert(Character != nullptr);
			
			float CanBuy(Character->GetCredits() / _RechargingFeePerEnergy);
			float Need(Ship->GetBattery()->GetEnergyCapacity() - Ship->GetBattery()->GetEnergy());
			float Buy((CanBuy > Need) ? (Need) : (CanBuy));
			
			Ship->GetBattery()->SetEnergy(Ship->GetBattery()->GetEnergy() + Buy);
			Character->RemoveCredits(static_cast< unsigned_numeric >(Buy * _RechargingFeePerEnergy));
		}
	}
}

void Planet::Refuel(Ship * Ship, Character * Character)
{
	for(auto PlanetAssetClass : _PlanetAssetClasses)
	{
		if(PlanetAssetClass->GetAssetClass()->GetIdentifier() == "fuel")
		{
			unsigned_numeric FuelPrice(PlanetAssetClass->GetPrice());
			
			assert(Character != nullptr);
			
			float CanBuy(Character->GetCredits() / FuelPrice);
			
			assert(Ship != nullptr);
			
			float Need(Ship->GetFuelCapacity() - Ship->GetFuel());
			float Buy((CanBuy > Need) ? (Need) : (CanBuy));
			
			Ship->SetFuel(Ship->GetFuel() + Buy);
			Character->RemoveCredits(static_cast< u4byte >(Buy * FuelPrice));
			
			break;
		}
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
		Character->RemoveCredits(static_cast< unsigned_numeric >(Buy * _RepairingFeePerHull));
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
			g_MessageDispatcher->PushMessage(new Message("taken_off", GetReference(), Content->GetReference()));
		}
	}
}

Hangar * Planet::_CreateHangar(Character * Character)
{
	assert(Character != nullptr);
	assert(g_ObjectFactory != nullptr);
	
	auto Result(dynamic_cast< Hangar * >(g_ObjectFactory->Create("hangar", "")));
	
	Result->SetObjectIdentifier("::hangar::on_planet(" + GetObjectIdentifier() + ")::for_character(" + Character->GetObjectIdentifier() + ")");
	Result->SetCharacter(Character);
	
	return Result;
}
