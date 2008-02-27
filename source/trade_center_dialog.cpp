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
#include "commodity.h"
#include "commodity_class.h"
#include "game_time.h"
#include "globals.h"
#include "key_event_information.h"
#include "label.h"
#include "object_aspect_name.h"
#include "object_factory.h"
#include "planet.h"
#include "scroll_bar.h"
#include "scroll_box.h"
#include "ship.h"
#include "string_cast.h"
#include "trade_center_dialog.h"
#include "weapon.h"
#include "weapon_class.h"

class TradeCenterAssetClass : public Widget
{
public:
	TradeCenterAssetClass(Widget * SupWidget, PlanetAssetClass * PlanetAssetClass, Ship * Ship);
	void UpdateCharacterAmount(void);
	const PlanetAssetClass * GetPlanetAssetClass(void) const;
private:
	PlanetAssetClass * m_PlanetAssetClass;
	Ship * m_Ship;
	Label * m_CharacterAmountLabel;
};

TradeCenterAssetClass::TradeCenterAssetClass(Widget * SupWidget, PlanetAssetClass * PlanetAssetClass, Ship * Ship) :
	Widget(SupWidget),
	m_PlanetAssetClass(PlanetAssetClass),
	m_Ship(Ship)
{
	Label * PlanetAssetClassNameLabel(new Label(this, PlanetAssetClass->GetAssetClass()->GetName()));
	
	PlanetAssetClassNameLabel->SetPosition(Vector2f(10.0f, 0.0f));
	PlanetAssetClassNameLabel->SetSize(Vector2f(0.0f, 20.0f));
	PlanetAssetClassNameLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	PlanetAssetClassNameLabel->SetAnchorRight(true);
	m_CharacterAmountLabel = new Label(this, "");
	m_CharacterAmountLabel->SetPosition(Vector2f(-110.0f, 0.0f));
	m_CharacterAmountLabel->SetSize(Vector2f(50.0f, 20.0f));
	m_CharacterAmountLabel->SetHorizontalAlignment(Label::ALIGN_RIGHT);
	m_CharacterAmountLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_CharacterAmountLabel->SetAnchorLeft(false);
	m_CharacterAmountLabel->SetAnchorRight(true);
	
	Label * PlanetAssetClassPriceLabel(new Label(this, to_string_cast(PlanetAssetClass->GetPrice())));
	
	PlanetAssetClassPriceLabel->SetPosition(Vector2f(-60.0f, 0.0f));
	PlanetAssetClassPriceLabel->SetSize(Vector2f(50.0f, 20.0f));
	PlanetAssetClassPriceLabel->SetHorizontalAlignment(Label::ALIGN_RIGHT);
	PlanetAssetClassPriceLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	PlanetAssetClassPriceLabel->SetAnchorLeft(false);
	PlanetAssetClassPriceLabel->SetAnchorRight(true);
	UpdateCharacterAmount();
}

void TradeCenterAssetClass::UpdateCharacterAmount(void)
{
	m_CharacterAmountLabel->SetString(to_string_cast(m_Ship->GetContentAmount(m_PlanetAssetClass->GetAssetClass()->GetObjectType(), m_PlanetAssetClass->GetAssetClass()->GetObjectClass())));
}

const PlanetAssetClass * TradeCenterAssetClass::GetPlanetAssetClass(void) const
{
	return m_PlanetAssetClass;
}

