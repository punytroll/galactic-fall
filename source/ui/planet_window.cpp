/**
 * galactic-fall
 * Copyright (C) 2013-2019  Hagen Möbius
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

#include "../character.h"
#include "../game_time.h"
#include "../object_aspect_name.h"
#include "../planet.h"
#include "hangar_widget.h"
#include "key_event.h"
#include "label.h"
#include "planet_window.h"
#include "progress_bar.h"
#include "text_button.h"
#include "trade_center_widget.h"

using namespace Expressions::Operators;

UI::PlanetWindow::PlanetWindow(Planet * Planet, Character * Character) :
	_Character(Character),
	_DescriptionLabel(nullptr),
	_HangarWidget(nullptr),
	_Planet(Planet),
	_TradeCenterWidget(nullptr)
{
	assert(_Character != nullptr);
	_CharacterDestroyingConnection = _Character->ConnectDestroyingCallback(std::bind(&UI::PlanetWindow::_OnCharacterDestroying, this));
	assert(_Planet != nullptr);
	_PlanetDestroyingConnection = _Planet->ConnectDestroyingCallback(std::bind(&UI::PlanetWindow::_OnPlanetDestroying, this));
	assert(_Planet->GetAspectName() != nullptr);
	SetTitle("Planet: " + _Planet->GetAspectName()->GetName());
	ConnectKeyCallback(std::bind(&UI::PlanetWindow::_OnKey, this, std::placeholders::_1));
	ConnectDestroyingCallback(std::bind(&UI::PlanetWindow::_OnDestroying, this, std::placeholders::_1));
	
	// create components
	auto CreditsLabel{new UI::Label{}};
	auto CycleProgressBar{new UI::ProgressBar{}};
	auto HangarButton{new UI::TextButton{}};
	auto HomeButton{new UI::TextButton{}};
	auto TradeCenterButton{new UI::TextButton{}};
	
	// initialize components
	HomeButton->SetLeft(10.0_c);
	HomeButton->SetTop(40.0_c);
	HomeButton->SetWidth(100.0_c);
	HomeButton->SetHeight(20.0_c);
	HomeButton->SetText("Home");
	HomeButton->ConnectClickedCallback(std::bind(&UI::PlanetWindow::_OnHomeButtonClicked, this));
	TradeCenterButton->SetLeft(left(HomeButton));
	TradeCenterButton->SetTop(bottom(HomeButton) + 10.0_c);
	TradeCenterButton->SetWidth(width(HomeButton));
	TradeCenterButton->SetHeight(20.0_c);
	TradeCenterButton->SetText("Trade Center");
	TradeCenterButton->ConnectClickedCallback(std::bind(&UI::PlanetWindow::_OnTradeCenterButtonClicked, this));
	HangarButton->SetLeft(left(HomeButton));
	HangarButton->SetTop(bottom(TradeCenterButton) + 10.0_c);
	HangarButton->SetWidth(width(HomeButton));
	HangarButton->SetHeight(20.0_c);
	HangarButton->SetText("Hangar");
	HangarButton->ConnectClickedCallback(std::bind(&UI::PlanetWindow::_OnHangarButtonClicked, this));
	CreditsLabel->SetName("credits");
	CreditsLabel->SetLeft(left(HomeButton));
	CreditsLabel->SetTop(top(CycleProgressBar) - 10.0_c - height(CreditsLabel));
	CreditsLabel->SetWidth(width(HomeButton));
	CreditsLabel->SetHeight(20.0_c);
	CreditsLabel->ConnectUpdatingCallback(std::bind(&UI::PlanetWindow::_OnUpdateCreditsLabel, this, CreditsLabel, std::placeholders::_1, std::placeholders::_2));
	CreditsLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	CycleProgressBar->SetName("cycle_progress_bar");
	CycleProgressBar->SetLeft(left(HomeButton));
	CycleProgressBar->SetTop(height(this) - 10.0_c - height(CycleProgressBar));
	CycleProgressBar->SetWidth(width(HomeButton));
	CycleProgressBar->SetHeight(20.0_c);
	CycleProgressBar->SetColor(Graphics::ColorRGBO{0.3f, 0.45f, 0.6f, 1.0f});
	CycleProgressBar->ConnectUpdatingCallback(std::bind(&UI::PlanetWindow::_OnUpdateCycleProgressBar, this, CycleProgressBar, std::placeholders::_1, std::placeholders::_2));
	// add components
	AddSubWidget(CreditsLabel);
	AddSubWidget(CycleProgressBar);
	AddSubWidget(HangarButton);
	AddSubWidget(HomeButton);
	AddSubWidget(TradeCenterButton);
	_OpenHomeScreen();
}

void UI::PlanetWindow::_OnDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		if(_Character != nullptr)
		{
			assert(_CharacterDestroyingConnection.IsValid() == true);
			_CharacterDestroyingConnection.Disconnect();
			assert(_CharacterDestroyingConnection.IsValid() == false);
			_Character = nullptr;
		}
		if(_Planet != nullptr)
		{
			assert(_PlanetDestroyingConnection.IsValid() == true);
			_PlanetDestroyingConnection.Disconnect();
			assert(_PlanetDestroyingConnection.IsValid() == false);
			_Planet = nullptr;
		}
	}
}

void UI::PlanetWindow::_OnCharacterDestroying(void)
{
	assert(_Character != nullptr);
	assert(_CharacterDestroyingConnection.IsValid() == true);
	_CharacterDestroyingConnection.Disconnect();
	assert(_CharacterDestroyingConnection.IsValid() == false);
	_Character = nullptr;
}

void UI::PlanetWindow::_OnHangarButtonClicked(void)
{
	_OpenHangar();
}

void UI::PlanetWindow::_OnHomeButtonClicked(void)
{
	_OpenHomeScreen();
}

void UI::PlanetWindow::_OnKey(UI::KeyEvent & KeyEvent)
{
	if((KeyEvent.GetPhase() == UI::Event::Phase::Bubbling) && (KeyEvent.GetKeyCode() == 28 /* T */) && (KeyEvent.IsDown() == true))
	{
		_OpenTradeCenter();
	}
	else if((KeyEvent.GetPhase() == UI::Event::Phase::Bubbling) && (KeyEvent.GetKeyCode() == 43 /* H */) && (KeyEvent.IsDown() == true))
	{
		_OpenHangar();
	}
}

