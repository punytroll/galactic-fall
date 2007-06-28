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

#include "border.h"
#include "button.h"
#include "character.h"
#include "color.h"
#include "commodity.h"
#include "globals.h"
#include "label.h"
#include "planet.h"
#include "planet_dialog.h"
#include "ship.h"
#include "string_cast.h"
#include "trade_center_dialog.h"

PlanetDialog::PlanetDialog(Widget * SupWidget, Planet * Planet, Character * Character) :
	WWindow(SupWidget, "Planet: " + Planet->GetName()),
	m_Planet(Planet),
	m_Character(Character),
	m_TradeCenterDialog(0)
{
	SetPosition(Vector2f(50.0f, 50.0f));
	SetSize(Vector2f(500.0f, 330.0f));
	GetBorder()->SetSize(Vector2f(500.0f, 330.0f));
	AddKeyListener(this);
	m_DescriptionLabel = new Label(this, m_Planet->GetDescription());
	m_DescriptionLabel->SetPosition(Vector2f(120.0f, 40.0f));
	m_DescriptionLabel->SetSize(Vector2f(360.0f, 100.0f));
	m_DescriptionLabel->SetWrap(true);
	m_DescriptionLabel->SetWordWrap(true);
	m_OKButton = new Button(this);
	m_OKButton->SetPosition(Vector2f(390.0f, 300.0f));
	m_OKButton->SetSize(Vector2f(100.0f, 20.0f));
	m_OKButton->AddClickedListener(this);
	m_OKButtonLabel = new Label(m_OKButton, "OK");
	m_OKButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	m_OKButtonLabel->SetSize(m_OKButton->GetSize());
	m_OKButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	m_OKButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_TradeCenterButton = new Button(this);
	m_TradeCenterButton->SetPosition(Vector2f(10.0f, 40.0f));
	m_TradeCenterButton->SetSize(Vector2f(100.0f, 20.0f));
	m_TradeCenterButton->AddClickedListener(this);
	m_TradeCenterLabel = new Label(m_TradeCenterButton, "Trade Center");
	m_TradeCenterLabel->SetPosition(Vector2f(0.0f, 0.0f));
	m_TradeCenterLabel->SetSize(m_TradeCenterButton->GetSize());
	m_TradeCenterLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	m_TradeCenterLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	
	const std::vector< PlanetCommodity * > & PlanetCommodities(m_Planet->GetCommodities());
	
	for(std::vector< PlanetCommodity * >::const_iterator PlanetCommodityIterator = PlanetCommodities.begin(); PlanetCommodityIterator != PlanetCommodities.end(); ++PlanetCommodityIterator)
	{
		if((*PlanetCommodityIterator)->GetCommodity()->GetIdentifier() == "fuel")
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
			
			Label * FuelPriceLabel = new Label(this, "Local fuel price is: " + to_string_cast((*PlanetCommodityIterator)->GetPrice()) + " credits/unit.");
			
			FuelPriceLabel->SetPosition(Vector2f(10.0f, 300.0f));
			
			break;
		}
	}
}

bool PlanetDialog::OnClicked(Widget * EventSource)
{
	if(EventSource == m_OKButton)
	{
		if(m_TradeCenterDialog == 0)
		{
			Destroy();
		}
		
		return true;
	}
	else if(EventSource == m_RefuelButton)
	{
		const std::vector< PlanetCommodity * > & PlanetCommodities(m_Planet->GetCommodities());
		
		for(std::vector< PlanetCommodity * >::const_iterator PlanetCommodityIterator = PlanetCommodities.begin(); PlanetCommodityIterator != PlanetCommodities.end(); ++PlanetCommodityIterator)
		{
			if((*PlanetCommodityIterator)->GetCommodity()->GetIdentifier() == "fuel")
			{
				float FuelPrice((*PlanetCommodityIterator)->GetPrice());
				float CanBuy(m_Character->GetCredits() / FuelPrice);
				float Need(m_Character->GetShip()->GetFuelCapacity() - m_Character->GetShip()->GetFuel());
				float Buy((CanBuy > Need) ? (Need) : (CanBuy));
				
				m_Character->GetShip()->SetFuel(m_Character->GetShip()->GetFuel() + Buy);
				m_Character->RemoveCredits(Buy * FuelPrice);
				
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
	WWindow::OnDestroy(EventSource);
	if(EventSource == m_TradeCenterDialog)
	{
		m_TradeCenterDialog = 0;
		GrabKeyFocus();
	}
}

bool PlanetDialog::OnKey(Widget * EventSource, int KeyCode, int State)
{
	if(((KeyCode == 9 /* ESCAPE */) || (KeyCode == 36 /* RETURN */)) && (State == EV_DOWN))
	{
		Destroy();
	}
	else if((KeyCode == 28 /* T */) && (State == EV_DOWN))
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
