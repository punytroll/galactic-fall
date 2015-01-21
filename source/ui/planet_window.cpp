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

#include "../character.h"
#include "../object_aspect_name.h"
#include "../planet.h"
#include "hangar_widget.h"
#include "key_event.h"
#include "label.h"
#include "planet_window.h"
#include "text_button.h"
#include "trade_center_widget.h"

UI::PlanetWindow::PlanetWindow(UI::Widget * SupWidget, Planet * Planet, Character * Character) :
	UI::Window(SupWidget),
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
	SetPosition(Vector2f(50.0f, 50.0f));
	SetSize(Vector2f(700.0f, 400.0f));
	ConnectKeyCallback(std::bind(&UI::PlanetWindow::_OnKey, this, std::placeholders::_1));
	ConnectDestroyingCallback(std::bind(&UI::PlanetWindow::_OnDestroying, this, std::placeholders::_1));
	
	UI::Button * HomeButton(new UI::TextButton(this, "Home"));
	
	HomeButton->SetPosition(Vector2f(10.0f, 40.0f));
	HomeButton->SetSize(Vector2f(100.0f, 20.0f));
	HomeButton->ConnectClickedCallback(std::bind(&UI::PlanetWindow::_OnHomeButtonClicked, this));
	
	UI::Button * TradeCenterButton(new UI::TextButton(this, "Trade Center"));
	
	TradeCenterButton->SetPosition(Vector2f(10.0f, 70.0f));
	TradeCenterButton->SetSize(Vector2f(100.0f, 20.0f));
	TradeCenterButton->ConnectClickedCallback(std::bind(&UI::PlanetWindow::_OnTradeCenterButtonClicked, this));
	
	UI::Button * HangarButton(new UI::TextButton(this, "Hangar"));
	
	HangarButton->SetPosition(Vector2f(10.0f, 100.0f));
	HangarButton->SetSize(Vector2f(100.0f, 20.0f));
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
		_HangarWidget = new UI::HangarWidget(this, _Planet, _Character);
		_HangarWidget->SetPosition(Vector2f(120.0f, 40.0f));
		_HangarWidget->SetSize(Vector2f(GetSize()[0] - 130.0f, GetSize()[1] - 50.0f));
		_HangarWidget->SetAnchorBottom(true);
		_HangarWidget->SetAnchorRight(true);
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
		_DescriptionLabel = new UI::Label(this, _Planet->GetDescription());
		_DescriptionLabel->SetPosition(Vector2f(120.0f, 40.0f));
		_DescriptionLabel->SetSize(Vector2f(GetSize()[0] - 130.0f, GetSize()[1] - 50.0f));
		_DescriptionLabel->SetWrap(true);
		_DescriptionLabel->SetWordWrap(true);
		_DescriptionLabel->SetAnchorRight(true);
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
		_TradeCenterWidget = new UI::TradeCenterWidget(this, _Planet, _Character);
		_TradeCenterWidget->SetPosition(Vector2f(120.0f, 40.0f));
		_TradeCenterWidget->SetSize(Vector2f(GetSize()[0] - 130.0f, GetSize()[1] - 50.0f));
		_TradeCenterWidget->SetAnchorBottom(true);
		_TradeCenterWidget->SetAnchorRight(true);
	}
	_TradeCenterWidget->GrabKeyFocus();
}
