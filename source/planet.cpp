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
#include "message.h"
#include "message_dispatcher.h"
#include "object_aspect_object_container.h"
#include "object_aspect_physical.h"
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

void Planet::SetDescription(const std::string & Description)
{
	_Description = Description;
}

void Planet::SetSize(const float & Size)
{
	_Size = Size;
	assert(GetAspectPhysical() != 0);
	GetAspectPhysical()->SetRadialSize(_Size / 2.0f);
}

PlanetAssetClass * Planet::CreatePlanetAssetClass(const AssetClass * AssetClass)
{
	/// @todo check whether the planet asset class already exists
	_PlanetAssetClasses.push_back(new PlanetAssetClass(AssetClass));
	
	return _PlanetAssetClasses.back();
}

void Planet::Land(Ship * Ship)
{
	assert(Ship != nullptr);
	Ship->SetVelocity(Vector3f(0.0f, 0.0f, 0.0f));
	assert(Ship->GetContainer() != nullptr);
	assert(Ship->GetContainer()->GetAspectObjectContainer() != nullptr);
	Ship->GetContainer()->GetAspectObjectContainer()->RemoveContent(Ship);
	assert(GetAspectObjectContainer() != nullptr);
	GetAspectObjectContainer()->AddContent(Ship);
	
	assert(Ship->GetAspectObjectContainer() != 0);
	
	// send message to all characters on the landed ship
	const std::set< Object * > & ShipContent(Ship->GetAspectObjectContainer()->GetContent());
	
	for(std::set< Object * >::iterator ContentIterator = ShipContent.begin(); ContentIterator != ShipContent.end(); ++ContentIterator)
	{
		Object * Content(*ContentIterator);
		
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
		assert(Ship != 0);
		if(Ship->GetBattery() != 0)
		{
			assert(Character != 0);
			
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
	for(std::vector< PlanetAssetClass * >::iterator PlanetAssetClassIterator = _PlanetAssetClasses.begin(); PlanetAssetClassIterator != _PlanetAssetClasses.end(); ++PlanetAssetClassIterator)
	{
		if((*PlanetAssetClassIterator)->GetAssetClass()->GetIdentifier() == "fuel")
		{
			unsigned_numeric FuelPrice((*PlanetAssetClassIterator)->GetPrice());
			
			assert(Character != 0);
			
			float CanBuy(Character->GetCredits() / FuelPrice);
			
			assert(Ship != 0);
			
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
		assert(Character != 0);
		
		float CanBuy(Character->GetCredits() / _RepairingFeePerHull);
		
		assert(Ship != 0);
		
		float Need(Ship->GetHullCapacity() - Ship->GetHull());
		float Buy((CanBuy > Need) ? (Need) : (CanBuy));
		
		Ship->SetHull(Ship->GetHull() + Buy);
		Character->RemoveCredits(static_cast< unsigned_numeric >(Buy * _RepairingFeePerHull));
	}
}

void Planet::TakeOff(Ship * Ship)
{
	assert(Ship != nullptr);
	assert(GetAspectObjectContainer() != nullptr);
	GetAspectObjectContainer()->RemoveContent(Ship);
	assert(GetContainer() != nullptr);
	assert(GetContainer()->GetAspectObjectContainer() != nullptr);
	GetContainer()->GetAspectObjectContainer()->AddContent(Ship);
	
	// send message to all characters on the ship that took off
	const std::set< Object * > & ShipContent(Ship->GetAspectObjectContainer()->GetContent());
	
	for(std::set< Object * >::iterator ContentIterator = ShipContent.begin(); ContentIterator != ShipContent.end(); ++ContentIterator)
	{
		Object * Content(*ContentIterator);
		
		if(Content->GetTypeIdentifier() == "character")
		{
			g_MessageDispatcher->PushMessage(new Message("taken_off", GetReference(), Content->GetReference()));
		}
	}
}
