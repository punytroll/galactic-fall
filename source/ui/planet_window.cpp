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

#include "../callbacks/callbacks.h"
#include "../key_event_information.h"
#include "../object_aspect_name.h"
#include "../planet.h"
#include "button.h"
#include "label.h"
#include "planet_window.h"
#include "space_dock_widget.h"
#include "trade_center_widget.h"

UI::PlanetWindow::PlanetWindow(UI::Widget * SupWidget, Reference< Planet > Planet, Reference< Character > Character) :
	UI::Window(SupWidget),
	_Character(Character),
	_DescriptionLabel(0),
	_Planet(Planet),
	_SpaceDockWidget(0),
	_TradeCenterWidget(0)
{
	assert(_Character.IsValid() == true);
	assert(_Planet.IsValid() == true);
	assert(_Planet->GetAspectName() != 0);
	GetTitleLabel()->SetText("Planet: " + _Planet->GetAspectName()->GetName());
	SetPosition(Vector2f(50.0f, 50.0f));
	SetSize(Vector2f(700.0f, 400.0f));
	ConnectKeyCallback(Callback(this, &PlanetWindow::_OnKey));
	
	UI::Button * HomeButton(new UI::Button(this));
	
	HomeButton->SetPosition(Vector2f(10.0f, 40.0f));
	HomeButton->SetSize(Vector2f(100.0f, 20.0f));
	HomeButton->ConnectClickedCallback(Callback(this, &PlanetWindow::_OnHomeButtonClicked));
	
	UI::Label * HomeButtonLabel(new UI::Label(HomeButton, "Home"));
	
	HomeButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	HomeButtonLabel->SetSize(HomeButton->GetSize());
	HomeButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	HomeButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	
	UI::Button * TradeCenterButton(new UI::Button(this));
	
	TradeCenterButton->SetPosition(Vector2f(10.0f, 70.0f));
	TradeCenterButton->SetSize(Vector2f(100.0f, 20.0f));
	TradeCenterButton->ConnectClickedCallback(Callback(this, &PlanetWindow::_OnTradeCenterButtonClicked));
	
	UI::Label * TradeCenterButtonLabel(new UI::Label(TradeCenterButton, "Trade Center"));
	
	TradeCenterButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	TradeCenterButtonLabel->SetSize(TradeCenterButton->GetSize());
	TradeCenterButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	TradeCenterButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	
	UI::Button * SpaceDockButton(new UI::Button(this));
	
	SpaceDockButton->SetPosition(Vector2f(10.0f, 100.0f));
	SpaceDockButton->SetSize(Vector2f(100.0f, 20.0f));
	SpaceDockButton->ConnectClickedCallback(Callback(this, &PlanetWindow::_OnSpaceDockButtonClicked));
	
	UI::Label * SpaceDockButtonLabel(new UI::Label(SpaceDockButton, "Space Dock"));
	
	SpaceDockButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	SpaceDockButtonLabel->SetSize(SpaceDockButton->GetSize());
	SpaceDockButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	SpaceDockButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	_OpenHomeScreen();
}

void UI::PlanetWindow::_OnHomeButtonClicked(void)
{
	_OpenHomeScreen();
}

bool UI::PlanetWindow::_OnKey(const KeyEventInformation & KeyEventInformation)
{
	if((KeyEventInformation.GetKeyCode() == 39 /* S */) && (KeyEventInformation.IsDown() == true))
	{
		_OpenSpaceDock();
	}
	else if((KeyEventInformation.GetKeyCode() == 28 /* T */) && (KeyEventInformation.IsDown() == true))
	{
		_OpenTradeCenter();
	}
	
	// eat all input
	return true;
}

void UI::PlanetWindow::_OnSpaceDockButtonClicked(void)
{
	_OpenSpaceDock();
}

void UI::PlanetWindow::_OnTradeCenterButtonClicked(void)
{
	_OpenTradeCenter();
}

void UI::PlanetWindow::_OpenHomeScreen(void)
{
	if(_SpaceDockWidget != 0)
	{
		assert(_DescriptionLabel == 0);
		assert(_TradeCenterWidget == 0);
		_SpaceDockWidget->Destroy();
		_SpaceDockWidget = 0;
	}
	if(_TradeCenterWidget != 0)
	{
		assert(_DescriptionLabel == 0);
		assert(_SpaceDockWidget == 0);
		_TradeCenterWidget->Destroy();
		_TradeCenterWidget = 0;
	}
	if(_DescriptionLabel == 0)
	{
		_DescriptionLabel = new UI::Label(this, _Planet->GetDescription());
		_DescriptionLabel->SetPosition(Vector2f(120.0f, 40.0f));
		_DescriptionLabel->SetSize(Vector2f(GetSize()[0] - 130.0f, GetSize()[1] - 50.0f));
		_DescriptionLabel->SetWrap(true);
		_DescriptionLabel->SetWordWrap(true);
		_DescriptionLabel->SetAnchorRight(true);
	}
	SetKeyFocus(0);
}

void UI::PlanetWindow::_OpenSpaceDock(void)
{
	if(_DescriptionLabel != 0)
	{
		assert(_TradeCenterWidget == 0);
		assert(_SpaceDockWidget == 0);
		_DescriptionLabel->Destroy();
		_DescriptionLabel = 0;
	}
	if(_TradeCenterWidget != 0)
	{
		assert(_DescriptionLabel == 0);
		assert(_SpaceDockWidget == 0);
		_TradeCenterWidget->Destroy();
		_TradeCenterWidget = 0;
	}
	if(_SpaceDockWidget == 0)
	{
		_SpaceDockWidget = new UI::SpaceDockWidget(this, _Planet, _Character);
		_SpaceDockWidget->SetPosition(Vector2f(120.0f, 40.0f));
		_SpaceDockWidget->SetSize(Vector2f(GetSize()[0] - 130.0f, GetSize()[1] - 50.0f));
		_SpaceDockWidget->SetAnchorBottom(true);
		_SpaceDockWidget->SetAnchorRight(true);
	}
	_SpaceDockWidget->GrabKeyFocus();
}

void UI::PlanetWindow::_OpenTradeCenter(void)
{
	if(_DescriptionLabel != 0)
	{
		assert(_SpaceDockWidget == 0);
		assert(_TradeCenterWidget == 0);
		_DescriptionLabel->Destroy();
		_DescriptionLabel = 0;
	}
	if(_SpaceDockWidget != 0)
	{
		assert(_DescriptionLabel == 0);
		assert(_TradeCenterWidget == 0);
		_SpaceDockWidget->Destroy();
		_SpaceDockWidget = 0;
	}
	if(_TradeCenterWidget == 0)
	{
		_TradeCenterWidget = new UI::TradeCenterWidget(this, _Planet, _Character);
		_TradeCenterWidget->SetPosition(Vector2f(120.0f, 40.0f));
		_TradeCenterWidget->SetSize(Vector2f(GetSize()[0] - 130.0f, GetSize()[1] - 50.0f));
		_TradeCenterWidget->SetAnchorBottom(true);
		_TradeCenterWidget->SetAnchorRight(true);
	}
	_TradeCenterWidget->GrabKeyFocus();
}
