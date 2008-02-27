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
#include "character.h"
#include "color.h"
#include "globals.h"
#include "message.h"
#include "message_dispatcher.h"
#include "planet.h"
#include "ship.h"

PlanetAssetClass::PlanetAssetClass(const AssetClass * AssetClass) :
	m_AssetClass(AssetClass),
	m_BasePriceModifier(1.0f)
{
}

PlanetAssetClass::~PlanetAssetClass(void)
{
}

u4byte PlanetAssetClass::GetPrice(void) const
{
	return static_cast< u4byte >(m_AssetClass->GetBasePrice() * m_BasePriceModifier);
}

Planet::Planet(const std::string & Identifier) :
	m_Identifier(Identifier),
	m_Color(0)
{
	// initialize object aspects
	AddAspectName();
}

Planet::~Planet(void)
{
	while(m_PlanetAssetClasses.size() > 0)
	{
		delete m_PlanetAssetClasses.back();
		m_PlanetAssetClasses.pop_back();
	}
	delete m_Color;
	m_Color = 0;
}

void Planet::SetDescription(const std::string & Description)
{
	m_Description = Description;
}

void Planet::SetSize(const float & Size)
{
	m_Size = Size;
	SetRadialSize(m_Size / 2.0f);
}

void Planet::SetColor(const Color & NewColor)
{
	delete m_Color;
	m_Color = new Color(NewColor);
}

PlanetAssetClass * Planet::CreatePlanetAssetClass(const AssetClass * AssetClass)
{
	/// @todo check whether the planet asset class already exists
	m_PlanetAssetClasses.push_back(new PlanetAssetClass(AssetClass));
	
	return m_PlanetAssetClasses.back();
}

void Planet::Land(Ship * Ship)
{
	Ship->SetVelocity(Vector3f(0.0f, 0.0f, 0.0f));
	Ship->SetHull(Ship->GetShipClass()->GetHull());
	
	// send message to all characters on the landed ship
	const std::set< Object * > & ShipContent(Ship->GetContent());
	
	for(std::set< Object * >::iterator ContentIterator = ShipContent.begin(); ContentIterator != ShipContent.end(); ++ContentIterator)
	{
		Character * TheCharacter(dynamic_cast< Character * >(*ContentIterator));
		
		if(TheCharacter != 0)
		{
			g_MessageDispatcher->PushMessage(new Message("landed", GetReference(), TheCharacter->GetReference()));
		}
	}
}

void Planet::TakeOff(Ship * Ship)
{
}
