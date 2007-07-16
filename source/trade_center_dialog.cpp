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

#include "button.h"
#include "cargo.h"
#include "character.h"
#include "commodity_class.h"
#include "globals.h"
#include "label.h"
#include "object_factory.h"
#include "planet.h"
#include "real_time.h"
#include "ship.h"
#include "string_cast.h"
#include "trade_center_dialog.h"

class TradeCenterCommodityClass : public Widget
{
public:
	TradeCenterCommodityClass(Widget * SupWidget, PlanetCommodityClass * PlanetCommodityClass, Ship * Ship);
	void UpdateCharacterAmount(void);
	const PlanetCommodityClass * GetPlanetCommodity(void) const;
private:
	PlanetCommodityClass * m_PlanetCommodity;
	Ship * m_Ship;
	Label * m_CharacterAmountLabel;
};

TradeCenterCommodityClass::TradeCenterCommodityClass(Widget * SupWidget, PlanetCommodityClass * PlanetCommodityClass, Ship * Ship) :
	Widget(SupWidget),
	m_PlanetCommodity(PlanetCommodityClass),
	m_Ship(Ship)
{
	Label * CommodityNameLabel(new Label(this, PlanetCommodityClass->GetCommodityClass()->GetName()));
	
	CommodityNameLabel->SetPosition(Vector2f(10.0f, 0.0f));
	CommodityNameLabel->SetSize(Vector2f(310.0f, 20.0f));
	CommodityNameLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_CharacterAmountLabel = new Label(this, "");
	m_CharacterAmountLabel->SetPosition(Vector2f(330.0f, 0.0f));
	m_CharacterAmountLabel->SetSize(Vector2f(50.0f, 20.0f));
	m_CharacterAmountLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	
	Label * CommodityPriceLabel(new Label(this, to_string_cast(PlanetCommodityClass->GetPrice())));
	
	CommodityPriceLabel->SetPosition(Vector2f(390.0f, 0.0f));
	CommodityPriceLabel->SetSize(Vector2f(50.0f, 20.0f));
	CommodityPriceLabel->SetHorizontalAlignment(Label::ALIGN_RIGHT);
	CommodityPriceLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	UpdateCharacterAmount();
}

void TradeCenterCommodityClass::UpdateCharacterAmount(void)
{
	m_CharacterAmountLabel->SetString(to_string_cast(m_Ship->GetContentAmount(m_PlanetCommodity->GetCommodityClass()->GetObjectType(), m_PlanetCommodity->GetCommodityClass()->GetObjectClass())));
}

const PlanetCommodityClass * TradeCenterCommodityClass::GetPlanetCommodity(void) const
{
	return m_PlanetCommodity;
}

TradeCenterDialog::TradeCenterDialog(Widget * SupWidget, Planet * Planet, Character * Character) :
	WWindow(SupWidget, "Trade Center: " + Planet->GetName()),
	m_Planet(Planet),
	m_Character(Character),
	m_SelectedTradeCenterCommodityClass(0)
{
	SetPosition(Vector2f(600.0f, 100.0f));
	SetSize(Vector2f(500.0f, 330.0f));
	AddKeyListener(this);
	m_OKButton = new Button(this);
	m_OKButton->SetPosition(Vector2f(390.0f, 300.0f));
	m_OKButton->SetSize(Vector2f(100.0f, 20.0f));
	m_OKButton->AddClickedListener(this);
	m_OKButtonLabel = new Label(m_OKButton, "OK");
	m_OKButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	m_OKButtonLabel->SetSize(m_OKButton->GetSize());
	m_OKButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	m_OKButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_BuyButton = new Button(this);
	m_BuyButton->SetPosition(Vector2f(10.0f, 300.0f));
	m_BuyButton->SetSize(Vector2f(100.0f, 20.0f));
	m_BuyButton->AddClickedListener(this);
	m_BuyButtonLabel = new Label(m_BuyButton, "Buy");
	m_BuyButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	m_BuyButtonLabel->SetSize(m_BuyButton->GetSize());
	m_BuyButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	m_BuyButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_SellButton = new Button(this);
	m_SellButton->SetPosition(Vector2f(120.0f, 300.0f));
	m_SellButton->SetSize(Vector2f(100.0f, 20.0f));
	m_SellButton->AddClickedListener(this);
	m_SellButtonLabel = new Label(m_SellButton, "Sell");
	m_SellButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	m_SellButtonLabel->SetSize(m_SellButton->GetSize());
	m_SellButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	m_SellButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_CommodityList = new Widget(this);
	m_CommodityList->SetPosition(Vector2f(10.0f, 40.0f));
	m_CommodityList->SetSize(Vector2f(480.0f, 150.0f));
	m_CommodityList->SetBackgroundColor(Color(0.23f, 0.23f, 0.23f, 1.0f));
	m_CommodityListItems = new Widget(m_CommodityList);
	m_CommodityListItems->SetPosition(Vector2f(0.0f, 0.0f));
	m_CommodityListItems->SetBackgroundColor(Color(0.15f, 0.15f, 0.15f, 1.0f));
	m_CommodityListItems->AddMouseButtonListener(this);
	
	const std::vector< PlanetCommodityClass * > & PlanetCommodityClasses(Planet->GetCommodityClasses());
	std::vector< PlanetCommodityClass * >::const_iterator PlanetCommodityClassIterator(PlanetCommodityClasses.begin());
	float Top(5.0f);
	
	while(PlanetCommodityClassIterator != PlanetCommodityClasses.end())
	{
		TradeCenterCommodityClass * NewTradeCenterCommodity(new TradeCenterCommodityClass(m_CommodityListItems, *PlanetCommodityClassIterator, m_Character->GetShip()));
		
		NewTradeCenterCommodity->SetPosition(Vector2f(5.0f, Top));
		NewTradeCenterCommodity->SetSize(Vector2f(450.0f, 20.0f));
		NewTradeCenterCommodity->AddMouseButtonListener(this);
		NewTradeCenterCommodity->AddMouseMotionListener(this);
		Top += 25.0f;
		++PlanetCommodityClassIterator;
	}
	m_CommodityListItems->SetSize(Vector2f(460.0f, Top));
	m_UpButton = new Button(this);
	m_UpButton->SetPosition(Vector2f(470.0f, 40.0f));
	m_UpButton->SetSize(Vector2f(20.0f, 20.0f));
	m_UpButton->AddClickedListener(this);
	m_DownButton = new Button(this);
	m_DownButton->SetPosition(Vector2f(470.0f, 170.0f));
	m_DownButton->SetSize(Vector2f(20.0f, 20.0f));
	m_DownButton->AddClickedListener(this);
	m_TraderCreditsLabel = new Label(this, "");
	m_TraderCreditsLabel->SetPosition(Vector2f(10.0f, 240.0f));
	m_TraderCreditsLabel->SetSize(Vector2f(200.0f, 20.0f));
	m_TraderFreeCargoHoldSizeLabel = new Label(this, "");
	m_TraderFreeCargoHoldSizeLabel->SetPosition(Vector2f(10.0f, 260.0f));
	m_TraderFreeCargoHoldSizeLabel->SetSize(Vector2f(200.0f, 20.0f));
	UpdateTraderCredits();
	UpdateTraderFreeCargoHoldSize();
}

