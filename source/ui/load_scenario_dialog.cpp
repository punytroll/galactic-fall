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

#include "../globals.h"
#include "../real_time.h"
#include "../scenario.h"
#include "../scenario_manager.h"
#include "key_event.h"
#include "label.h"
#include "list_box.h"
#include "list_box_scenario_item.h"
#include "load_scenario_dialog.h"
#include "mouse_button_event.h"
#include "text_button.h"

using namespace Expressions::Operators;

UI::LoadScenarioDialog::LoadScenarioDialog(ScenarioManager * ScenarioManager) :
	_ScenarioManager(ScenarioManager)
{
	SetTitle("Load Scenario");
	ConnectKeyCallback(std::bind(&UI::LoadScenarioDialog::_OnKey, this, std::placeholders::_1));
	
	// create components
	auto CancelButton{new UI::TextButton{}};
	auto OKButton{new UI::TextButton{}};
	
	_MessageLabel = new UI::Label{};
	_ScenarioListBox = new UI::ListBox{};
	// initialize components
	OKButton->SetLeft(width(this) - 10.0_c - width(OKButton));
	OKButton->SetTop(height(this) - 30.0_c);
	OKButton->SetWidth(100.0_c);
	OKButton->SetHeight(20.0_c);
	OKButton->SetText("OK");
	OKButton->ConnectClickedCallback(std::bind(&UI::LoadScenarioDialog::_Close, this, UI::Dialog::ClosingReason::OK_BUTTON));
	CancelButton->SetLeft(left(OKButton) - 10.0_c - width(CancelButton));
	CancelButton->SetTop(height(this) - 30.0_c);
	CancelButton->SetWidth(100.0_c);
	CancelButton->SetHeight(20.0_c);
	CancelButton->SetText("Cancel");
	CancelButton->ConnectClickedCallback(std::bind(&UI::LoadScenarioDialog::_Close, this, UI::Dialog::ClosingReason::CANCEL_BUTTON));
	_MessageLabel->SetLeft(10.0_c);
	_MessageLabel->SetTop(40.0_c);
	_MessageLabel->SetWidth(width(this) - 2.0_c * left(_MessageLabel));
	_MessageLabel->SetHeight(30.0_c);
	_MessageLabel->SetTextColor(Graphics::ColorRGBO(1.0f, 0.3, 0.3f, 1.0f));
	_MessageLabel->SetWrap(true);
	_MessageLabel->SetWordWrap(true);
	_MessageLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	_ScenarioListBox->SetLeft(10.0_c);
	_ScenarioListBox->SetTop(bottom(_MessageLabel) + 10.0_c);
	_ScenarioListBox->SetWidth(width(this) - 2.0_c * left(_ScenarioListBox));
	_ScenarioListBox->SetHeight(top(OKButton) - 10.0_c - top(_ScenarioListBox));
	_ScenarioListBox->SetHorizontalScrollBarVisible(false);
	for(auto ScenarioPair : _ScenarioManager->GetScenarios())
	{
		_ScenarioListBox->GetContent()->AddSubWidget(new UI::ListBoxScenarioItem{ScenarioPair.second});
	}
	// add components
	AddSubWidget(CancelButton);
	AddSubWidget(_MessageLabel);
	AddSubWidget(OKButton);
	AddSubWidget(_ScenarioListBox);
}

Scenario * UI::LoadScenarioDialog::GetScenario(void)
{
	if(_ScenarioListBox->GetSelectedItem() != nullptr)
	{
		auto SelectedListBoxScenarioItem{dynamic_cast< UI::ListBoxScenarioItem * >(_ScenarioListBox->GetSelectedItem())};
		
		assert(SelectedListBoxScenarioItem != nullptr);
		
		return SelectedListBoxScenarioItem->GetScenario();
	}
	else
	{
		return nullptr;
	}
}

void UI::LoadScenarioDialog::ShowErrorMessage(const std::string & ErrorMessage)
{
	if(_MessageTimeoutNotification.IsValid() == true)
	{
		_MessageTimeoutNotification.Dismiss();
	}
	_MessageLabel->SetVisible(true);
	_MessageLabel->SetText(ErrorMessage);
	_MessageTimeoutNotification = g_RealTimeTimeoutNotifications->Add(RealTime::Get() + 2.0f, std::bind(&UI::Label::SetVisible, _MessageLabel, false));
}

void UI::LoadScenarioDialog::_OnKey(UI::KeyEvent & KeyEvent)
{
	if((KeyEvent.GetKeyCode() == 9 /* ESCAPE */) && (KeyEvent.IsDown() == true))
	{
		_Close(UI::Dialog::ClosingReason::ESCAPE_KEY);
	}
	else if((KeyEvent.GetKeyCode() == 36 /* RETURN */) && (KeyEvent.IsDown() == true))
	{
		_Close(UI::Dialog::ClosingReason::RETURN_KEY);
	}
}
