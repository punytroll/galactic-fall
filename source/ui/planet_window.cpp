/**
 * galactic-fall
 * Copyright (C) 2013  Hagen Möbius
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
#include "../key_event_information.h"
#include "../object_aspect_name.h"
#include "../planet.h"
#include "../ship.h"
#include "button.h"
#include "label.h"
#include "planet_window.h"
#include "trade_center_widget.h"

UI::PlanetWindow::PlanetWindow(UI::Widget * SupWidget, Reference< Planet > Planet, Reference< Character > Character) :
	UI::Window(SupWidget),
	_Character(Character),
	_DescriptionLabel(0),
	_Planet(Planet),
	_TradeCenterWidget(0)
{
	assert(_Character.IsValid() == true);
	assert(_Planet.IsValid() == true);
	assert(_Planet->GetAspectName() != 0);
	GetTitleLabel()->SetText("Planet: " + _Planet->GetAspectName()->GetName());
	SetPosition(Vector2f(50.0f, 50.0f));
	SetSize(Vector2f(700.0f, 400.0f));
	ConnectKeyCallback(Callback(this, &PlanetWindow::_OnKey));
	
	UI::Button * HomeButton(new UI::Button(this));
	
	HomeButton->SetPosition(Vector2f(10.0f, 40.0f));
	HomeButton->SetSize(Vector2f(100.0f, 20.0f));
	HomeButton->ConnectClickedCallback(Callback(this, &PlanetWindow::_OnHomeButtonClicked));
	
	UI::Label * HomeButtonLabel(new UI::Label(HomeButton, "Home"));
	
	HomeButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	HomeButtonLabel->SetSize(HomeButton->GetSize());
	HomeButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	HomeButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	
	UI::Button * TakeOffButton(new UI::Button(this));
	
	TakeOffButton->SetPosition(Vector2f(10.0f, 150.0f));
	TakeOffButton->SetSize(Vector2f(100.0f, 20.0f));
	TakeOffButton->ConnectClickedCallback(Callback(this, &PlanetWindow::_OnTakeOffButtonClicked));
	
	UI::Label * TakeOffButtonLabel(new UI::Label(TakeOffButton, "Take Off"));
	
	TakeOffButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	TakeOffButtonLabel->SetSize(TakeOffButton->GetSize());
	TakeOffButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	TakeOffButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	
	UI::Button * TradeCenterButton(new UI::Button(this));
	
	TradeCenterButton->SetPosition(Vector2f(10.0f, 70.0f));
	TradeCenterButton->SetSize(Vector2f(100.0f, 20.0f));
	TradeCenterButton->ConnectClickedCallback(Callback(this, &PlanetWindow::_OnTradeCenterButtonClicked));
	
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
			RefuelButton->ConnectClickedCallback(Callback(this, &PlanetWindow::_OnRefuelButtonClicked));
			
			UI::Label * RefuelButtonLabel(new UI::Label(RefuelButton, "Refuel"));
			
			RefuelButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
			RefuelButtonLabel->SetSize(RefuelButton->GetSize());
			RefuelButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
			RefuelButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
			
			break;
		}
	}
	_OnHomeButtonClicked();
}

void UI::PlanetWindow::_OnHomeButtonClicked(void)
{
	if(_TradeCenterWidget != 0)
	{
		assert(_DescriptionLabel == 0);
		_TradeCenterWidget->Destroy();
		_TradeCenterWidget = 0;
	}
	if(_DescriptionLabel == 0)
	{
		_DescriptionLabel = new UI::Label(this, _Planet->GetDescription());
		_DescriptionLabel->SetPosition(Vector2f(120.0f, 40.0f));
		_DescriptionLabel->SetSize(Vector2f(GetSize()[0] - 130.0f, GetSize()[1] - 50.0f));
		_DescriptionLabel->SetWrap(true);
		_DescriptionLabel->SetWordWrap(true);
		_DescriptionLabel->SetAnchorRight(true);
	}
	SetKeyFocus(0);
}

bool UI::PlanetWindow::_OnKey(const KeyEventInformation & KeyEventInformation)
{
	if(((KeyEventInformation.GetKeyCode() == 9 /* ESCAPE */) || (KeyEventInformation.GetKeyCode() == 36 /* RETURN */)) && (KeyEventInformation.IsDown() == true))
	{
		_TakeOff();
	}
	else if((KeyEventInformation.GetKeyCode() == 28 /* T */) && (KeyEventInformation.IsDown() == true))
	{
		_OpenTradeCenter();
	}
	
	// eat all input
	return true;
}

void UI::PlanetWindow::_OnRefuelButtonClicked(void)
{
	_Refuel();
}

void UI::PlanetWindow::_OnTakeOffButtonClicked(void)
{
	_TakeOff();
}

void UI::PlanetWindow::_OnTradeCenterButtonClicked(void)
{
	_OpenTradeCenter();
}

void UI::PlanetWindow::_OpenTradeCenter(void)
{
	if(_DescriptionLabel != 0)
	{
		assert(_TradeCenterWidget == 0);
		_DescriptionLabel->Destroy();
		_DescriptionLabel = 0;
	}
	if(_TradeCenterWidget == 0)
	{
		_TradeCenterWidget = new UI::TradeCenterWidget(this, _Planet, _Character);
		_TradeCenterWidget->SetPosition(Vector2f(120.0f, 40.0f));
		_TradeCenterWidget->SetSize(Vector2f(GetSize()[0] - 130.0f, GetSize()[1] - 50.0f));
		_TradeCenterWidget->SetAnchorBottom(true);
		_TradeCenterWidget->SetAnchorRight(true);
	}
	_TradeCenterWidget->GrabKeyFocus();
}

void UI::PlanetWindow::_Refuel(void)
{
	assert(_Planet.IsValid() == true);
	
	const std::vector< PlanetAssetClass * > & PlanetAssetClasses(_Planet->GetPlanetAssetClasses());
	
	for(std::vector< PlanetAssetClass * >::const_iterator PlanetAssetClassIterator = PlanetAssetClasses.begin(); PlanetAssetClassIterator != PlanetAssetClasses.end(); ++PlanetAssetClassIterator)
	{
		if((*PlanetAssetClassIterator)->GetAssetClass()->GetIdentifier() == "fuel")
		{
			u4byte FuelPrice((*PlanetAssetClassIterator)->GetPrice());
			
			assert(_Character.IsValid() == true);
			
			float CanBuy(_Character->GetCredits() / FuelPrice);
			
			assert(_Character->GetShip() != 0);
			
			float Need(_Character->GetShip()->GetFuelCapacity() - _Character->GetShip()->GetFuel());
			float Buy((CanBuy > Need) ? (Need) : (CanBuy));
			
			_Character->GetShip()->SetFuel(_Character->GetShip()->GetFuel() + Buy);
			_Character->RemoveCredits(static_cast< u4byte >(Buy * FuelPrice));
			
			break;
		}
	}
}

void UI::PlanetWindow::_TakeOff(void)
{
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != 0);
	_Character->GetShip()->SetTakeOff(true);
}
