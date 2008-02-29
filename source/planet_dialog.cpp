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
#include "button.h"
#include "character.h"
#include "color.h"
#include "globals.h"
#include "key_event_information.h"
#include "label.h"
#include "object_aspect_name.h"
#include "planet.h"
#include "planet_dialog.h"
#include "ship.h"
#include "string_cast.h"
#include "trade_center_dialog.h"

PlanetDialog::PlanetDialog(Widget * SupWidget, Planet * Planet, Character * Character) :
	WWindow(SupWidget, "Planet: " + Planet->GetAspectName()->GetName()),
	m_Planet(Planet),
	m_Character(Character),
	m_TradeCenterDialog(0)
{
	SetPosition(Vector2f(50.0f, 50.0f));
	SetSize(Vector2f(500.0f, 330.0f));
	AddDestroyListener(this);
	AddKeyListener(this);
	m_DescriptionLabel = new Label(this, m_Planet->GetDescription());
	m_DescriptionLabel->SetPosition(Vector2f(120.0f, 40.0f));
	m_DescriptionLabel->SetSize(Vector2f(360.0f, 100.0f));
	m_DescriptionLabel->SetWrap(true);
	m_DescriptionLabel->SetWordWrap(true);
	m_DescriptionLabel->SetAnchorRight(true);
	m_TakeOffButton = new Button(this);
	m_TakeOffButton->SetPosition(Vector2f(390.0f, 300.0f));
	m_TakeOffButton->SetSize(Vector2f(100.0f, 20.0f));
	m_TakeOffButton->AddClickedListener(this);
	m_TakeOffButton->SetAnchorBottom(true);
	m_TakeOffButton->SetAnchorLeft(false);
	m_TakeOffButton->SetAnchorRight(true);
	m_TakeOffButton->SetAnchorTop(false);
	
	Label * TakeOffButtonLabel = new Label(m_TakeOffButton, "Take Off");
	
	TakeOffButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	TakeOffButtonLabel->SetSize(m_TakeOffButton->GetSize());
	TakeOffButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	TakeOffButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_TradeCenterButton = new Button(this);
	m_TradeCenterButton->SetPosition(Vector2f(10.0f, 40.0f));
	m_TradeCenterButton->SetSize(Vector2f(100.0f, 20.0f));
	m_TradeCenterButton->AddClickedListener(this);
	m_TradeCenterLabel = new Label(m_TradeCenterButton, "Trade Center");
	m_TradeCenterLabel->SetPosition(Vector2f(0.0f, 0.0f));
	m_TradeCenterLabel->SetSize(m_TradeCenterButton->GetSize());
	m_TradeCenterLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	m_TradeCenterLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	
	const std::vector< PlanetAssetClass * > & PlanetAssetClasses(m_Planet->GetPlanetAssetClasses());
	
	for(std::vector< PlanetAssetClass * >::const_iterator PlanetAssetClassIterator = PlanetAssetClasses.begin(); PlanetAssetClassIterator != PlanetAssetClasses.end(); ++PlanetAssetClassIterator)
	{
		if((*PlanetAssetClassIterator)->GetAssetClass()->GetIdentifier() == "fuel")
		{
			m_RefuelButton = new Button(this);
			m_RefuelButton->SetPosition(Vector2f(10.0f, 70.0f));
			m_RefuelButton->SetSize(Vector2f(100.0f, 20.0f));
			m_RefuelButton->AddClickedListener(this);
			m_RefuelButtonLabel = new Label(m_RefuelButton, "Refuel");
			m_RefuelButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
			m_RefuelButtonLabel->SetSize(m_TradeCenterButton->GetSize());
			m_RefuelButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
			m_RefuelButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
			
			Label * FuelPriceLabel = new Label(this, "Local fuel price is: " + to_string_cast((*PlanetAssetClassIterator)->GetPrice()) + " credits/unit.");
			
			FuelPriceLabel->SetPosition(Vector2f(10.0f, 300.0f));
			FuelPriceLabel->SetSize(Vector2f(300.0f, 20.0f));
			FuelPriceLabel->SetAnchorTop(false);
			FuelPriceLabel->SetAnchorBottom(true);
			
			break;
		}
	}
}

bool PlanetDialog::OnClicked(Widget * EventSource)
{
	if(EventSource == m_TakeOffButton)
	{
		m_Character->GetShip()->SetTakeOff(true);
		
		return true;
	}
	else if(EventSource == m_RefuelButton)
	{
		const std::vector< PlanetAssetClass * > & PlanetAssetClasses(m_Planet->GetPlanetAssetClasses());
		
		for(std::vector< PlanetAssetClass * >::const_iterator PlanetAssetClassIterator = PlanetAssetClasses.begin(); PlanetAssetClassIterator != PlanetAssetClasses.end(); ++PlanetAssetClassIterator)
		{
			if((*PlanetAssetClassIterator)->GetAssetClass()->GetIdentifier() == "fuel")
			{
				u4byte FuelPrice((*PlanetAssetClassIterator)->GetPrice());
				float CanBuy(m_Character->GetCredits() / FuelPrice);
				float Need(m_Character->GetShip()->GetFuelCapacity() - m_Character->GetShip()->GetFuel());
				float Buy((CanBuy > Need) ? (Need) : (CanBuy));
				
				m_Character->GetShip()->SetFuel(m_Character->GetShip()->GetFuel() + Buy);
				m_Character->RemoveCredits(static_cast< u4byte >(Buy * FuelPrice));
				
				break;
			}
		}
	}
	else if(EventSource == m_TradeCenterButton)
	{
		if(m_TradeCenterDialog == 0)
		{
			m_TradeCenterDialog = new TradeCenterDialog(GetRootWidget(), m_Planet, m_Character);
			m_TradeCenterDialog->GrabKeyFocus();
			m_TradeCenterDialog->AddDestroyListener(this);
		}
		
		return true;
	}
	
	return false;
}

void PlanetDialog::OnDestroy(Widget * EventSource)
{
	if(EventSource == this)
	{
		if(m_TradeCenterDialog != 0)
		{
			m_TradeCenterDialog->Destroy();
		}
	}
	else if(EventSource == m_TradeCenterDialog)
	{
		m_TradeCenterDialog = 0;
		GrabKeyFocus();
	}
}

bool PlanetDialog::OnKey(Widget * EventSource, const KeyEventInformation & KeyEventInformation)
{
	if(((KeyEventInformation.GetKeyCode() == 9 /* ESCAPE */) || (KeyEventInformation.GetKeyCode() == 36 /* RETURN */)) && (KeyEventInformation.IsDown() == true))
	{
		m_Character->GetShip()->SetTakeOff(true);
	}
	else if((KeyEventInformation.GetKeyCode() == 28 /* T */) && (KeyEventInformation.IsDown() == true))
	{
		if(m_TradeCenterDialog == 0)
		{
			m_TradeCenterDialog = new TradeCenterDialog(GetRootWidget(), m_Planet, m_Character);
			m_TradeCenterDialog->GrabKeyFocus();
			m_TradeCenterDialog->AddDestroyListener(this);
		}
	}
	// eat up all other keys
	return true;
}