TradeCenterDialog::TradeCenterDialog(Widget * SupWidget, Planet * Planet, Character * Character) :
	WWindow(SupWidget, "Trade Center: " + Planet->GetAspectName()->GetName()),
	m_Planet(Planet),
	m_Character(Character),
	m_SelectedTradeCenterAssetClass(0)
{
	SetPosition(Vector2f(600.0f, 100.0f));
	SetSize(Vector2f(500.0f, 330.0f));
	AddKeyListener(this);
	m_OKButton = new Button(this);
	m_OKButton->SetPosition(Vector2f(390.0f, 300.0f));
	m_OKButton->SetSize(Vector2f(100.0f, 20.0f));
	m_OKButton->SetAnchorBottom(true);
	m_OKButton->SetAnchorLeft(false);
	m_OKButton->SetAnchorRight(true);
	m_OKButton->SetAnchorTop(false);
	m_OKButton->AddClickedListener(this);
	m_OKButtonLabel = new Label(m_OKButton, "OK");
	m_OKButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	m_OKButtonLabel->SetSize(m_OKButton->GetSize());
	m_OKButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	m_OKButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_BuyButton = new Button(this);
	m_BuyButton->SetPosition(Vector2f(10.0f, 300.0f));
	m_BuyButton->SetSize(Vector2f(100.0f, 20.0f));
	m_BuyButton->SetAnchorBottom(true);
	m_BuyButton->SetAnchorTop(false);
	m_BuyButton->AddClickedListener(this);
	m_BuyButtonLabel = new Label(m_BuyButton, "Buy");
	m_BuyButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	m_BuyButtonLabel->SetSize(m_BuyButton->GetSize());
	m_BuyButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	m_BuyButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_SellButton = new Button(this);
	m_SellButton->SetPosition(Vector2f(120.0f, 300.0f));
	m_SellButton->SetSize(Vector2f(100.0f, 20.0f));
	m_SellButton->SetAnchorBottom(true);
	m_SellButton->SetAnchorTop(false);
	m_SellButton->AddClickedListener(this);
	m_SellButtonLabel = new Label(m_SellButton, "Sell");
	m_SellButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	m_SellButtonLabel->SetSize(m_SellButton->GetSize());
	m_SellButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	m_SellButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_AssetClassScrollBox = new ScrollBox(this);
	m_AssetClassScrollBox->SetPosition(Vector2f(10.0f, 40.0f));
	m_AssetClassScrollBox->SetSize(Vector2f(480.0f, 150.0f));
	m_AssetClassScrollBox->SetHorizontalScrollBarVisible(false);
	m_AssetClassScrollBox->SetAnchorRight(true);
	m_AssetClassScrollBox->SetAnchorBottom(true);
	m_AssetClassScrollBox->AddMouseButtonListener(this);
	
	const std::vector< PlanetAssetClass * > & PlanetAssetClasses(Planet->GetPlanetAssetClasses());
	std::vector< PlanetAssetClass * >::const_iterator PlanetAssetClassIterator(PlanetAssetClasses.begin());
	float Top(5.0f);
	
	while(PlanetAssetClassIterator != PlanetAssetClasses.end())
	{
		TradeCenterAssetClass * NewTradeCenterAssetClass(new TradeCenterAssetClass(m_AssetClassScrollBox->GetContent(), *PlanetAssetClassIterator, m_Character->GetShip()));
		
		NewTradeCenterAssetClass->SetPosition(Vector2f(5.0f, Top));
		NewTradeCenterAssetClass->SetSize(Vector2f(m_AssetClassScrollBox->GetContent()->GetSize()[0] - 10.0f, 20.0f));
		NewTradeCenterAssetClass->SetAnchorRight(true);
		NewTradeCenterAssetClass->AddMouseButtonListener(this);
		NewTradeCenterAssetClass->AddMouseMotionListener(this);
		Top += 25.0f;
		++PlanetAssetClassIterator;
	}
	m_AssetClassScrollBox->GetContent()->SetSize(Vector2f(460.0f, Top));
	m_AssetClassScrollBox->GetContent()->SetAnchorRight(true);
	m_TraderCreditsLabel = new Label(this, "");
	m_TraderCreditsLabel->SetPosition(Vector2f(10.0f, 240.0f));
	m_TraderCreditsLabel->SetSize(Vector2f(200.0f, 20.0f));
	m_TraderCreditsLabel->SetAnchorBottom(true);
	m_TraderCreditsLabel->SetAnchorTop(false);
	m_TraderAvailableSpaceLabel = new Label(this, "");
	m_TraderAvailableSpaceLabel->SetPosition(Vector2f(10.0f, 260.0f));
	m_TraderAvailableSpaceLabel->SetSize(Vector2f(200.0f, 20.0f));
	m_TraderAvailableSpaceLabel->SetAnchorBottom(true);
	m_TraderAvailableSpaceLabel->SetAnchorTop(false);
	UpdateTraderCredits();
	UpdateTraderAvailableSpace();
}

