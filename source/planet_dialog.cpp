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
#include "callbacks/callbacks.h"
#include "character.h"
#include "color.h"
#include "globals.h"
#include "key_event_information.h"
#include "object_aspect_name.h"
#include "planet.h"
#include "planet_dialog.h"
#include "ship.h"
#include "string_cast.h"
#include "trade_center_dialog.h"
#include "ui/label.h"

PlanetDialog::PlanetDialog(Widget * SupWidget, Planet * Planet, Character * Character) :
	WWindow(SupWidget, "Planet: " + Planet->GetAspectName()->GetName()),
	m_Planet(Planet),
	m_Character(Character),
	m_TradeCenterDialog(0)
{
	SetPosition(Vector2f(50.0f, 50.0f));
	SetSize(Vector2f(500.0f, 330.0f));
	ConnectDestroyingCallback(Callback(this, &PlanetDialog::OnDestroying));
	ConnectKeyCallback(Callback(this, &PlanetDialog::OnKey));
	
	UI::Label * DescriptionLabel(new UI::Label(this, m_Planet->GetDescription()));
	
	DescriptionLabel->SetPosition(Vector2f(120.0f, 40.0f));
	DescriptionLabel->SetSize(Vector2f(360.0f, 100.0f));
	DescriptionLabel->SetWrap(true);
	DescriptionLabel->SetWordWrap(true);
	DescriptionLabel->SetAnchorRight(true);
	m_TakeOffButton = new Button(this);
	m_TakeOffButton->SetPosition(Vector2f(390.0f, 300.0f));
	m_TakeOffButton->SetSize(Vector2f(100.0f, 20.0f));
	m_TakeOffButton->ConnectClickedCallback(Callback(this, &PlanetDialog::OnTakeOffClicked));
	m_TakeOffButton->SetAnchorBottom(true);
	m_TakeOffButton->SetAnchorLeft(false);
	m_TakeOffButton->SetAnchorRight(true);
	m_TakeOffButton->SetAnchorTop(false);
	
	UI::Label * TakeOffButtonLabel(new UI::Label(m_TakeOffButton, "Take Off"));
	
	TakeOffButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	TakeOffButtonLabel->SetSize(m_TakeOffButton->GetSize());
	TakeOffButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	TakeOffButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	m_TradeCenterButton = new Button(this);
	m_TradeCenterButton->SetPosition(Vector2f(10.0f, 40.0f));
	m_TradeCenterButton->SetSize(Vector2f(100.0f, 20.0f));
	m_TradeCenterButton->ConnectClickedCallback(Callback(this, &PlanetDialog::OnTradeCenterClicked));
	
	UI::Label * TradeCenterLabel(new UI::Label(m_TradeCenterButton, "Trade Center"));
	TradeCenterLabel->SetPosition(Vector2f(0.0f, 0.0f));
	TradeCenterLabel->SetSize(m_TradeCenterButton->GetSize());
	TradeCenterLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	TradeCenterLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	
	const std::vector< PlanetAssetClass * > & PlanetAssetClasses(m_Planet->GetPlanetAssetClasses());
	
	for(std::vector< PlanetAssetClass * >::const_iterator PlanetAssetClassIterator = PlanetAssetClasses.begin(); PlanetAssetClassIterator != PlanetAssetClasses.end(); ++PlanetAssetClassIterator)
	{
		if((*PlanetAssetClassIterator)->GetAssetClass()->GetIdentifier() == "fuel")
		{
			m_RefuelButton = new Button(this);
			m_RefuelButton->SetPosition(Vector2f(10.0f, 70.0f));
			m_RefuelButton->SetSize(Vector2f(100.0f, 20.0f));
			m_RefuelButton->ConnectClickedCallback(Callback(this, &PlanetDialog::OnRefuelClicked));
			
			UI::Label * RefuelButtonLabel(new UI::Label(m_RefuelButton, "Refuel"));
			
			RefuelButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
			RefuelButtonLabel->SetSize(m_TradeCenterButton->GetSize());
			RefuelButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
			RefuelButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
			
			UI::Label * FuelPriceLabel(new UI::Label(this, "Local fuel price is: " + to_string_cast((*PlanetAssetClassIterator)->GetPrice()) + " credits/unit."));
			
			FuelPriceLabel->SetPosition(Vector2f(10.0f, 300.0f));
			FuelPriceLabel->SetSize(Vector2f(300.0f, 20.0f));
			FuelPriceLabel->SetAnchorTop(false);
			FuelPriceLabel->SetAnchorBottom(true);
			
			break;
		}
	}
}

void PlanetDialog::OnDestroying(void)
{
	if(m_TradeCenterDialog != 0)
	{
		m_TradeCenterDialog->Destroy();
	}
}

void PlanetDialog::OnRefuelClicked(void)
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

void PlanetDialog::OnTakeOffClicked(void)
{
	m_Character->GetShip()->SetTakeOff(true);
}

void PlanetDialog::OnTradeCenterClicked(void)
{
	OpenTradeCenterDialog();
}

void PlanetDialog::OnTradeCenterDialogDestroying(void)
{
	m_TradeCenterDialog = 0;
	GrabKeyFocus();
}

bool PlanetDialog::OnKey(const KeyEventInformation & KeyEventInformation)
{
	if(((KeyEventInformation.GetKeyCode() == 9 /* ESCAPE */) || (KeyEventInformation.GetKeyCode() == 36 /* RETURN */)) && (KeyEventInformation.IsDown() == true))
	{
		m_Character->GetShip()->SetTakeOff(true);
	}
	else if((KeyEventInformation.GetKeyCode() == 28 /* T */) && (KeyEventInformation.IsDown() == true))
	{
		OpenTradeCenterDialog();
	}
	
	// eat all input
	return true;
}

void PlanetDialog::OpenTradeCenterDialog(void)
{
	if(m_TradeCenterDialog == 0)
	{
		m_TradeCenterDialog = new TradeCenterDialog(GetRootWidget(), m_Planet, m_Character);
		m_TradeCenterDialog->GrabKeyFocus();
		m_TradeCenterDialog->ConnectDestroyingCallback(Callback(this, &PlanetDialog::OnTradeCenterDialogDestroying));
	}
}