void UI::PlanetWindow::_OnPlanetDestroying(void)
{
	assert(_Planet != nullptr);
	assert(_PlanetDestroyingConnection.IsValid() == true);
	_PlanetDestroyingConnection.Disconnect();
	assert(_PlanetDestroyingConnection.IsValid() == false);
	_Planet = nullptr;
}

void UI::PlanetWindow::_OnTradeCenterButtonClicked(void)
{
	_OpenTradeCenter();
}

void UI::PlanetWindow::_OnUpdateCreditsLabel(UI::Label * CreditsLabel, float RealTimeSeconds, float GameTimeSeconds)
{
	if(_Character != nullptr)
	{
		CreditsLabel->SetText("Credits: " + to_string_cast(_Character->GetCredits()));
	}
}

void UI::PlanetWindow::_OnUpdateCycleProgressBar(UI::ProgressBar * CycleProgressBar, float RealTimeSeconds, float GameTimeSeconds)
{
	CycleProgressBar->SetText(to_string_cast(GameTime::GetCycle()));
	CycleProgressBar->SetFillLevel(GameTime::GetCycleFraction());
}

void UI::PlanetWindow::_OpenHangar(void)
{
	if(_DescriptionLabel != nullptr)
	{
		assert(_TradeCenterWidget == nullptr);
		assert(_HangarWidget == nullptr);
		_DescriptionLabel->Destroy();
		_DescriptionLabel = nullptr;
	}
	if(_TradeCenterWidget != nullptr)
	{
		assert(_DescriptionLabel == nullptr);
		assert(_HangarWidget == nullptr);
		_TradeCenterWidget->Destroy();
		_TradeCenterWidget = nullptr;
	}
	if(_HangarWidget == nullptr)
	{
		// create components
		_HangarWidget = new UI::HangarWidget{_Planet, _Character};
		// initialize components
		_HangarWidget->SetLeft(120.0_c);
		_HangarWidget->SetTop(40.0_c);
		_HangarWidget->SetWidth(width(this) - 10.0_c - left(_HangarWidget));
		_HangarWidget->SetHeight(height(this) - 10.0_c - top(_HangarWidget));
		// add components
		AddSubWidget(_HangarWidget);
	}
	_HangarWidget->GrabKeyFocus();
}

void UI::PlanetWindow::_OpenHomeScreen(void)
{
	if(_HangarWidget != nullptr)
	{
		assert(_DescriptionLabel == nullptr);
		assert(_TradeCenterWidget == nullptr);
		_HangarWidget->Destroy();
		_HangarWidget = nullptr;
	}
	if(_TradeCenterWidget != nullptr)
	{
		assert(_DescriptionLabel == nullptr);
		assert(_HangarWidget == nullptr);
		_TradeCenterWidget->Destroy();
		_TradeCenterWidget = nullptr;
	}
	if(_DescriptionLabel == nullptr)
	{
		assert(_Planet != nullptr);
		_DescriptionLabel = new UI::Label{};
		_DescriptionLabel->SetLeft(120.0_c);
		_DescriptionLabel->SetTop(40.0_c);
		_DescriptionLabel->SetWidth(width(this) - 10.0_c - left(_DescriptionLabel));
		_DescriptionLabel->SetHeight(height(this) - 10.0_c - top(_DescriptionLabel));
		_DescriptionLabel->SetText(_Planet->GetDescription());
		_DescriptionLabel->SetWrap(true);
		_DescriptionLabel->SetWordWrap(true);
		AddSubWidget(_DescriptionLabel);
	}
	_DescriptionLabel->GrabKeyFocus();
}

void UI::PlanetWindow::_OpenTradeCenter(void)
{
	if(_DescriptionLabel != nullptr)
	{
		assert(_HangarWidget == nullptr);
		assert(_TradeCenterWidget == nullptr);
		_DescriptionLabel->Destroy();
		_DescriptionLabel = nullptr;
	}
	if(_HangarWidget != nullptr)
	{
		assert(_DescriptionLabel == nullptr);
		assert(_TradeCenterWidget == nullptr);
		_HangarWidget->Destroy();
		_HangarWidget = nullptr;
	}
	if(_TradeCenterWidget == nullptr)
	{
		// create components
		_TradeCenterWidget = new UI::TradeCenterWidget{_Planet, _Character};
		// initialize components
		_TradeCenterWidget->SetLeft(120.0_c);
		_TradeCenterWidget->SetTop(40.0_c);
		_TradeCenterWidget->SetWidth(width(this) - 10.0_c - left(_TradeCenterWidget));
		_TradeCenterWidget->SetHeight(height(this) - 10.0_c - top(_TradeCenterWidget));
		// add components
		AddSubWidget(_TradeCenterWidget);
	}
	_TradeCenterWidget->GrabKeyFocus();
}
