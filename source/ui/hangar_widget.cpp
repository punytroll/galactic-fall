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
#include "../color.h"
#include "../planet.h"
#include "../ship.h"
#include "../string_cast.h"
#include "hangar_widget.h"
#include "progress_bar.h"
#include "text_button.h"

UI::HangarWidget::HangarWidget(UI::Widget * SupWidget, Reference< Planet > Planet, Reference< Character > Character) :
	UI::Widget(SupWidget),
	_Character(Character),
	_Planet(Planet)
{
	assert(_Character.IsValid() == true);
	assert(_Planet.IsValid() == true);
	SetSize(Vector2f(600.0f, 300.0f));
	
	UI::Button * RepairButton(new UI::TextButton(this, "Repair"));
	
	RepairButton->SetPosition(Vector2f(0.0f, 0.0f));
	RepairButton->SetSize(Vector2f(180.0f, 20.0f));
	RepairButton->ConnectClickedCallback(Callback(this, &UI::HangarWidget::_OnRepairButtonClicked));
	RepairButton->ConnectUpdatingCallback(Bind1(Callback(this, &UI::HangarWidget::_OnRepairButtonUpdating), RepairButton));
	
	UI::ProgressBar * HullStateProgressBar(new UI::ProgressBar(this));
	
	HullStateProgressBar->SetPosition(Vector2f(190.0f, 0.0f));
	HullStateProgressBar->SetSize(Vector2f(410.0f, 20.0f));
	HullStateProgressBar->SetAnchorRight(true);
	HullStateProgressBar->ConnectUpdatingCallback(Bind1(Callback(this, &UI::HangarWidget::_OnHullStateProgressBarUpdating), HullStateProgressBar));
	
	UI::Button * RechargeButton(new UI::TextButton(this, "Recharge"));
	
	RechargeButton->SetPosition(Vector2f(0.0f, 30.0f));
	RechargeButton->SetSize(Vector2f(180.0f, 20.0f));
	RechargeButton->ConnectClickedCallback(Callback(this, &UI::HangarWidget::_OnRechargeButtonClicked));
	RechargeButton->ConnectUpdatingCallback(Bind1(Callback(this, &UI::HangarWidget::_OnRechargeButtonUpdating), RechargeButton));
	
	UI::ProgressBar * EnergyStateProgressBar(new UI::ProgressBar(this));
	
	EnergyStateProgressBar->SetPosition(Vector2f(190.0f, 30.0f));
	EnergyStateProgressBar->SetSize(Vector2f(410.0f, 20.0f));
	EnergyStateProgressBar->SetAnchorRight(true);
	EnergyStateProgressBar->ConnectUpdatingCallback(Bind1(Callback(this, &UI::HangarWidget::_OnEnergyStateProgressBarUpdating), EnergyStateProgressBar));
	
	UI::Button * RefuelButton(new UI::TextButton(this, "Refuel"));
	
	RefuelButton->SetPosition(Vector2f(0.0f, 60.0f));
	RefuelButton->SetSize(Vector2f(180.0f, 20.0f));
	RefuelButton->ConnectClickedCallback(Callback(this, &UI::HangarWidget::_OnRefuelButtonClicked));
	RefuelButton->ConnectUpdatingCallback(Bind1(Callback(this, &UI::HangarWidget::_OnRefuelButtonUpdating), RefuelButton));
	
	UI::ProgressBar * FuelStateProgressBar(new UI::ProgressBar(this));
	
	FuelStateProgressBar->SetPosition(Vector2f(190.0f, 60.0f));
	FuelStateProgressBar->SetSize(Vector2f(410.0f, 20.0f));
	FuelStateProgressBar->SetAnchorRight(true);
	FuelStateProgressBar->ConnectUpdatingCallback(Bind1(Callback(this, &UI::HangarWidget::_OnFuelStateProgressBarUpdating), FuelStateProgressBar));
	
	UI::Button * TakeOffButton(new UI::TextButton(this, "Take Off"));
	
	TakeOffButton->SetPosition(Vector2f(0.0f, 90.0f));
	TakeOffButton->SetSize(Vector2f(180.0f, 20.0f));
	TakeOffButton->ConnectClickedCallback(Callback(this, &UI::HangarWidget::_OnTakeOffButtonClicked));
}

