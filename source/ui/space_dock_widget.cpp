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
#include "../battery.h"
#include "../callbacks/callbacks.h"
#include "../character.h"
#include "../planet.h"
#include "../ship.h"
#include "../string_cast.h"
#include "border.h"
#include "button.h"
#include "label.h"
#include "space_dock_widget.h"

UI::SpaceDockWidget::SpaceDockWidget(UI::Widget * SupWidget, Reference< Planet > Planet, Reference< Character > Character) :
	UI::Widget(SupWidget),
	_Character(Character),
	_Planet(Planet)
{
	assert(_Character.IsValid() == true);
	assert(_Planet.IsValid() == true);
	SetSize(Vector2f(600.0f, 300.0f));
	
	UI::Button * RepairButton(new UI::Button(this));
	
	RepairButton->SetPosition(Vector2f(0.0f, 0.0f));
	RepairButton->SetSize(Vector2f(180.0f, 20.0f));
	RepairButton->ConnectClickedCallback(Callback(this, &UI::SpaceDockWidget::_OnRepairButtonClicked));
	RepairButton->ConnectUpdatingCallback(Bind1(Callback(this, &UI::SpaceDockWidget::_OnRepairButtonUpdating), RepairButton));
	
	UI::Label * RepairButtonLabel(new UI::Label(RepairButton, "Repair"));
	
	RepairButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	RepairButtonLabel->SetSize(RepairButton->GetSize());
	RepairButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	RepairButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	
	UI::Widget * HullStateBar(new UI::Widget(this));
	
	HullStateBar->SetPosition(Vector2f(190.0f, 0.0f));
	HullStateBar->SetSize(Vector2f(410.0f, 20.0f));
	HullStateBar->SetAnchorRight(true);
	
	UI::Widget * HullStateBarFill(new UI::Widget(HullStateBar));
	
	HullStateBarFill->SetPosition(Vector2f(0.0f, 0.0f));
	HullStateBarFill->SetSize(HullStateBar->GetSize());
	
	UI::Label * HullStateBarLabel(new UI::Label(HullStateBar));
	
	HullStateBarLabel->SetPosition(Vector2f(0.0f, 0.0f));
	HullStateBarLabel->SetSize(HullStateBar->GetSize());
	HullStateBarLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	HullStateBarLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	HullStateBarLabel->SetAnchorBottom(true);
	HullStateBarLabel->SetAnchorLeft(true);
	HullStateBarLabel->SetAnchorRight(true);
	HullStateBarLabel->SetAnchorTop(true);
	HullStateBar->ConnectUpdatingCallback(Bind1(Bind1(Callback(this, &UI::SpaceDockWidget::_OnHullStateBarUpdating), HullStateBarFill), HullStateBarLabel));
	
	UI::Border * HullStateBarBorder(new UI::Border(HullStateBar));
	
	HullStateBarBorder->SetPosition(Vector2f(0.0f, 0.0f));
	HullStateBarBorder->SetSize(HullStateBar->GetSize());
	HullStateBarBorder->SetWidth(1.0f);
	HullStateBarBorder->SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
	HullStateBarBorder->SetAnchorBottom(true);
	HullStateBarBorder->SetAnchorLeft(true);
	HullStateBarBorder->SetAnchorRight(true);
	HullStateBarBorder->SetAnchorTop(true);
	
	UI::Button * RechargeButton(new UI::Button(this));
	
	RechargeButton->SetPosition(Vector2f(0.0f, 30.0f));
	RechargeButton->SetSize(Vector2f(180.0f, 20.0f));
	RechargeButton->ConnectClickedCallback(Callback(this, &UI::SpaceDockWidget::_OnRechargeButtonClicked));
	RechargeButton->ConnectUpdatingCallback(Bind1(Callback(this, &UI::SpaceDockWidget::_OnRechargeButtonUpdating), RechargeButton));
	
	UI::Label * RechargeButtonLabel(new UI::Label(RechargeButton, "Recharge"));
	
	RechargeButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	RechargeButtonLabel->SetSize(RechargeButton->GetSize());
	RechargeButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	RechargeButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	
	UI::Widget * EnergyStateBar(new UI::Widget(this));
	
	EnergyStateBar->SetPosition(Vector2f(190.0f, 30.0f));
	EnergyStateBar->SetSize(Vector2f(410.0f, 20.0f));
	EnergyStateBar->SetAnchorRight(true);
	
	UI::Widget * EnergyStateBarFill(new UI::Widget(EnergyStateBar));
	
	EnergyStateBarFill->SetPosition(Vector2f(0.0f, 0.0f));
	EnergyStateBarFill->SetSize(EnergyStateBar->GetSize());
	
	UI::Label * EnergyStateBarLabel(new UI::Label(EnergyStateBar));
	
	EnergyStateBarLabel->SetPosition(Vector2f(0.0f, 0.0f));
	EnergyStateBarLabel->SetSize(EnergyStateBar->GetSize());
	EnergyStateBarLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	EnergyStateBarLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	EnergyStateBarLabel->SetAnchorBottom(true);
	EnergyStateBarLabel->SetAnchorLeft(true);
	EnergyStateBarLabel->SetAnchorRight(true);
	EnergyStateBarLabel->SetAnchorTop(true);
	EnergyStateBar->ConnectUpdatingCallback(Bind1(Bind1(Callback(this, &UI::SpaceDockWidget::_OnEnergyStateBarUpdating), EnergyStateBarFill), EnergyStateBarLabel));
	
	UI::Border * EnergyStateBarBorder(new UI::Border(EnergyStateBar));
	
	EnergyStateBarBorder->SetPosition(Vector2f(0.0f, 0.0f));
	EnergyStateBarBorder->SetSize(EnergyStateBar->GetSize());
	EnergyStateBarBorder->SetWidth(1.0f);
	EnergyStateBarBorder->SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
	EnergyStateBarBorder->SetAnchorBottom(true);
	EnergyStateBarBorder->SetAnchorLeft(true);
	EnergyStateBarBorder->SetAnchorRight(true);
	EnergyStateBarBorder->SetAnchorTop(true);
	
	UI::Button * RefuelButton(new UI::Button(this));
	
	RefuelButton->SetPosition(Vector2f(0.0f, 60.0f));
	RefuelButton->SetSize(Vector2f(180.0f, 20.0f));
	RefuelButton->ConnectClickedCallback(Callback(this, &UI::SpaceDockWidget::_OnRefuelButtonClicked));
	RefuelButton->ConnectUpdatingCallback(Bind1(Callback(this, &UI::SpaceDockWidget::_OnRefuelButtonUpdating), RefuelButton));
	
	UI::Label * RefuelButtonLabel(new UI::Label(RefuelButton, "Refuel"));
	
	RefuelButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	RefuelButtonLabel->SetSize(RefuelButton->GetSize());
	RefuelButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	RefuelButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	
	UI::Widget * FuelStateBar(new UI::Widget(this));
	
	FuelStateBar->SetPosition(Vector2f(190.0f, 60.0f));
	FuelStateBar->SetSize(Vector2f(410.0f, 20.0f));
	FuelStateBar->SetAnchorRight(true);
	
	UI::Widget * FuelStateBarFill(new UI::Widget(FuelStateBar));
	
	FuelStateBarFill->SetPosition(Vector2f(0.0f, 0.0f));
	FuelStateBarFill->SetSize(FuelStateBar->GetSize());
	
	UI::Label * FuelStateBarLabel(new UI::Label(FuelStateBar));
	
	FuelStateBarLabel->SetPosition(Vector2f(0.0f, 0.0f));
	FuelStateBarLabel->SetSize(FuelStateBar->GetSize());
	FuelStateBarLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	FuelStateBarLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	FuelStateBarLabel->SetAnchorBottom(true);
	FuelStateBarLabel->SetAnchorLeft(true);
	FuelStateBarLabel->SetAnchorRight(true);
	FuelStateBarLabel->SetAnchorTop(true);
	FuelStateBar->ConnectUpdatingCallback(Bind1(Bind1(Callback(this, &UI::SpaceDockWidget::_OnFuelStateBarUpdating), FuelStateBarFill), FuelStateBarLabel));
	
	UI::Border * FuelStateBarBorder(new UI::Border(FuelStateBar));
	
	FuelStateBarBorder->SetPosition(Vector2f(0.0f, 0.0f));
	FuelStateBarBorder->SetSize(FuelStateBar->GetSize());
	FuelStateBarBorder->SetWidth(1.0f);
	FuelStateBarBorder->SetColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
	FuelStateBarBorder->SetAnchorBottom(true);
	FuelStateBarBorder->SetAnchorLeft(true);
	FuelStateBarBorder->SetAnchorRight(true);
	FuelStateBarBorder->SetAnchorTop(true);
	
	UI::Button * TakeOffButton(new UI::Button(this));
	
	TakeOffButton->SetPosition(Vector2f(0.0f, 90.0f));
	TakeOffButton->SetSize(Vector2f(180.0f, 20.0f));
	TakeOffButton->ConnectClickedCallback(Callback(this, &UI::SpaceDockWidget::_OnTakeOffButtonClicked));
	
	UI::Label * TakeOffButtonLabel(new UI::Label(TakeOffButton, "Take Off"));
	
	TakeOffButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	TakeOffButtonLabel->SetSize(TakeOffButton->GetSize());
	TakeOffButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	TakeOffButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
}

