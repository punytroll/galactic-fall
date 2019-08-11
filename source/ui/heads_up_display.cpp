/**
 * galactic-fall
 * Copyright (C) 2015-2019  Hagen Möbius
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

#include <graphics/color_rgbo.h>

#include <string_cast/string_cast.h>

#include "../battery.h"
#include "../character.h"
#include "../game_time.h"
#include "../globals.h"
#include "../map_knowledge.h"
#include "../object_aspect_name.h"
#include "../object_aspect_object_container.h"
#include "../output_observer.h"
#include "../real_time.h"
#include "../ship.h"
#include "../system.h"
#include "border.h"
#include "heads_up_display.h"
#include "label.h"
#include "mini_map_display.h"
#include "progress_bar.h"
#include "scanner_display.h"

using namespace Expressions::Operators;

int WantToJump(Ship * Ship, System * System);

UI::HeadsUpDisplay::HeadsUpDisplay(void) :
	_MessageLabel(nullptr)
{
	// create components
	auto CreditsLabel{new UI::Label{}};
	auto CycleLabel{new UI::Label{}};
	auto CycleProgressBar{new UI::ProgressBar{}};
	auto EnergyLabel{new UI::Label{}};
	auto FuelLabel{new UI::Label{}};
	auto HullLabel{new UI::Label{}};
	auto LinkedSystemTargetLabel{new UI::Label{}};
	auto MiniMapBorder{new UI::Border{}};
	auto MiniMapDisplay{new UI::MiniMapDisplay{}};
	auto MiniMapSystemNameLabel{new UI::Label{}};
	auto MiniMapWidget{new UI::Widget{}};
	auto ScannerBorder{new UI::Border{}};
	auto ScannerDisplay{new UI::ScannerDisplay{}};
	auto ScannerTargetNameLabel{new UI::Label{}};
	auto ScannerWidget{new UI::Widget{}};
	auto TimeWarpLabel{new UI::Label{}};
	
	_MessageLabel = new UI::Label{};
	// initialize components
	CycleLabel->SetName("cycle_label");
	CycleLabel->SetLeft(0.0_c);
	CycleLabel->SetTop(0.0_c);
	CycleLabel->SetWidth(40.0_c);
	CycleLabel->SetHeight(20.0_c);
	CycleLabel->SetText("Cycle:");
	CycleLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	CycleLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	CycleProgressBar->SetName("cycle_progress_bar");
	CycleProgressBar->SetLeft(right(CycleLabel) + 10.0_c);
	CycleProgressBar->SetTop(top(CycleLabel) + 3.0_c);
	CycleProgressBar->SetWidth(100.0_c);
	CycleProgressBar->SetHeight(height(CycleLabel) - 2.0_c * top(CycleProgressBar));
	CycleProgressBar->SetColor(Graphics::ColorRGBO(0.3f, 0.45f, 0.6f, 1.0f));
	CycleProgressBar->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateCycleProgressBar, this, CycleProgressBar, std::placeholders::_1, std::placeholders::_2));
	TimeWarpLabel->SetName("time_warp");
	TimeWarpLabel->SetLeft(right(CycleProgressBar) + 10.0_c);
	TimeWarpLabel->SetTop(top(CycleLabel));
	TimeWarpLabel->SetWidth(200.0_c);
	TimeWarpLabel->SetHeight(height(CycleLabel));
	TimeWarpLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	TimeWarpLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	TimeWarpLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateTimeWarpLabel, this, TimeWarpLabel, std::placeholders::_1, std::placeholders::_2));
	LinkedSystemTargetLabel->SetName("linked_system_target");
	LinkedSystemTargetLabel->SetLeft(0.0_c);
	LinkedSystemTargetLabel->SetTop(20.0_c);
	LinkedSystemTargetLabel->SetWidth(200.0_c);
	LinkedSystemTargetLabel->SetHeight(20.0_c);
	LinkedSystemTargetLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	LinkedSystemTargetLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateLinkedSystemTargetLabel, this, LinkedSystemTargetLabel, std::placeholders::_1, std::placeholders::_2));
	LinkedSystemTargetLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	CreditsLabel->SetName("credits");
	CreditsLabel->SetLeft(0.0_c);
	CreditsLabel->SetTop(40.0_c);
	CreditsLabel->SetWidth(200.0_c);
	CreditsLabel->SetHeight(20.0_c);
	CreditsLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	CreditsLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateCreditsLabel, this, CreditsLabel, std::placeholders::_1, std::placeholders::_2));
	CreditsLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	FuelLabel->SetName("fuel");
	FuelLabel->SetLeft(0.0_c);
	FuelLabel->SetTop(60.0_c);
	FuelLabel->SetWidth(200.0_c);
	FuelLabel->SetHeight(20.0_c);
	FuelLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	FuelLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateFuelLabel, this, FuelLabel, std::placeholders::_1, std::placeholders::_2));
	FuelLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	EnergyLabel->SetName("energy");
	EnergyLabel->SetLeft(0.0_c);
	EnergyLabel->SetTop(80.0_c);
	EnergyLabel->SetWidth(200.0_c);
	EnergyLabel->SetHeight(20.0_c);
	EnergyLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	EnergyLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateEnergyLabel, this, EnergyLabel, std::placeholders::_1, std::placeholders::_2));
	EnergyLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	HullLabel->SetName("hull");
	HullLabel->SetLeft(0.0_c);
	HullLabel->SetTop(100.0_c);
	HullLabel->SetWidth(200.0_c);
	HullLabel->SetHeight(20.0_c);
	HullLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	HullLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateHullLabel, this, HullLabel, std::placeholders::_1, std::placeholders::_2));
	HullLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	_MessageLabel->SetName("message");
	_MessageLabel->SetLeft(0.0_c);
	_MessageLabel->SetTop(40.0_c);
	_MessageLabel->SetWidth(width(this));
	_MessageLabel->SetHeight(12.0_c);
	_MessageLabel->SetTextColor(Graphics::ColorRGBO(1.0f, 0.3f, 0.3f, 1.0f));
	_MessageLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Center);
	_MessageLabel->SetVisible(false);
	MiniMapWidget->SetName("mini_map");
	MiniMapWidget->SetLeft(width(this) - width(MiniMapWidget));
	MiniMapWidget->SetTop(height(this) - height(MiniMapWidget));
	MiniMapWidget->SetWidth(220.0_c);
	MiniMapWidget->SetHeight(240.0_c);
	MiniMapWidget->SetBackgroundColor(Graphics::ColorRGBO(0.0f, 0.1f, 0.17f, 0.8f));
	MiniMapWidget->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateMiniMapWidget, this, MiniMapWidget, std::placeholders::_1, std::placeholders::_2));
	MiniMapSystemNameLabel->SetName("system_name");
	MiniMapSystemNameLabel->SetLeft(0.0_c);
	MiniMapSystemNameLabel->SetTop(0.0_c);
	MiniMapSystemNameLabel->SetWidth(width(MiniMapWidget));
	MiniMapSystemNameLabel->SetHeight(20.0_c);
	MiniMapSystemNameLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	MiniMapSystemNameLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Center);
	MiniMapSystemNameLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	MiniMapSystemNameLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateMiniMapSystemNameLabel, this, MiniMapSystemNameLabel, std::placeholders::_1, std::placeholders::_2));
	MiniMapDisplay->SetName("display");
	MiniMapDisplay->SetLeft(0.0_c);
	MiniMapDisplay->SetTop(bottom(MiniMapSystemNameLabel));
	MiniMapDisplay->SetWidth(width(MiniMapWidget));
	MiniMapDisplay->SetHeight(height(MiniMapWidget) - top(MiniMapDisplay));
	MiniMapDisplay->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateMiniMapDisplay, this, MiniMapDisplay, std::placeholders::_1, std::placeholders::_2));
	MiniMapBorder->SetName("border");
	MiniMapBorder->SetLeft(0.0_c);
	MiniMapBorder->SetTop(0.0_c);
	MiniMapBorder->SetWidth(width(MiniMapWidget));
	MiniMapBorder->SetHeight(height(MiniMapWidget));
	MiniMapBorder->SetLineWidth(1.0f);
	MiniMapBorder->SetColor(Graphics::ColorRGBO(0.1f, 0.2f, 0.3f, 1.0f));
	ScannerWidget->SetName("scanner");
	ScannerWidget->SetLeft(0.0_c);
	ScannerWidget->SetTop(height(this) - height(ScannerWidget));
	ScannerWidget->SetWidth(220.0_c);
	ScannerWidget->SetHeight(240.0_c);
	ScannerWidget->SetBackgroundColor(Graphics::ColorRGBO(0.0f, 0.1f, 0.17f, 0.8f));
	ScannerWidget->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateScannerWidget, this, ScannerWidget, std::placeholders::_1, std::placeholders::_2));
	ScannerTargetNameLabel->SetName("target_name");
	ScannerTargetNameLabel->SetLeft(0.0_c);
	ScannerTargetNameLabel->SetTop(0.0_c);
	ScannerTargetNameLabel->SetWidth(width(ScannerWidget));
	ScannerTargetNameLabel->SetHeight(20.0_c);
	ScannerTargetNameLabel->SetTextColor(Graphics::ColorRGBO(0.7f, 0.8f, 1.0f, 1.0f));
	ScannerTargetNameLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Center);
	ScannerTargetNameLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	ScannerTargetNameLabel->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateScannerTargetNameLabel, this, ScannerTargetNameLabel, std::placeholders::_1, std::placeholders::_2));
	ScannerDisplay->SetName("display");
	ScannerDisplay->SetLeft(0.0_c);
	ScannerDisplay->SetTop(bottom(ScannerTargetNameLabel));
	ScannerDisplay->SetWidth(width(ScannerWidget));
	ScannerDisplay->SetHeight(height(ScannerWidget) - top(ScannerDisplay));
	ScannerDisplay->ConnectUpdatingCallback(std::bind(&UI::HeadsUpDisplay::_UpdateScannerDisplay, this, ScannerDisplay, std::placeholders::_1, std::placeholders::_2));
	ScannerBorder->SetName("border");
	ScannerBorder->SetLeft(0.0_c);
	ScannerBorder->SetTop(0.0_c);
	ScannerBorder->SetWidth(width(ScannerWidget));
	ScannerBorder->SetHeight(height(ScannerWidget));
	ScannerBorder->SetLineWidth(1.0f);
	ScannerBorder->SetColor(Graphics::ColorRGBO(0.1f, 0.2f, 0.3f, 1.0f));
	// add components
	AddSubWidget(CycleLabel);
	AddSubWidget(CycleProgressBar);
	AddSubWidget(TimeWarpLabel);
	AddSubWidget(LinkedSystemTargetLabel);
	AddSubWidget(CreditsLabel);
	AddSubWidget(FuelLabel);
	AddSubWidget(EnergyLabel);
	AddSubWidget(HullLabel);
	AddSubWidget(_MessageLabel);
	AddSubWidget(MiniMapWidget);
	MiniMapWidget->AddSubWidget(MiniMapBorder);
	MiniMapWidget->AddSubWidget(MiniMapDisplay);
	MiniMapWidget->AddSubWidget(MiniMapSystemNameLabel);
	AddSubWidget(ScannerWidget);
	ScannerWidget->AddSubWidget(ScannerBorder);
	ScannerWidget->AddSubWidget(ScannerDisplay);
	ScannerWidget->AddSubWidget(ScannerTargetNameLabel);
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

void UI::HeadsUpDisplay::_UpdateCycleProgressBar(UI::ProgressBar * CycleProgressBar, float RealTimeSeconds, float GameTimeSeconds)
{
	if(g_Galaxy != nullptr)
	{
		CycleProgressBar->SetVisible(true);
		CycleProgressBar->SetText(to_string_cast(GameTime::GetCycle()));
		CycleProgressBar->SetFillLevel(GameTime::GetCycleFraction());
	}
	else
	{
		CycleProgressBar->SetVisible(false);
	}
}

void UI::HeadsUpDisplay::_UpdateEnergyLabel(UI::Label * EnergyLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	assert(g_CharacterObserver != nullptr);
	if((g_CharacterObserver->GetObservedCharacter() != nullptr) && (g_CharacterObserver->GetObservedCharacter()->GetShip() != nullptr))
	{
		auto Energy{0.0f};
		
		for(auto Content : g_CharacterObserver->GetObservedCharacter()->GetShip()->GetAspectObjectContainer()->GetContent())
		{
			auto TheBattery{dynamic_cast< Battery * >(Content)};
			
			if(TheBattery != nullptr)
			{
				Energy += TheBattery->GetEnergy();
			}
		}
		if(Energy > 0.0f)
		{
			EnergyLabel->SetVisible(true);
			EnergyLabel->SetText("Energy: " + to_string_cast(Energy, 2));
		}
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
		TimeWarpLabel->SetText("(x" + to_string_cast(g_TimeWarp, 2) + ')');
	}
	else
	{
		TimeWarpLabel->SetVisible(false);
	}
}
