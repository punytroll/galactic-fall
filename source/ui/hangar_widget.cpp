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
#include "../character.h"
#include "../color.h"
#include "../planet.h"
#include "../ship.h"
#include "../string_cast.h"
#include "event.h"
#include "hangar_widget.h"
#include "load_ship_window.h"
#include "outfit_ship_dialog.h"
#include "progress_bar.h"
#include "text_button.h"

UI::HangarWidget::HangarWidget(UI::Widget * SupWidget, Reference< Planet > Planet, Reference< Character > Character) :
	UI::Widget(SupWidget),
	_Character(Character),
	_LoadShipWindow(nullptr),
	_OutfitShipDialog(nullptr),
	_Planet(Planet)
{
	assert(_Character.IsValid() == true);
	assert(_Planet.IsValid() == true);
	ConnectDestroyingCallback(std::bind(&UI::HangarWidget::_OnDestroying, this, std::placeholders::_1));
	SetSize(Vector2f(600.0f, 300.0f));
	
	UI::Button * LoadButton(new UI::TextButton(this, "Load"));
	
	LoadButton->SetPosition(Vector2f(0.0f, 0.0f));
	LoadButton->SetSize(Vector2f(180.0f, 20.0f));
	LoadButton->ConnectClickedCallback(std::bind(&UI::HangarWidget::_OnLoadButtonClicked, this));
	LoadButton->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnLoadButtonUpdating, this, LoadButton, std::placeholders::_1, std::placeholders::_2));
	
	UI::Button * OutfitButton(new UI::TextButton(this, "Outfit"));
	
	OutfitButton->SetPosition(Vector2f(0.0f, 30.0f));
	OutfitButton->SetSize(Vector2f(180.0f, 20.0f));
	OutfitButton->ConnectClickedCallback(std::bind(&UI::HangarWidget::_OnOutfitButtonClicked, this));
	OutfitButton->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnOutfitButtonUpdating, this, OutfitButton, std::placeholders::_1, std::placeholders::_2));
	
	UI::Button * RepairButton(new UI::TextButton(this, "Repair"));
	
	RepairButton->SetPosition(Vector2f(0.0f, 60.0f));
	RepairButton->SetSize(Vector2f(180.0f, 20.0f));
	RepairButton->ConnectClickedCallback(std::bind(&UI::HangarWidget::_OnRepairButtonClicked, this));
	RepairButton->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnRepairButtonUpdating, this, RepairButton, std::placeholders::_1, std::placeholders::_2));
	
	UI::ProgressBar * HullStateProgressBar(new UI::ProgressBar(this));
	
	HullStateProgressBar->SetPosition(Vector2f(190.0f, 60.0f));
	HullStateProgressBar->SetSize(Vector2f(410.0f, 20.0f));
	HullStateProgressBar->SetAnchorRight(true);
	HullStateProgressBar->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnHullStateProgressBarUpdating, this, HullStateProgressBar, std::placeholders::_1, std::placeholders::_2));
	
	UI::Button * RechargeButton(new UI::TextButton(this, "Recharge"));
	
	RechargeButton->SetPosition(Vector2f(0.0f, 90.0f));
	RechargeButton->SetSize(Vector2f(180.0f, 20.0f));
	RechargeButton->ConnectClickedCallback(std::bind(&UI::HangarWidget::_OnRechargeButtonClicked, this));
	RechargeButton->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnRechargeButtonUpdating, this, RechargeButton, std::placeholders::_1, std::placeholders::_2));
	
	UI::ProgressBar * EnergyStateProgressBar(new UI::ProgressBar(this));
	
	EnergyStateProgressBar->SetPosition(Vector2f(190.0f, 90.0f));
	EnergyStateProgressBar->SetSize(Vector2f(410.0f, 20.0f));
	EnergyStateProgressBar->SetAnchorRight(true);
	EnergyStateProgressBar->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnEnergyStateProgressBarUpdating, this, EnergyStateProgressBar, std::placeholders::_1, std::placeholders::_2));
	
	UI::Button * RefuelButton(new UI::TextButton(this, "Refuel"));
	
	RefuelButton->SetPosition(Vector2f(0.0f, 120.0f));
	RefuelButton->SetSize(Vector2f(180.0f, 20.0f));
	RefuelButton->ConnectClickedCallback(std::bind(&UI::HangarWidget::_OnRefuelButtonClicked, this));
	RefuelButton->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnRefuelButtonUpdating, this, RefuelButton, std::placeholders::_1, std::placeholders::_2));
	
	UI::ProgressBar * FuelStateProgressBar(new UI::ProgressBar(this));
	
	FuelStateProgressBar->SetPosition(Vector2f(190.0f, 120.0f));
	FuelStateProgressBar->SetSize(Vector2f(410.0f, 20.0f));
	FuelStateProgressBar->SetAnchorRight(true);
	FuelStateProgressBar->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnFuelStateProgressBarUpdating, this, FuelStateProgressBar, std::placeholders::_1, std::placeholders::_2));
	
	UI::Button * TakeOffButton(new UI::TextButton(this, "Take Off"));
	
	TakeOffButton->SetPosition(Vector2f(0.0f, 150.0f));
	TakeOffButton->SetSize(Vector2f(180.0f, 20.0f));
	TakeOffButton->ConnectClickedCallback(std::bind(&UI::HangarWidget::_OnTakeOffButtonClicked, this));
	TakeOffButton->ConnectUpdatingCallback(std::bind(&UI::HangarWidget::_OnTakeOffButtonUpdating, this, TakeOffButton, std::placeholders::_1, std::placeholders::_2));
}