void TradeCenterDialog::UpdateTraderCredits(void)
{
	m_TraderCreditsLabel->SetString("Credits: " + to_string_cast(m_Character->GetCredits()));
}

void TradeCenterDialog::UpdateTraderAvailableSpace(void)
{
	m_TraderAvailableSpaceLabel->SetString("Available Space: " + to_string_cast(0.001 * m_Character->GetShip()->GetAvailableSpace(), 3));
}

void TradeCenterDialog::Buy(const PlanetAssetClass * PlanetAssetClass)
{
	u4byte Price(PlanetAssetClass->GetPrice());
	
	if(m_Character->RemoveCredits(Price) == true)
	{
		if(m_Character->GetShip()->GetAvailableSpace() >= g_ObjectFactory->GetSpaceRequirement(PlanetAssetClass->GetAssetClass()->GetObjectType(), PlanetAssetClass->GetAssetClass()->GetObjectClass()))
		{
			Object * NewCargo(g_ObjectFactory->Create(PlanetAssetClass->GetAssetClass()->GetObjectType(), PlanetAssetClass->GetAssetClass()->GetObjectClass()));
			
			NewCargo->SetObjectIdentifier("::asset(" + PlanetAssetClass->GetAssetClass()->GetIdentifier() + ")::" + PlanetAssetClass->GetAssetClass()->GetObjectType() + "(" + PlanetAssetClass->GetAssetClass()->GetObjectClass() + ")::created_on(" + m_Planet->GetObjectIdentifier() + ")::created_at_game_time(" + to_string_cast(GameTime::Get(), 6) + ")::bought_by(" + m_Character->GetObjectIdentifier() + ")::created_at_address(" + to_string_cast(reinterpret_cast< void * >(NewCargo)) + ")");
			m_Character->GetShip()->AddContent(NewCargo);
			UpdateTraderCredits();
			UpdateTraderAvailableSpace();
		}
		else
		{
			m_Character->AddCredits(Price);
		}
	}
}

void TradeCenterDialog::Sell(const PlanetAssetClass * PlanetAssetClass)
{
	const std::set< Object * > & Content(m_Character->GetShip()->GetContent());
	std::set< Object * >::const_iterator ContentIterator(Content.begin());
	
	while(ContentIterator != Content.end())
	{
		Object * ContentObject(0);
		
		if(PlanetAssetClass->GetAssetClass()->GetObjectType() == "commodity")
		{
			Commodity * ContentCommodity(dynamic_cast< Commodity * >(*ContentIterator));
			
			if((ContentCommodity != 0) && (ContentCommodity->GetCommodityClass()->GetIdentifier() == PlanetAssetClass->GetAssetClass()->GetObjectClass()))
			{
				ContentObject = ContentCommodity;
			}
		}
		else if(PlanetAssetClass->GetAssetClass()->GetObjectType() == "weapon")
		{
			Weapon * ContentWeapon(dynamic_cast< Weapon * >(*ContentIterator));
			
			if((ContentWeapon != 0) && (ContentWeapon->GetSlot() == 0) && (ContentWeapon->GetWeaponClass()->GetIdentifier() == PlanetAssetClass->GetAssetClass()->GetObjectClass()))
			{
				ContentObject = ContentWeapon;
			}
		}
		if(ContentObject != 0)
		{
			m_Character->GetShip()->RemoveContent(ContentObject);
			delete ContentObject;
			m_Character->AddCredits(PlanetAssetClass->GetPrice());
			UpdateTraderCredits();
			UpdateTraderAvailableSpace();
			
			break;
		}
		++ContentIterator;
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
		if(m_SelectedTradeCenterAssetClass != 0)
		{
			Buy(m_SelectedTradeCenterAssetClass->GetPlanetAssetClass());
			m_SelectedTradeCenterAssetClass->UpdateCharacterAmount();
		}
	}
	else if(EventSource == m_SellButton)
	{
		if(m_SelectedTradeCenterAssetClass != 0)
		{
			Sell(m_SelectedTradeCenterAssetClass->GetPlanetAssetClass());
			m_SelectedTradeCenterAssetClass->UpdateCharacterAmount();
		}
	}
	
	return false;
}

