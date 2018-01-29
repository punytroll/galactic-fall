/**
 * galactic-fall
 * Copyright (C) 2015  Hagen M�bius
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

#include <expressions/operators.h>

#include <string_cast/string_cast.h>

#include "../battery.h"
#include "../character.h"
#include "../globals.h"
#include "../graphics/color_rgbo.h"
#include "../map_knowledge.h"
#include "../object_aspect_name.h"
#include "../output_observer.h"
#include "../real_time.h"
#include "../ship.h"
#include "../system.h"
#include "border.h"
#include "heads_up_display.h"
#include "label.h"
#include "mini_map_display.h"
#include "scanner_display.h"

using namespace Expressions::Operators;

int WantToJump(Ship * Ship, System * System);

UI::HeadsUpDisplay::HeadsUpDisplay(UI::Widget * SupWidget) :
	UI::Widget(SupWidget),
	_MessageLabel(nullptr)
{
	auto TimeWarpLabel{new UI::Label{this}};
	
	TimeWarpLabel->SetName("time_warp");
	TimeWarpLabel->SetLeft(0.0_c);
	TimeWarpLabel->SetTop(0.0_c);
	TimeWarpLabel->SetWidth(200.0_c);
	TimeWarpLabel->SetHeight(20.0f);
	TimeWarpLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	TimeWarpLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateTimeWarpLabel, this, TimeWarpLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto LinkedSystemTargetLabel{new UI::Label{this}};
	
	LinkedSystemTargetLabel->SetName("linked_system_target");
	LinkedSystemTargetLabel->SetLeft(0.0_c);
	LinkedSystemTargetLabel->SetTop(20.0_c);
	LinkedSystemTargetLabel->SetWidth(200.0_c);
	LinkedSystemTargetLabel->SetHeight(20.0f);
	LinkedSystemTargetLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	LinkedSystemTargetLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateLinkedSystemTargetLabel, this, LinkedSystemTargetLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto CreditsLabel{new UI::Label{this}};
	
	CreditsLabel->SetName("credits");
	CreditsLabel->SetLeft(0.0_c);
	CreditsLabel->SetTop(40.0_c);
	CreditsLabel->SetWidth(200.0_c);
	CreditsLabel->SetHeight(20.0f);
	CreditsLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	CreditsLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateCreditsLabel, this, CreditsLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto FuelLabel{new UI::Label{this}};
	
	FuelLabel->SetName("fuel");
	FuelLabel->SetLeft(0.0_c);
	FuelLabel->SetTop(60.0_c);
	FuelLabel->SetWidth(200.0_c);
	FuelLabel->SetHeight(20.0f);
	FuelLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	FuelLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateFuelLabel, this, FuelLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto EnergyLabel{new UI::Label{this}};
	
	EnergyLabel->SetName("energy");
	EnergyLabel->SetLeft(0.0_c);
	EnergyLabel->SetTop(80.0_c);
	EnergyLabel->SetWidth(200.0_c);
	EnergyLabel->SetHeight(20.0f);
	EnergyLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	EnergyLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateEnergyLabel, this, EnergyLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto HullLabel{new UI::Label{this}};
	
	HullLabel->SetName("hull");
	HullLabel->SetLeft(0.0_c);
	HullLabel->SetTop(100.0_c);
	HullLabel->SetWidth(200.0_c);
	HullLabel->SetHeight(20.0f);
	HullLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	HullLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateHullLabel, this, HullLabel, std::placeholders::_1, std::placeholders::_2));
	_MessageLabel = new UI::Label{this};
	_MessageLabel->SetName("message");
	_MessageLabel->SetLeft(0.0_c);
	_MessageLabel->SetTop(40.0_c);
	_MessageLabel->SetWidth(constant(GetWidth()));
	_MessageLabel->SetHeight(12.0f);
	_MessageLabel->SetAnchorRight(true);
	_MessageLabel->SetTextColor(Graphics::ColorRGBO(1.0f, 0.3f, 0.3f, 1.0f));
	_MessageLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Center);
	_MessageLabel->SetVisible(false);
	
	auto MiniMapWidget{new UI::Widget{this}};
	
	MiniMapWidget->SetName("mini_map");
	MiniMapWidget->SetLeft(constant(GetWidth() - 220.0f));
	MiniMapWidget->SetTop(constant(GetHeight() - 240.0f));
	MiniMapWidget->SetWidth(220.0_c);
	MiniMapWidget->SetHeight(240.0f);
	MiniMapWidget->SetBackgroundColor(Graphics::ColorRGBO(0.0f, 0.1f, 0.17f, 0.8f));
	MiniMapWidget->SetAnchorBottom(true);
	MiniMapWidget->SetAnchorLeft(false);
	MiniMapWidget->SetAnchorRight(true);
	MiniMapWidget->SetAnchorTop(false);
	MiniMapWidget->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateMiniMapWidget, this, MiniMapWidget, std::placeholders::_1, std::placeholders::_2));
	
	auto MiniMapSystemNameLabel{new UI::Label{MiniMapWidget}};
	
	MiniMapSystemNameLabel->SetName("system_name");
	MiniMapSystemNameLabel->SetWidth(constant(MiniMapWidget->GetWidth()));
	MiniMapSystemNameLabel->SetHeight(20.0f);
	MiniMapSystemNameLabel->SetAnchorRight(true);
	MiniMapSystemNameLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	MiniMapSystemNameLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Center);
	MiniMapSystemNameLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	MiniMapSystemNameLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateMiniMapSystemNameLabel, this, MiniMapSystemNameLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto MiniMapDisplay{new UI::MiniMapDisplay{MiniMapWidget}};
	
	MiniMapDisplay->SetName("display");
	MiniMapDisplay->SetLeft(0.0_c);
	MiniMapDisplay->SetTop(20.0_c);
	MiniMapDisplay->SetWidth(constant(MiniMapWidget->GetWidth()));
	MiniMapDisplay->SetHeight(MiniMapWidget->GetHeight() - 20.0f);
	MiniMapDisplay->SetAnchorBottom(true);
	MiniMapDisplay->SetAnchorRight(true);
	MiniMapDisplay->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateMiniMapDisplay, this, MiniMapDisplay, std::placeholders::_1, std::placeholders::_2));
	
	auto MiniMapBorder{new UI::Border{MiniMapWidget}};
	
	MiniMapBorder->SetName("border");
	MiniMapBorder->SetWidth(constant(MiniMapWidget->GetWidth()));
	MiniMapBorder->SetHeight(MiniMapWidget->GetHeight());
	MiniMapBorder->SetAnchorBottom(true);
	MiniMapBorder->SetAnchorRight(true);
	MiniMapBorder->SetLineWidth(1.0f);
	MiniMapBorder->SetColor(Graphics::ColorRGBO(0.1f, 0.2f, 0.3f, 1.0f));
	
	auto ScannerWidget{new UI::Widget{this}};
	
	ScannerWidget->SetName("scanner");
	ScannerWidget->SetLeft(0.0_c);
	ScannerWidget->SetTop(constant(GetHeight() - 240.0f));
	ScannerWidget->SetWidth(220.0_c);
	ScannerWidget->SetHeight(240.0f);
	ScannerWidget->SetBackgroundColor(Graphics::ColorRGBO(0.0f, 0.1f, 0.17f, 0.8f));
	ScannerWidget->SetAnchorBottom(true);
	ScannerWidget->SetAnchorTop(false);
	ScannerWidget->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateScannerWidget, this, ScannerWidget, std::placeholders::_1, std::placeholders::_2));
	
	auto ScannerTargetNameLabel{new UI::Label{ScannerWidget}};
	
	ScannerTargetNameLabel->SetName("target_name");
	ScannerTargetNameLabel->SetWidth(constant(ScannerWidget->GetWidth()));
	ScannerTargetNameLabel->SetHeight(20.0f);
	ScannerTargetNameLabel->SetAnchorRight(true);
	ScannerTargetNameLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	ScannerTargetNameLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Center);
	ScannerTargetNameLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	ScannerTargetNameLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateScannerTargetNameLabel, this, ScannerTargetNameLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto ScannerDisplay{new UI::ScannerDisplay{ScannerWidget}};
	
	ScannerDisplay->SetName("display");
	ScannerDisplay->SetLeft(0.0_c);
	ScannerDisplay->SetTop(20.0_c);
	ScannerDisplay->SetWidth(constant(ScannerWidget->GetWidth()));
	ScannerDisplay->SetHeight(ScannerWidget->GetHeight() - 20.0f);
	ScannerDisplay->SetAnchorBottom(true);
	ScannerDisplay->SetAnchorRight(true);
	ScannerDisplay->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateScannerDisplay, this, ScannerDisplay, std::placeholders::_1, std::placeholders::_2));
	
	auto ScannerBorder{new UI::Border{ScannerWidget}};
	
	ScannerBorder->SetName("border");
	ScannerBorder->SetWidth(constant(ScannerWidget->GetWidth()));
	ScannerBorder->SetHeight(ScannerWidget->GetHeight());
	ScannerBorder->SetAnchorBottom(true);
	ScannerBorder->SetAnchorRight(true);
	ScannerBorder->SetLineWidth(1.0f);
	ScannerBorder->SetColor(Graphics::ColorRGBO(0.1f, 0.2f, 0.3f, 1.0f));
}

void UI::HeadsUpDisplay::_HideMessage(void)
{
	assert(_MessageLabel);
	_MessageLabel->SetVisible(false);
}

void UI::HeadsUpDisplay::SetMessage(const std::string & Message)
{
	assert(_MessageLabel != nullptr);
	_MessageLabel->SetText(Message);
	_MessageLabel->SetVisible(true);
	if(_MessageTimeoutNotification.IsValid() == true)
	{
		_MessageTimeoutNotification.Dismiss();
	}
	_MessageTimeoutNotification = g_RealTimeTimeoutNotifications->Add(RealTime::Get() + 2.0f, std::bind(&UI::HeadsUpDisplay::_HideMessage, this));
}

void UI::HeadsUpDisplay::_UpdateCreditsLabel(UI::Label * CreditsLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(g_CharacterObserver != nullptr);
	if(g_CharacterObserver->GetObservedCharacter() != nullptr)
	{
		CreditsLabel->SetVisible(true);
		CreditsLabel->SetText("Credits: " + to_string_cast(g_CharacterObserver->GetObservedCharacter()->GetCredits()));
	}
	else
	{
		CreditsLabel->SetVisible(false);
	}
}

void UI::HeadsUpDisplay::_UpdateEnergyLabel(UI::Label * EnergyLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(g_CharacterObserver != nullptr);
	if((g_CharacterObserver->GetObservedCharacter() != nullptr) && (g_CharacterObserver->GetObservedCharacter()->GetShip() != nullptr) && (g_CharacterObserver->GetObservedCharacter()->GetShip()->GetBattery() != nullptr))
	{
		EnergyLabel->SetVisible(true);
		EnergyLabel->SetText("Energy: " + to_string_cast(g_CharacterObserver->GetObservedCharacter()->GetShip()->GetBattery()->GetEnergy(), 2));
	}
	else
	{
		EnergyLabel->SetVisible(false);
	}
}

void UI::HeadsUpDisplay::_UpdateFuelLabel(UI::Label * FuelLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(g_CharacterObserver != nullptr);
	if((g_CharacterObserver->GetObservedCharacter() != nullptr) && (g_CharacterObserver->GetObservedCharacter()->GetShip() != nullptr))
	{
		FuelLabel->SetVisible(true);
		FuelLabel->SetText("Fuel: " + to_string_cast(g_CharacterObserver->GetObservedCharacter()->GetShip()->GetFuel(), 2));
	}
	else
	{
		FuelLabel->SetVisible(false);
	}
}

void UI::HeadsUpDisplay::_UpdateHullLabel(UI::Label * HullLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(g_CharacterObserver != nullptr);
	if((g_CharacterObserver->GetObservedCharacter() != nullptr) && (g_CharacterObserver->GetObservedCharacter()->GetShip() != nullptr))
	{
		HullLabel->SetVisible(true);
		HullLabel->SetText("Hull: " + to_string_cast(g_CharacterObserver->GetObservedCharacter()->GetShip()->GetHull(), 2));
	}
	else
	{
		HullLabel->SetVisible(false);
	}
}

void UI::HeadsUpDisplay::_UpdateLinkedSystemTargetLabel(UI::Label * LinkedSystemTargetLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(g_CharacterObserver != nullptr);
	if((g_CharacterObserver->GetObservedCharacter() != nullptr) && (g_CharacterObserver->GetObservedCharacter()->GetShip() != nullptr) && (g_CharacterObserver->GetObservedCharacter()->GetShip()->GetLinkedSystemTarget() != nullptr))
	{
		LinkedSystemTargetLabel->SetVisible(true);
		assert(g_CharacterObserver->GetObservedCharacter()->GetMapKnowledge() != nullptr);
		
		const std::set< System * > UnexploredSystems(g_CharacterObserver->GetObservedCharacter()->GetMapKnowledge()->GetUnexploredSystems());
		
		if(UnexploredSystems.find(g_CharacterObserver->GetObservedCharacter()->GetShip()->GetLinkedSystemTarget()) == UnexploredSystems.end())
		{
			LinkedSystemTargetLabel->SetText(g_CharacterObserver->GetObservedCharacter()->GetShip()->GetLinkedSystemTarget()->GetAspectName()->GetName());
		}
		else
		{
			LinkedSystemTargetLabel->SetText("Unknown System");
		}
		if(WantToJump(g_CharacterObserver->GetObservedCharacter()->GetShip(), g_CharacterObserver->GetObservedCharacter()->GetShip()->GetLinkedSystemTarget()) == OK)
		{
			LinkedSystemTargetLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
		}
		else
		{
			LinkedSystemTargetLabel->SetTextColor(Graphics::ColorRGBO(0.4f, 0.4f, 0.4f, 1.0f));
		}
	}
	else
	{
		LinkedSystemTargetLabel->SetVisible(false);
	}
}

void UI::HeadsUpDisplay::_UpdateMiniMapSystemNameLabel(UI::Label * MiniMapSystemNameLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(g_CharacterObserver != nullptr);
	if((g_CharacterObserver->GetObservedCharacter() != nullptr) && (g_CharacterObserver->GetObservedCharacter()->GetShip() != nullptr))
	{
		MiniMapSystemNameLabel->SetVisible(true);
		assert(g_CharacterObserver->GetObservedCharacter()->GetShip()->GetSystem() != nullptr);
		assert(g_CharacterObserver->GetObservedCharacter()->GetShip()->GetSystem()->GetAspectName() != nullptr);
		MiniMapSystemNameLabel->SetText(g_CharacterObserver->GetObservedCharacter()->GetShip()->GetSystem()->GetAspectName()->GetName());
	}
	else
	{
		MiniMapSystemNameLabel->SetVisible(false);
	}
}

void UI::HeadsUpDisplay::_UpdateMiniMapDisplay(UI::MiniMapDisplay * MiniMapDisplay, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(g_CharacterObserver != nullptr);
	if((g_CharacterObserver->GetObservedCharacter() != nullptr) && (g_CharacterObserver->GetObservedCharacter()->GetShip() != nullptr))
	{
		MiniMapDisplay->SetVisible(true);
		MiniMapDisplay->SetCharacter(g_CharacterObserver->GetObservedCharacter());
	}
	else
	{
		MiniMapDisplay->SetVisible(false);
	}
}

void UI::HeadsUpDisplay::_UpdateMiniMapWidget(UI::Widget * MiniMapWidget, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(g_CharacterObserver != nullptr);
	if(g_CharacterObserver->GetObservedCharacter() != nullptr)
	{
		MiniMapWidget->SetVisible(true);
	}
	else
	{
		MiniMapWidget->SetVisible(false);
	}
}

void UI::HeadsUpDisplay::_UpdateScannerDisplay(UI::ScannerDisplay * ScannerDisplay, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(g_CharacterObserver != nullptr);
	if((g_CharacterObserver->GetObservedCharacter() != nullptr) && (g_CharacterObserver->GetObservedCharacter()->GetShip() != nullptr) && (g_CharacterObserver->GetObservedCharacter()->GetShip()->GetTarget() != nullptr))
	{
		ScannerDisplay->SetVisible(true);
		ScannerDisplay->SetTarget(g_CharacterObserver->GetObservedCharacter()->GetShip()->GetTarget());
	}
	else
	{
		ScannerDisplay->SetVisible(false);
	}
}

void UI::HeadsUpDisplay::_UpdateScannerTargetNameLabel(UI::Label * ScannerTargetNameLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(g_CharacterObserver != nullptr);
	if((g_CharacterObserver->GetObservedCharacter() != nullptr) && (g_CharacterObserver->GetObservedCharacter()->GetShip() != nullptr) && (g_CharacterObserver->GetObservedCharacter()->GetShip()->GetTarget() != nullptr))
	{
		ScannerTargetNameLabel->SetVisible(true);
		assert(g_CharacterObserver->GetObservedCharacter()->GetShip()->GetTarget()->GetAspectName() != nullptr);
		ScannerTargetNameLabel->SetText(g_CharacterObserver->GetObservedCharacter()->GetShip()->GetTarget()->GetAspectName()->GetName());
	}
	else
	{
		ScannerTargetNameLabel->SetVisible(false);
	}
}

void UI::HeadsUpDisplay::_UpdateScannerWidget(UI::Widget * ScannerWidget, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(g_CharacterObserver != nullptr);
	if(g_CharacterObserver->GetObservedCharacter() != nullptr)
	{
		ScannerWidget->SetVisible(true);
	}
	else
	{
		ScannerWidget->SetVisible(false);
	}
}

void UI::HeadsUpDisplay::_UpdateTimeWarpLabel(UI::Label * TimeWarpLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	if(g_Galaxy != nullptr)
	{
		TimeWarpLabel->SetVisible(true);
		TimeWarpLabel->SetText("Time Warp: " + to_string_cast(g_TimeWarp, 2));
	}
	else
	{
		TimeWarpLabel->SetVisible(false);
	}
}
