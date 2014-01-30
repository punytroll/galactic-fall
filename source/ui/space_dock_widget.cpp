/**
 * galactic-fall
 * Copyright (C) 2014  Hagen Möbius
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
#include "../ship.h"
#include "../planet.h"
#include "button.h"
#include "label.h"
#include "space_dock_widget.h"

UI::SpaceDockWidget::SpaceDockWidget(UI::Widget * SupWidget, Reference< Planet > Planet, Reference< Character > Character) :
	UI::Widget(SupWidget),
	_Character(Character),
	_Planet(Planet)
{
	SetSize(Vector2f(600.0f, 300.0f));
	
	UI::Button * RepairButton(new UI::Button(this));
	
	RepairButton->SetPosition(Vector2f(0.0f, 0.0f));
	RepairButton->SetSize(Vector2f(180.0f, 20.0f));
	RepairButton->ConnectClickedCallback(Callback(this, &SpaceDockWidget::_OnRepairButtonClicked));
	
	UI::Label * RepairButtonLabel(new UI::Label(RepairButton, "Repair"));
	
	RepairButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	RepairButtonLabel->SetSize(RepairButton->GetSize());
	RepairButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	RepairButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	
	UI::Button * RechargeButton(new UI::Button(this));
	
	RechargeButton->SetPosition(Vector2f(0.0f, 30.0f));
	RechargeButton->SetSize(Vector2f(180.0f, 20.0f));
	RechargeButton->ConnectClickedCallback(Callback(this, &SpaceDockWidget::_OnRechargeButtonClicked));
	
	UI::Label * RechargeButtonLabel(new UI::Label(RechargeButton, "Recharge"));
	
	RechargeButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	RechargeButtonLabel->SetSize(RechargeButton->GetSize());
	RechargeButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	RechargeButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	
	const std::vector< PlanetAssetClass * > & PlanetAssetClasses(_Planet->GetPlanetAssetClasses());
	
	for(std::vector< PlanetAssetClass * >::const_iterator PlanetAssetClassIterator = PlanetAssetClasses.begin(); PlanetAssetClassIterator != PlanetAssetClasses.end(); ++PlanetAssetClassIterator)
	{
		if((*PlanetAssetClassIterator)->GetAssetClass()->GetIdentifier() == "fuel")
		{
			UI::Button * RefuelButton(new UI::Button(this));
			
			RefuelButton->SetPosition(Vector2f(0.0f, 60.0f));
			RefuelButton->SetSize(Vector2f(180.0f, 20.0f));
			RefuelButton->ConnectClickedCallback(Callback(this, &SpaceDockWidget::_OnRefuelButtonClicked));
			
			UI::Label * RefuelButtonLabel(new UI::Label(RefuelButton, "Refuel"));
			
			RefuelButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
			RefuelButtonLabel->SetSize(RefuelButton->GetSize());
			RefuelButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
			RefuelButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
			
			break;
		}
	}
	
	UI::Button * TakeOffButton(new UI::Button(this));
	
	TakeOffButton->SetPosition(Vector2f(0.0f, 90.0f));
	TakeOffButton->SetSize(Vector2f(180.0f, 20.0f));
	TakeOffButton->ConnectClickedCallback(Callback(this, &SpaceDockWidget::_OnTakeOffButtonClicked));
	
	UI::Label * TakeOffButtonLabel(new UI::Label(TakeOffButton, "Take Off"));
	
	TakeOffButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	TakeOffButtonLabel->SetSize(TakeOffButton->GetSize());
	TakeOffButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	TakeOffButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
}

void UI::SpaceDockWidget::_OnRechargeButtonClicked(void)
{
	_Recharge();
}

void UI::SpaceDockWidget::_OnRefuelButtonClicked(void)
{
	_Refuel();
}

void UI::SpaceDockWidget::_OnRepairButtonClicked(void)
{
	_Repair();
}

void UI::SpaceDockWidget::_OnTakeOffButtonClicked(void)
{
	_TakeOff();
}

void UI::SpaceDockWidget::_Recharge(void)
{
	assert(_Planet.IsValid() == true);
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != 0);
	_Planet->Recharge(_Character->GetShip(), _Character.Get());
}

void UI::SpaceDockWidget::_Refuel(void)
{
	assert(_Planet.IsValid() == true);
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != 0);
	_Planet->Refuel(_Character->GetShip(), _Character.Get());
}

void UI::SpaceDockWidget::_Repair(void)
{
	assert(_Planet.IsValid() == true);
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != 0);
	_Planet->Repair(_Character->GetShip(), _Character.Get());
}

void UI::SpaceDockWidget::_TakeOff(void)
{
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != 0);
	_Character->GetShip()->SetTakeOff(true);
}