void UI::HangarWidget::_OnDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		if(_OutfitShipDialog != nullptr)
		{
			_OutfitShipDialog->Destroy();
		}
	}
	if(_LoadShipWindow != nullptr)
	{
		_LoadShipWindow->Destroy();
	}
}

void UI::HangarWidget::_OnEnergyStateProgressBarUpdating(UI::ProgressBar * EnergyStateProgressBar, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(EnergyStateProgressBar != nullptr);
	assert(_Character.IsValid() == true);
	
	float EnergyPercentage(0.0f);
	
	if((_Character->GetShip() != nullptr) && (_Character->GetShip()->GetBattery() != nullptr))
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
	assert(FuelStateProgressBar != nullptr);
	assert(_Character.IsValid() == true);
	
	float FuelPercentage(0.0f);
	
	if(_Character->GetShip() != nullptr)
	{
		FuelStateProgressBar->SetText(to_string_cast(_Character->GetShip()->GetFuel(), 2));
		FuelPercentage = _Character->GetShip()->GetFuel() / _Character->GetShip()->GetFuelCapacity();
	}
	else
	{
		FuelStateProgressBar->SetText("n/a");
	}
	FuelStateProgressBar->SetFillLevel(FuelPercentage);
	FuelStateProgressBar->SetColor(Color(1.0f - FuelPercentage, FuelPercentage, 0.0f, 1.0f));
}

void UI::HangarWidget::_OnHullStateProgressBarUpdating(UI::ProgressBar * HullStateProgressBar, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(HullStateProgressBar != nullptr);
	assert(_Character.IsValid() == true);
	
	float HullPercentage(0.0f);
	
	if(_Character->GetShip() != nullptr)
	{
		HullStateProgressBar->SetText(to_string_cast(_Character->GetShip()->GetHull(), 2));
		HullPercentage = _Character->GetShip()->GetHull() / _Character->GetShip()->GetHullCapacity();
	}
	else
	{
		HullStateProgressBar->SetText("n/a");
	}
	HullStateProgressBar->SetFillLevel(HullPercentage);
	HullStateProgressBar->SetColor(Color(1.0f - HullPercentage, HullPercentage, 0.0f, 1.0f));
}