void UI::SpaceDockWidget::_OnEnergyStateBarUpdating(UI::Widget * EnergyStateBarFill, UI::Label * EnergyStateBarLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != 0);
	assert(_Character->GetShip()->GetBattery() != 0);
	assert(EnergyStateBarLabel != 0);
	EnergyStateBarLabel->SetText(to_string_cast(_Character->GetShip()->GetBattery()->GetEnergy(), 2));
	assert(EnergyStateBarFill != 0);
	
	float EnergyPercentage(_Character->GetShip()->GetBattery()->GetEnergy() / _Character->GetShip()->GetBattery()->GetEnergyCapacity());
	
	EnergyStateBarFill->SetSize(Vector2f(EnergyPercentage * EnergyStateBarLabel->GetSize()[0], EnergyStateBarFill->GetSize()[1]));
	EnergyStateBarFill->SetBackgroundColor(Color(1.0f - EnergyPercentage, EnergyPercentage, 0.0f, 1.0f));
}

void UI::SpaceDockWidget::_OnFuelStateBarUpdating(UI::Widget * FuelStateBarFill, UI::Label * FuelStateBarLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != 0);
	assert(FuelStateBarLabel != 0);
	FuelStateBarLabel->SetText(to_string_cast(_Character->GetShip()->GetFuel(), 2));
	assert(FuelStateBarFill != 0);
	
	float FuelPercentage(_Character->GetShip()->GetFuel() / _Character->GetShip()->GetFuelCapacity());
	
	FuelStateBarFill->SetSize(Vector2f(FuelPercentage * FuelStateBarLabel->GetSize()[0], FuelStateBarFill->GetSize()[1]));
	FuelStateBarFill->SetBackgroundColor(Color(1.0f - FuelPercentage, FuelPercentage, 0.0f, 1.0f));
}

