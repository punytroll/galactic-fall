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

#include "../asset_class.h"
#include "../callbacks/callbacks.h"
#include "../character.h"
#include "../color.h"
#include "../globals.h"
#include "../key_event_information.h"
#include "../object_aspect_name.h"
#include "../planet.h"
#include "../ship.h"
#include "../string_cast.h"
#include "button.h"
#include "label.h"
#include "planet_dialog.h"
#include "trade_center_widget.h"

UI::PlanetDialog::PlanetDialog(UI::Widget * SupWidget, Planet * Planet, Character * Character) :
	UI::Window(SupWidget, "Planet: " + Planet->GetAspectName()->GetName()),
	_Planet(Planet),
	_Character(Character),
	_TradeCenterWidget(0)
{
	SetPosition(Vector2f(50.0f, 50.0f));
	SetSize(Vector2f(700.0f, 400.0f));
	ConnectKeyCallback(Callback(this, &PlanetDialog::_OnKey));
	
	UI::Button * HomeButton(new UI::Button(this));
	
	HomeButton->SetPosition(Vector2f(10.0f, 40.0f));
	HomeButton->SetSize(Vector2f(100.0f, 20.0f));
	HomeButton->ConnectClickedCallback(Callback(this, &PlanetDialog::_OnHomeButtonClicked));
	
	UI::Label * HomeButtonLabel(new UI::Label(HomeButton, "Home"));
	
	HomeButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	HomeButtonLabel->SetSize(HomeButton->GetSize());
	HomeButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	HomeButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	
	UI::Button * TakeOffButton(new UI::Button(this));
	
	TakeOffButton->SetPosition(Vector2f(10.0f, 150.0f));
	TakeOffButton->SetSize(Vector2f(100.0f, 20.0f));
	TakeOffButton->ConnectClickedCallback(Callback(this, &PlanetDialog::_OnTakeOffButtonClicked));
	
	UI::Label * TakeOffButtonLabel(new UI::Label(TakeOffButton, "Take Off"));
	
	TakeOffButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	TakeOffButtonLabel->SetSize(TakeOffButton->GetSize());
	TakeOffButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	TakeOffButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	
	UI::Button * TradeCenterButton(new UI::Button(this));
	
	TradeCenterButton->SetPosition(Vector2f(10.0f, 70.0f));
	TradeCenterButton->SetSize(Vector2f(100.0f, 20.0f));
	TradeCenterButton->ConnectClickedCallback(Callback(this, &PlanetDialog::_OnTradeCenterButtonClicked));
	
	UI::Label * TradeCenterLabel(new UI::Label(TradeCenterButton, "Trade Center"));
	
	TradeCenterLabel->SetPosition(Vector2f(0.0f, 0.0f));
	TradeCenterLabel->SetSize(TradeCenterButton->GetSize());
	TradeCenterLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	TradeCenterLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	
	const std::vector< PlanetAssetClass * > & PlanetAssetClasses(_Planet->GetPlanetAssetClasses());
	
	for(std::vector< PlanetAssetClass * >::const_iterator PlanetAssetClassIterator = PlanetAssetClasses.begin(); PlanetAssetClassIterator != PlanetAssetClasses.end(); ++PlanetAssetClassIterator)
	{
		if((*PlanetAssetClassIterator)->GetAssetClass()->GetIdentifier() == "fuel")
		{
			UI::Button * RefuelButton(new UI::Button(this));
			
			RefuelButton->SetPosition(Vector2f(10.0f, 100.0f));
			RefuelButton->SetSize(Vector2f(100.0f, 20.0f));
			RefuelButton->ConnectClickedCallback(Callback(this, &PlanetDialog::_OnRefuelButtonClicked));
			
			UI::Label * RefuelButtonLabel(new UI::Label(RefuelButton, "Refuel"));
			
			RefuelButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
			RefuelButtonLabel->SetSize(RefuelButton->GetSize());
			RefuelButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
			RefuelButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
			
			break;
		}
	}
	_DescriptionLabel = new UI::Label(this, _Planet->GetDescription());
	_DescriptionLabel->SetPosition(Vector2f(120.0f, 40.0f));
	_DescriptionLabel->SetSize(Vector2f(570.0f, 350.0f));
	_DescriptionLabel->SetWrap(true);
	_DescriptionLabel->SetWordWrap(true);
	_DescriptionLabel->SetAnchorRight(true);
	_DescriptionLabel->SetVisible(false);
	_TradeCenterWidget = new UI::TradeCenterWidget(this, _Planet, _Character);
	_TradeCenterWidget->SetPosition(Vector2f(120.0f, 40.0f));
	_TradeCenterWidget->SetSize(Vector2f(570.0f, 350.0f));
	_TradeCenterWidget->SetAnchorBottom(true);
	_TradeCenterWidget->SetAnchorRight(true);
	_TradeCenterWidget->SetVisible(false);
	_OnHomeButtonClicked();
}

void UI::PlanetDialog::_OnHomeButtonClicked(void)
{
	_TradeCenterWidget->SetVisible(false);
	_DescriptionLabel->SetVisible(true);
	SetKeyFocus(0);
}

void UI::PlanetDialog::_OnRefuelButtonClicked(void)
{
	const std::vector< PlanetAssetClass * > & PlanetAssetClasses(_Planet->GetPlanetAssetClasses());
	
	for(std::vector< PlanetAssetClass * >::const_iterator PlanetAssetClassIterator = PlanetAssetClasses.begin(); PlanetAssetClassIterator != PlanetAssetClasses.end(); ++PlanetAssetClassIterator)
	{
		if((*PlanetAssetClassIterator)->GetAssetClass()->GetIdentifier() == "fuel")
		{
			u4byte FuelPrice((*PlanetAssetClassIterator)->GetPrice());
			float CanBuy(_Character->GetCredits() / FuelPrice);
			float Need(_Character->GetShip()->GetFuelCapacity() - _Character->GetShip()->GetFuel());
			float Buy((CanBuy > Need) ? (Need) : (CanBuy));
			
			_Character->GetShip()->SetFuel(_Character->GetShip()->GetFuel() + Buy);
			_Character->RemoveCredits(static_cast< u4byte >(Buy * FuelPrice));
			
			break;
		}
	}
}

void UI::PlanetDialog::_OnTakeOffButtonClicked(void)
{
	_Character->GetShip()->SetTakeOff(true);
}

void UI::PlanetDialog::_OnTradeCenterButtonClicked(void)
{
	_OpenTradeCenterWidget();
}

bool UI::PlanetDialog::_OnKey(const KeyEventInformation & KeyEventInformation)
{
	if(((KeyEventInformation.GetKeyCode() == 9 /* ESCAPE */) || (KeyEventInformation.GetKeyCode() == 36 /* RETURN */)) && (KeyEventInformation.IsDown() == true))
	{
		_Character->GetShip()->SetTakeOff(true);
	}
	else if((KeyEventInformation.GetKeyCode() == 28 /* T */) && (KeyEventInformation.IsDown() == true))
	{
		_OpenTradeCenterWidget();
	}
	
	// eat all input
	return true;
}

void UI::PlanetDialog::_OpenTradeCenterWidget(void)
{
	_DescriptionLabel->SetVisible(false);
	_TradeCenterWidget->SetVisible(true);
	_TradeCenterWidget->GrabKeyFocus();
}