void UI::HangarWidget::_OnLoadButtonClicked(void)
{
	if(_LoadShipWindow == nullptr)
	{
		assert(_Character.IsValid() == true);
		assert(_Character->GetShip() != nullptr);
		_LoadShipWindow = new UI::LoadShipWindow(GetRootWidget(), _Planet->GetHangar(_Character.Get()), _Character->GetShip());
		_LoadShipWindow->ConnectDestroyingCallback(std::bind(&UI::HangarWidget::_OnLoadShipWindowDestroying, this, std::placeholders::_1));
		_LoadShipWindow->GrabKeyFocus();
	}
	else
	{
		GetRootWidget()->RaiseSubWidget(_LoadShipWindow);
	}
}

void UI::HangarWidget::_OnLoadButtonUpdating(UI::Button * LoadButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(LoadButton != nullptr);
	assert(_Character.IsValid());
	LoadButton->SetEnabled(_Character->GetShip() != nullptr);
}

void UI::HangarWidget::_OnLoadShipWindowDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		_LoadShipWindow = nullptr;
	}
}

void UI::HangarWidget::_OnOutfitButtonClicked(void)
{
	if(_OutfitShipDialog == nullptr)
	{
		assert(_Character.IsValid() == true);
		assert(_Character->GetShip() != nullptr);
		_OutfitShipDialog = new UI::OutfitShipDialog(GetRootWidget(), _Character->GetShip()->GetReference());
		_OutfitShipDialog->ConnectDestroyingCallback(std::bind(&UI::HangarWidget::_OnOutfitShipDialogDestroying, this, std::placeholders::_1));
	}
	else
	{
		GetRootWidget()->RaiseSubWidget(_OutfitShipDialog);
	}
}

void UI::HangarWidget::_OnOutfitButtonUpdating(UI::Button * OutfitButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(OutfitButton != nullptr);
	assert(_Character.IsValid());
	OutfitButton->SetEnabled(_Character->GetShip() != nullptr);
}

void UI::HangarWidget::_OnOutfitShipDialogDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		_OutfitShipDialog = nullptr;
	}
}

void UI::HangarWidget::_OnRechargeButtonClicked(void)
{
	assert(_Planet.IsValid() == true);
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != nullptr);
	_Planet->Recharge(_Character->GetShip(), _Character.Get());
}

void UI::HangarWidget::_OnRechargeButtonUpdating(UI::Button * RechargeButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Character.IsValid() == true);
	assert(_Planet.IsValid() == true);
	RechargeButton->SetEnabled((_Planet->GetOffersRecharging() == true) && (_Character->GetShip() != nullptr) && (_Character->GetShip()->GetBattery() != nullptr) && (_Character->GetShip()->GetBattery()->GetEnergy() < _Character->GetShip()->GetBattery()->GetEnergyCapacity()));
}

void UI::HangarWidget::_OnRefuelButtonClicked(void)
{
	assert(_Planet.IsValid() == true);
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != nullptr);
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
	RefuelButton->SetEnabled((OffersRefueling == true) && (_Character->GetShip() != nullptr) && (_Character->GetShip()->GetFuel() < _Character->GetShip()->GetFuelCapacity()));
}

void UI::HangarWidget::_OnRepairButtonClicked(void)
{
	assert(_Planet.IsValid() == true);
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != nullptr);
	_Planet->Repair(_Character->GetShip(), _Character.Get());
}

void UI::HangarWidget::_OnRepairButtonUpdating(UI::Button * RepairButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_Character.IsValid() == true);
	assert(_Planet.IsValid() == true);
	RepairButton->SetEnabled((_Planet->GetOffersRepairing() == true) && (_Character->GetShip() != nullptr) && (_Character->GetShip()->GetHull() < _Character->GetShip()->GetHullCapacity()));
}

void UI::HangarWidget::_OnTakeOffButtonClicked(void)
{
	assert(_Character.IsValid() == true);
	assert(_Character->GetShip() != nullptr);
	_Character->GetShip()->SetTakeOff(true);
}

void UI::HangarWidget::_OnTakeOffButtonUpdating(UI::Button * TakeOffButton, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(TakeOffButton != nullptr);
	assert(_Character.IsValid());
	TakeOffButton->SetEnabled(_Character->GetShip() != nullptr);
}