void UI::HangarWidget::_OnEnergyStateProgressBarUpdating(UI::ProgressBar * EnergyStateProgressBar, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != nullptr);
	assert(EnergyStateProgressBar != nullptr);
	
	float EnergyPercentage(0.0f);
	
	if(_Character->GetShip()->GetBattery() != 0)
	{
		EnergyStateProgressBar->SetText(to_string_cast(_Character->GetShip()->GetBattery()->GetEnergy(), 2));
		EnergyPercentage = _Character->GetShip()->GetBattery()->GetEnergy() / _Character->GetShip()->GetBattery()->GetEnergyCapacity();
	}
	else
	{
		EnergyStateProgressBar->SetText("n/a");
	}
	EnergyStateProgressBar->SetFillLevel(EnergyPercentage);
	EnergyStateProgressBar->SetColor(Color(1.0f - EnergyPercentage, EnergyPercentage, 0.0f, 1.0f));
}

void UI::HangarWidget::_OnFuelStateProgressBarUpdating(UI::ProgressBar * FuelStateProgressBar, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != nullptr);
	assert(FuelStateProgressBar != nullptr);
	FuelStateProgressBar->SetText(to_string_cast(_Character->GetShip()->GetFuel(), 2));
	
	float FuelPercentage(_Character->GetShip()->GetFuel() / _Character->GetShip()->GetFuelCapacity());
	
	FuelStateProgressBar->SetFillLevel(FuelPercentage);
	FuelStateProgressBar->SetColor(Color(1.0f - FuelPercentage, FuelPercentage, 0.0f, 1.0f));
}

void UI::HangarWidget::_OnHullStateProgressBarUpdating(UI::ProgressBar * HullStateProgressBar, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != nullptr);
	assert(HullStateProgressBar != nullptr);
	HullStateProgressBar->SetText(to_string_cast(_Character->GetShip()->GetHull(), 2));
	
	float HullPercentage(_Character->GetShip()->GetHull() / _Character->GetShip()->GetHullCapacity());
	
	HullStateProgressBar->SetFillLevel(HullPercentage);
	HullStateProgressBar->SetColor(Color(1.0f - HullPercentage, HullPercentage, 0.0f, 1.0f));
}

void UI::HangarWidget::_OnRechargeButtonClicked(void)
{
	assert(_Planet.IsValid() == true);
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != 0);
	_Planet->Recharge(_Character->GetShip(), _Character.Get());
}

void UI::HangarWidget::_OnRechargeButtonUpdating(UI::Button * RechargeButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Character.IsValid() == true);
	assert(_Planet.IsValid() == true);
	RechargeButton->SetEnabled((_Planet->GetOffersRecharging() == true) && (_Character->GetShip() != 0) && (_Character->GetShip()->GetBattery() != 0) && (_Character->GetShip()->GetBattery()->GetEnergy() < _Character->GetShip()->GetBattery()->GetEnergyCapacity()));
}

void UI::HangarWidget::_OnRefuelButtonClicked(void)
{
	assert(_Planet.IsValid() == true);
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != 0);
	_Planet->Refuel(_Character->GetShip(), _Character.Get());
}

void UI::HangarWidget::_OnRefuelButtonUpdating(UI::Button * RefuelButton, float RealTimeSeconds, float GameTimeSeconds)
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

void UI::HangarWidget::_OnRepairButtonClicked(void)
{
	assert(_Planet.IsValid() == true);
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != 0);
	_Planet->Repair(_Character->GetShip(), _Character.Get());
}

void UI::HangarWidget::_OnRepairButtonUpdating(UI::Button * RepairButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Character.IsValid() == true);
	assert(_Planet.IsValid() == true);
	RepairButton->SetEnabled((_Planet->GetOffersRepairing() == true) && (_Character->GetShip() != 0) && (_Character->GetShip()->GetHull() < _Character->GetShip()->GetHullCapacity()));
}

void UI::HangarWidget::_OnTakeOffButtonClicked(void)
{
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != 0);
	_Character->GetShip()->SetTakeOff(true);
}