void TradeCenterDialog::UpdateTraderCredits(void)
{
	m_TraderCreditsLabel->SetString("Credits: " + to_string_cast(m_Character->GetCredits()));
}

void TradeCenterDialog::UpdateTraderFreeCargoHoldSize(void)
{
	m_TraderFreeCargoHoldSizeLabel->SetString("Free Cargo Hold: " + to_string_cast(m_Character->GetShip()->GetFreeCargoHoldSize()));
}

void TradeCenterDialog::Buy(const PlanetCommodityClass * PlanetCommodityClass)
{
	u4byte Price(PlanetCommodityClass->GetPrice());
	
	if(m_Character->RemoveCredits(Price) == true)
	{
		if(m_Character->GetShip()->GetFreeCargoHoldSize() >= 1.0f)
		{
			Object * NewCargo(g_ObjectFactory->Create(PlanetCommodityClass->GetCommodityClass()->GetObjectType(), PlanetCommodityClass->GetCommodityClass()->GetObjectClass()));
			
			NewCargo->SetObjectIdentifier("::" + PlanetCommodityClass->GetCommodityClass()->GetObjectType() + "(" + PlanetCommodityClass->GetCommodityClass()->GetObjectClass() + ")::created_on(" + m_Planet->GetObjectIdentifier() + ")::created_at(" + to_string_cast(RealTime::GetTime(), 6) + ")::bought_by(" + m_Character->GetObjectIdentifier() + ")::commodity_class(" + PlanetCommodityClass->GetCommodityClass()->GetIdentifier() + ")");
			m_Character->GetShip()->AddContent(NewCargo);
			UpdateTraderCredits();
			UpdateTraderFreeCargoHoldSize();
		}
		else
		{
			m_Character->AddCredits(Price);
		}
	}
}

void TradeCenterDialog::Sell(const PlanetCommodityClass * PlanetCommodityClass)
{
	const std::set< Object * > & Manifest(m_Character->GetShip()->GetContent());
	std::set< Object * >::const_iterator ManifestIterator(Manifest.begin());
	
	while(ManifestIterator != Manifest.end())
	{
		Cargo * TheCargo(dynamic_cast< Cargo * >(*ManifestIterator));
		
		if((TheCargo != 0) && (TheCargo->GetCommodityClass() == PlanetCommodityClass->GetCommodityClass()))
		{
			m_Character->GetShip()->RemoveContent(*ManifestIterator);
			delete TheCargo;
			m_Character->AddCredits(PlanetCommodityClass->GetPrice());
			UpdateTraderCredits();
			UpdateTraderFreeCargoHoldSize();
			
			break;
		}
		++ManifestIterator;
	}
}

