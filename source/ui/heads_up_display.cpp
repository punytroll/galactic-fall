/**
 * galactic-fall
 * Copyright (C) 2015  Hagen Möbius
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

int WantToJump(Ship * Ship, System * System);

UI::HeadsUpDisplay::HeadsUpDisplay(UI::Widget * SupWidget) :
	UI::Widget(SupWidget),
	_MessageLabel(nullptr)
{
	SetSize(Vector2f(1000.0f, 1000.0f));
	
	auto TimeWarpLabel(new UI::Label(this));
	
	TimeWarpLabel->SetName("time_warp");
	TimeWarpLabel->SetPosition(Vector2f(0.0f, 0.0f));
	TimeWarpLabel->SetSize(Vector2f(200.0f, 20.0f));
	TimeWarpLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	TimeWarpLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateTimeWarpLabel, this, TimeWarpLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto LinkedSystemTargetLabel(new UI::Label(this));
	
	LinkedSystemTargetLabel->SetName("linked_system_target");
	LinkedSystemTargetLabel->SetPosition(Vector2f(0.0f, 20.0f));
	LinkedSystemTargetLabel->SetSize(Vector2f(200.0f, 20.0f));
	LinkedSystemTargetLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	LinkedSystemTargetLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateLinkedSystemTargetLabel, this, LinkedSystemTargetLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto CreditsLabel(new UI::Label(this));
	
	CreditsLabel->SetName("credits");
	CreditsLabel->SetPosition(Vector2f(0.0f, 40.0f));
	CreditsLabel->SetSize(Vector2f(200.0f, 20.0f));
	CreditsLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	CreditsLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateCreditsLabel, this, CreditsLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto FuelLabel(new UI::Label(this));
	
	FuelLabel->SetName("fuel");
	FuelLabel->SetPosition(Vector2f(0.0f, 60.0f));
	FuelLabel->SetSize(Vector2f(200.0f, 20.0f));
	FuelLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	FuelLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateFuelLabel, this, FuelLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto EnergyLabel(new UI::Label(this));
	
	EnergyLabel->SetName("energy");
	EnergyLabel->SetPosition(Vector2f(0.0f, 80.0f));
	EnergyLabel->SetSize(Vector2f(200.0f, 20.0f));
	EnergyLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	EnergyLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateEnergyLabel, this, EnergyLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto HullLabel(new UI::Label(this));
	
	HullLabel->SetName("hull");
	HullLabel->SetPosition(Vector2f(0.0f, 100.0f));
	HullLabel->SetSize(Vector2f(200.0f, 20.0f));
	HullLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	HullLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateHullLabel, this, HullLabel, std::placeholders::_1, std::placeholders::_2));
	_MessageLabel = new UI::Label(this);
	_MessageLabel->SetName("message");
	_MessageLabel->SetPosition(Vector2f(0.0f, 40.0f));
	_MessageLabel->SetSize(Vector2f(GetSize()[0], 12.0f));
	_MessageLabel->SetAnchorRight(true);
	_MessageLabel->SetTextColor(Graphics::ColorRGBO(1.0f, 0.3f, 0.3f, 1.0f));
	_MessageLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	_MessageLabel->SetVisible(false);
	
	auto MiniMapWidget(new UI::Widget(this));
	
	MiniMapWidget->SetName("mini_map");
	MiniMapWidget->SetSize(Vector2f(220.0f, 240.0f));
	MiniMapWidget->SetPosition(GetSize() - MiniMapWidget->GetSize());
	MiniMapWidget->SetBackgroundColor(Graphics::ColorRGBO(0.0f, 0.1f, 0.17f, 0.8f));
	MiniMapWidget->SetAnchorBottom(true);
	MiniMapWidget->SetAnchorLeft(false);
	MiniMapWidget->SetAnchorRight(true);
	MiniMapWidget->SetAnchorTop(false);
	MiniMapWidget->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateMiniMapWidget, this, MiniMapWidget, std::placeholders::_1, std::placeholders::_2));
	
	auto MiniMapSystemNameLabel(new UI::Label(MiniMapWidget));
	
	MiniMapSystemNameLabel->SetName("system_name");
	MiniMapSystemNameLabel->SetSize(Vector2f(MiniMapWidget->GetSize()[0], 20.0f));
	MiniMapSystemNameLabel->SetAnchorRight(true);
	MiniMapSystemNameLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	MiniMapSystemNameLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	MiniMapSystemNameLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	MiniMapSystemNameLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateMiniMapSystemNameLabel, this, MiniMapSystemNameLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto MiniMapDisplay(new UI::MiniMapDisplay(MiniMapWidget));
	
	MiniMapDisplay->SetName("display");
	MiniMapDisplay->SetPosition(Vector2f(0.0f, 20.0f));
	MiniMapDisplay->SetSize(Vector2f(MiniMapWidget->GetSize()[0], MiniMapWidget->GetSize()[1] - MiniMapDisplay->GetSize()[1]));
	MiniMapDisplay->SetAnchorBottom(true);
	MiniMapDisplay->SetAnchorRight(true);
	MiniMapDisplay->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateMiniMapDisplay, this, MiniMapDisplay, std::placeholders::_1, std::placeholders::_2));
	
	auto MiniMapBorder(new UI::Border(MiniMapWidget));
	
	MiniMapBorder->SetName("border");
	MiniMapBorder->SetSize(MiniMapWidget->GetSize());
	MiniMapBorder->SetAnchorBottom(true);
	MiniMapBorder->SetAnchorRight(true);
	MiniMapBorder->SetWidth(1.0f);
	MiniMapBorder->SetColor(Graphics::ColorRGBO(0.1f, 0.2f, 0.3f, 1.0f));
	
	auto ScannerWidget(new UI::Widget(this));
	
	ScannerWidget->SetName("scanner");
	ScannerWidget->SetSize(Vector2f(220.0f, 240.0f));
	ScannerWidget->SetPosition(Vector2f(0.0f, GetSize()[1] - ScannerWidget->GetSize()[1]));
	ScannerWidget->SetBackgroundColor(Graphics::ColorRGBO(0.0f, 0.1f, 0.17f, 0.8f));
	ScannerWidget->SetAnchorBottom(true);
	ScannerWidget->SetAnchorTop(false);
	ScannerWidget->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateScannerWidget, this, ScannerWidget, std::placeholders::_1, std::placeholders::_2));
	
	auto ScannerTargetNameLabel(new UI::Label(ScannerWidget));
	
	ScannerTargetNameLabel->SetName("target_name");
	ScannerTargetNameLabel->SetSize(Vector2f(ScannerWidget->GetSize()[0], 20.0f));
	ScannerTargetNameLabel->SetAnchorRight(true);
	ScannerTargetNameLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	ScannerTargetNameLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	ScannerTargetNameLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	ScannerTargetNameLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateScannerTargetNameLabel, this, ScannerTargetNameLabel, std::placeholders::_1, std::placeholders::_2));
	
	auto ScannerDisplay(new UI::ScannerDisplay(ScannerWidget));
	
	ScannerDisplay->SetName("display");
	ScannerDisplay->SetPosition(Vector2f(0.0f, 20.0f));
	ScannerDisplay->SetSize(Vector2f(ScannerWidget->GetSize()[0], ScannerWidget->GetSize()[1] - ScannerDisplay->GetSize()[1]));
	ScannerDisplay->SetAnchorBottom(true);
	ScannerDisplay->SetAnchorRight(true);
	ScannerDisplay->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateScannerDisplay, this, ScannerDisplay, std::placeholders::_1, std::placeholders::_2));
	
	auto ScannerBorder(new UI::Border(ScannerWidget));
	
	ScannerBorder->SetName("border");
	ScannerBorder->SetSize(ScannerWidget->GetSize());
	ScannerBorder->SetAnchorBottom(true);
	ScannerBorder->SetAnchorRight(true);
	ScannerBorder->SetWidth(1.0f);
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
	if(g_CharacterObserver->GetObservedCharacter() != nullptr)
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