void UI::SpaceDockWidget::_OnHullStateBarUpdating(UI::Widget * HullStateBarFill, UI::Label * HullStateBarLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != 0);
	assert(HullStateBarLabel != 0);
	HullStateBarLabel->SetText(to_string_cast(_Character->GetShip()->GetHull(), 2));
	assert(HullStateBarFill != 0);
	
	float HullPercentage(_Character->GetShip()->GetHull() / _Character->GetShip()->GetHullCapacity());
	
	HullStateBarFill->SetSize(Vector2f(HullPercentage * HullStateBarLabel->GetSize()[0], HullStateBarFill->GetSize()[1]));
	HullStateBarFill->SetBackgroundColor(Color(1.0f - HullPercentage, HullPercentage, 0.0f, 1.0f));
}

void UI::SpaceDockWidget::_OnRechargeButtonClicked(void)
{
	assert(_Planet.IsValid() == true);
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != 0);
	_Planet->Recharge(_Character->GetShip(), _Character.Get());
}

void UI::SpaceDockWidget::_OnRechargeButtonUpdating(UI::Button * RechargeButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Character.IsValid() == true);
	assert(_Planet.IsValid() == true);
	RechargeButton->SetEnabled((_Planet->GetOffersRecharging() == true) && (_Character->GetShip() != 0) && (_Character->GetShip()->GetBattery() != 0) && (_Character->GetShip()->GetBattery()->GetEnergy() < _Character->GetShip()->GetBattery()->GetEnergyCapacity()));
}

void UI::SpaceDockWidget::_OnRefuelButtonClicked(void)
{
	assert(_Planet.IsValid() == true);
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != 0);
	_Planet->Refuel(_Character->GetShip(), _Character.Get());
}

void UI::SpaceDockWidget::_OnRefuelButtonUpdating(UI::Button * RefuelButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Character.IsValid() == true);
	assert(_Planet.IsValid() == true);
	
	const std::vector< PlanetAssetClass * > & PlanetAssetClasses(_Planet->GetPlanetAssetClasses());
	bool OffersRefueling(false);
	
	for(std::vector< PlanetAssetClass * >::const_iterator PlanetAssetClassIterator = PlanetAssetClasses.begin(); PlanetAssetClassIterator != PlanetAssetClasses.end(); ++PlanetAssetClassIterator)
	{
		if((*PlanetAssetClassIterator)->GetAssetClass()->GetIdentifier() == "fuel")
		{
			OffersRefueling = true;
			
			break;
		}
	}
	RefuelButton->SetEnabled((OffersRefueling == true) && (_Character->GetShip() != 0) && (_Character->GetShip()->GetFuel() < _Character->GetShip()->GetFuelCapacity()));
}

void UI::SpaceDockWidget::_OnRepairButtonClicked(void)
{
	assert(_Planet.IsValid() == true);
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != 0);
	_Planet->Repair(_Character->GetShip(), _Character.Get());
}

void UI::SpaceDockWidget::_OnRepairButtonUpdating(UI::Button * RepairButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Character.IsValid() == true);
	assert(_Planet.IsValid() == true);
	RepairButton->SetEnabled((_Planet->GetOffersRepairing() == true) && (_Character->GetShip() != 0) && (_Character->GetShip()->GetHull() < _Character->GetShip()->GetHullCapacity()));
}

void UI::SpaceDockWidget::_OnTakeOffButtonClicked(void)
{
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != 0);
	_Character->GetShip()->SetTakeOff(true);
}