bool TradeCenterDialog::OnClicked(Widget * EventSource)
{
	if(EventSource == m_OKButton)
	{
		Destroy();
		
		return true;
	}
	else if(EventSource == m_BuyButton)
	{
		if(m_SelectedTradeCenterCommodityClass != 0)
		{
			Buy(m_SelectedTradeCenterCommodityClass->GetPlanetCommodity());
			m_SelectedTradeCenterCommodityClass->UpdateCharacterAmount();
		}
	}
	else if(EventSource == m_SellButton)
	{
		if(m_SelectedTradeCenterCommodityClass != 0)
		{
			Sell(m_SelectedTradeCenterCommodityClass->GetPlanetCommodity());
			m_SelectedTradeCenterCommodityClass->UpdateCharacterAmount();
		}
	}
	else if(EventSource == m_UpButton)
	{
		Vector2f Position(m_CommodityListItems->GetPosition());
		
		if(Position[1] < 0.0f)
		{
			Position[1] = std::min(0.0f, Position[1] + 10.0f);
			m_CommodityListItems->SetPosition(Position);
		}
	}
	else if(EventSource == m_DownButton)
	{
		Vector2f Position(m_CommodityListItems->GetPosition());
		const Vector2f & Size(m_CommodityListItems->GetSize());
		
		if(Position[1] + Size[1] > m_CommodityList->GetSize()[1])
		{
			Position[1] -= std::min(10.f, Position[1] + Size[1] - m_CommodityList->GetSize()[1]);
			m_CommodityListItems->SetPosition(Position);
		}
	}
	
	return false;
}

bool TradeCenterDialog::OnKey(Widget * EventSource, int Key, int State)
{
	if(((Key == 9 /* ESCAPE */) || (Key == 36 /* RETURN */) || (Key == 28 /* T */)) && (State == EV_DOWN))
	{
		Destroy();
	}
	else if((Key == 56 /* B */) && (m_SelectedTradeCenterCommodityClass != 0) && (State == EV_DOWN))
	{
		Buy(m_SelectedTradeCenterCommodityClass->GetPlanetCommodity());
		m_SelectedTradeCenterCommodityClass->UpdateCharacterAmount();
	}
	else if((Key == 39 /* S */) && (m_SelectedTradeCenterCommodityClass != 0) && (State == EV_DOWN))
	{
		Sell(m_SelectedTradeCenterCommodityClass->GetPlanetCommodity());
		m_SelectedTradeCenterCommodityClass->UpdateCharacterAmount();
	}
	
	return true;
}

bool TradeCenterDialog::OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y)
{
	if(WWindow::OnMouseButton(EventSource, Button, State, X, Y) == true)
	{
		return true;
	}
	if((Button == 1 /* LEFT */) && (State == EV_DOWN))
	{
		TradeCenterCommodityClass * SelectedTradeCenterCommodity(dynamic_cast< TradeCenterCommodityClass * >(EventSource));
		
		if(SelectedTradeCenterCommodity != 0)
		{
			if(m_SelectedTradeCenterCommodityClass != 0)
			{
				m_SelectedTradeCenterCommodityClass->UnsetBackgroundColor();
			}
			m_SelectedTradeCenterCommodityClass = SelectedTradeCenterCommodity;
			SelectedTradeCenterCommodity->SetBackgroundColor(Color(0.4f, 0.1f, 0.1f));
			
			return true;
		}
	}
	else if((Button == 4 /* WHEEL_UP */) && (State == EV_DOWN))
	{
		Vector2f Position(m_CommodityListItems->GetPosition());
		
		if(Position[1] < 0.0f)
		{
			Position[1] = std::min(0.0f, Position[1] + 5.0f);
			m_CommodityListItems->SetPosition(Position);
		}
		
		return true;
	}
	else if((Button == 5 /* WHEEL_DOWN */) && (State == EV_DOWN))
	{
		Vector2f Position(m_CommodityListItems->GetPosition());
		const Vector2f & Size(m_CommodityListItems->GetSize());
		
		if(Position[1] + Size[1] > m_CommodityList->GetSize()[1])
		{
			Position[1] -= std::min(5.f, Position[1] + Size[1] - m_CommodityList->GetSize()[1]);
			m_CommodityListItems->SetPosition(Position);
		}
		
		return true;
	}
	
	return false;
}

void TradeCenterDialog::OnMouseEnter(Widget * EventSource)
{
	WWindow::OnMouseEnter(EventSource);
	
	TradeCenterCommodityClass * EnteredTradeCenterCommodity(dynamic_cast< TradeCenterCommodityClass * >(EventSource));
	
	if((EnteredTradeCenterCommodity != 0) && (EnteredTradeCenterCommodity != m_SelectedTradeCenterCommodityClass))
	{
		EnteredTradeCenterCommodity->SetBackgroundColor(Color(0.3f, 0.2f, 0.2f));
	}
}

void TradeCenterDialog::OnMouseLeave(Widget * EventSource)
{
	WWindow::OnMouseLeave(EventSource);
	
	TradeCenterCommodityClass * LeftTradeCenterCommodity(dynamic_cast< TradeCenterCommodityClass * >(EventSource));
	
	if((LeftTradeCenterCommodity != 0) && (LeftTradeCenterCommodity != m_SelectedTradeCenterCommodityClass))
	{
		LeftTradeCenterCommodity->UnsetBackgroundColor();
	}
}