bool TradeCenterDialog::OnKey(Widget * EventSource, const KeyEventInformation & KeyEventInformation)
{
	if(((KeyEventInformation.GetKeyCode() == 9 /* ESCAPE */) || (KeyEventInformation.GetKeyCode() == 36 /* RETURN */) || (KeyEventInformation.GetKeyCode() == 28 /* T */)) && (KeyEventInformation.IsDown() == true))
	{
		Destroy();
	}
	else if((KeyEventInformation.GetKeyCode() == 56 /* B */) && (m_SelectedTradeCenterAssetClass != 0) && (KeyEventInformation.IsDown() == true))
	{
		Buy(m_SelectedTradeCenterAssetClass->GetPlanetAssetClass());
		m_SelectedTradeCenterAssetClass->UpdateCharacterAmount();
	}
	else if((KeyEventInformation.GetKeyCode() == 39 /* S */) && (m_SelectedTradeCenterAssetClass != 0) && (KeyEventInformation.IsDown() == true))
	{
		Sell(m_SelectedTradeCenterAssetClass->GetPlanetAssetClass());
		m_SelectedTradeCenterAssetClass->UpdateCharacterAmount();
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
		TradeCenterAssetClass * SelectedTradeCenterAssetClass(dynamic_cast< TradeCenterAssetClass * >(EventSource));
		
		if(SelectedTradeCenterAssetClass != 0)
		{
			if(m_SelectedTradeCenterAssetClass != 0)
			{
				m_SelectedTradeCenterAssetClass->UnsetBackgroundColor();
			}
			m_SelectedTradeCenterAssetClass = SelectedTradeCenterAssetClass;
			SelectedTradeCenterAssetClass->SetBackgroundColor(Color(0.4f, 0.1f, 0.1f, 1.0f));
			
			return true;
		}
	}
	else if((Button == 4 /* WHEEL_UP */) && (State == EV_DOWN))
	{
		m_AssetClassScrollBox->GetVerticalScrollBar()->StepLess();
		
		return true;
	}
	else if((Button == 5 /* WHEEL_DOWN */) && (State == EV_DOWN))
	{
		m_AssetClassScrollBox->GetVerticalScrollBar()->StepMore();
		
		return true;
	}
	
	return false;
}

void TradeCenterDialog::OnMouseEnter(Widget * EventSource)
{
	WWindow::OnMouseEnter(EventSource);
	
	TradeCenterAssetClass * EnteredTradeCenterAssetClass(dynamic_cast< TradeCenterAssetClass * >(EventSource));
	
	if((EnteredTradeCenterAssetClass != 0) && (EnteredTradeCenterAssetClass != m_SelectedTradeCenterAssetClass))
	{
		EnteredTradeCenterAssetClass->SetBackgroundColor(Color(0.3f, 0.2f, 0.2f, 1.0f));
	}
}

void TradeCenterDialog::OnMouseLeave(Widget * EventSource)
{
	WWindow::OnMouseLeave(EventSource);
	
	TradeCenterAssetClass * LeftTradeCenterAssetClass(dynamic_cast< TradeCenterAssetClass * >(EventSource));
	
	if((LeftTradeCenterAssetClass != 0) && (LeftTradeCenterAssetClass != m_SelectedTradeCenterAssetClass))
	{
		LeftTradeCenterAssetClass->UnsetBackgroundColor();
	}
}
