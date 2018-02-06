/**
 * galactic-fall
 * Copyright (C) 2013-2018  Hagen Möbius
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

#include "../character.h"
#include "../object_aspect_name.h"
#include "../planet.h"
#include "hangar_widget.h"
#include "key_event.h"
#include "label.h"
#include "planet_window.h"
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
	
	auto HomeButton{new UI::TextButton{this, "Home"}};
	
	HomeButton->SetLeft(10.0_c);
	HomeButton->SetTop(40.0_c);
	HomeButton->SetWidth(100.0_c);
	HomeButton->SetHeight(20.0_c);
	HomeButton->ConnectClickedCallback(std::bind(&UI::PlanetWindow::_OnHomeButtonClicked, this));
	
	auto TradeCenterButton{new UI::TextButton{this, "Trade Center"}};
	
	TradeCenterButton->SetLeft(10.0_c);
	TradeCenterButton->SetTop(70.0_c);
	TradeCenterButton->SetWidth(100.0_c);
	TradeCenterButton->SetHeight(20.0_c);
	TradeCenterButton->ConnectClickedCallback(std::bind(&UI::PlanetWindow::_OnTradeCenterButtonClicked, this));
	
	auto HangarButton{new UI::TextButton{this, "Hangar"}};
	
	HangarButton->SetLeft(10.0_c);
	HangarButton->SetTop(100.0_c);
	HangarButton->SetWidth(100.0_c);
	HangarButton->SetHeight(20.0_c);
	HangarButton->ConnectClickedCallback(std::bind(&UI::PlanetWindow::_OnHangarButtonClicked, this));
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
		_HangarWidget = new UI::HangarWidget{this, _Planet, _Character};
		_HangarWidget->SetLeft(120.0_c);
		_HangarWidget->SetTop(40.0_c);
		_HangarWidget->SetWidth(width(this) - 10.0_c - left(_HangarWidget));
		_HangarWidget->SetHeight(height(this) - 10.0_c - top(_HangarWidget));
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
		_DescriptionLabel = new UI::Label{this, _Planet->GetDescription()};
		_DescriptionLabel->SetLeft(120.0_c);
		_DescriptionLabel->SetTop(40.0_c);
		_DescriptionLabel->SetWidth(width(this) - 10.0_c - left(_DescriptionLabel));
		_DescriptionLabel->SetHeight(height(this) - 10.0_c - top(_DescriptionLabel));
		_DescriptionLabel->SetWrap(true);
		_DescriptionLabel->SetWordWrap(true);
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
		_TradeCenterWidget = new UI::TradeCenterWidget{this, _Planet, _Character};
		_TradeCenterWidget->SetLeft(120.0_c);
		_TradeCenterWidget->SetTop(40.0_c);
		_TradeCenterWidget->SetWidth(width(this) - 10.0_c - left(_TradeCenterWidget));
		_TradeCenterWidget->SetHeight(height(this) - 10.0_c - top(_TradeCenterWidget));
	}
	_TradeCenterWidget->GrabKeyFocus();
}
