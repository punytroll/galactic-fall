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

#include "../globals.h"
#include "../graphics/color_rgbo.h"
#include "../real_time.h"
#include "../scenario.h"
#include "../scenario_manager.h"
#include "key_event.h"
#include "label.h"
#include "list_box_item.h"
#include "load_scenario_dialog.h"
#include "mouse_button_event.h"
#include "scroll_box.h"
#include "text_button.h"

namespace UI
{
	class ScenarioItem : public UI::ListBoxItem
	{
	public:
		ScenarioItem(UI::Widget * SupWidget, Scenario * Scenario);
		// getters
		Scenario * GetScenario(void);
	private:
		// member variables
		Scenario * _Scenario;
	};
}

UI::ScenarioItem::ScenarioItem(UI::Widget * SupWidget, Scenario * Scenario) :
	UI::ListBoxItem(SupWidget),
	_Scenario(Scenario)
{
	auto CaptionLabel{new UI::Label{this, Scenario->GetName()}};
	
	CaptionLabel->SetLeft(5.0f);
	CaptionLabel->SetTop(0.0f);
	CaptionLabel->SetWidth(GetWidth() - 10.0f);
	CaptionLabel->SetHeight(GetHeight());
	CaptionLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	CaptionLabel->SetAnchorBottom(true);
	CaptionLabel->SetAnchorLeft(true);
	CaptionLabel->SetAnchorRight(true);
	CaptionLabel->SetAnchorTop(true);
	SetHeight(20.0f);
}

Scenario * UI::ScenarioItem::GetScenario(void)
{
	return _Scenario;
}

UI::LoadScenarioDialog::LoadScenarioDialog(UI::Widget * SupWidget, ScenarioManager * ScenarioManager) :
	UI::Dialog(SupWidget),
	_ScenarioManager(ScenarioManager),
	_SelectedScenarioItem(nullptr)
{
	SetTitle("Load Scenario");
	ConnectKeyCallback(std::bind(&UI::LoadScenarioDialog::_OnKey, this, std::placeholders::_1));

	auto OKButton{new UI::TextButton{this, "OK"}};
	
	OKButton->SetLeft(GetWidth() - 110.0f);
	OKButton->SetTop(GetHeight() - 30.0f);
	OKButton->SetWidth(100.0f);
	OKButton->SetHeight(20.0f);
	OKButton->SetAnchorBottom(true);
	OKButton->SetAnchorLeft(false);
	OKButton->SetAnchorRight(true);
	OKButton->SetAnchorTop(false);
	OKButton->ConnectClickedCallback(std::bind(&UI::LoadScenarioDialog::_Close, this, UI::Dialog::ClosingReason::OK_BUTTON));
	
	auto CancelButton{new UI::TextButton{this, "Cancel"}};
	
	CancelButton->SetLeft(GetWidth() - 220.0f);
	CancelButton->SetTop(GetHeight() - 30.0f);
	CancelButton->SetWidth(100.0f);
	CancelButton->SetHeight(20.0f);
	CancelButton->SetAnchorBottom(true);
	CancelButton->SetAnchorLeft(false);
	CancelButton->SetAnchorRight(true);
	CancelButton->SetAnchorTop(false);
	CancelButton->ConnectClickedCallback(std::bind(&UI::LoadScenarioDialog::_Close, this, UI::Dialog::ClosingReason::CANCEL_BUTTON));
	_MessageLabel = new UI::Label{this};
	_MessageLabel->SetLeft(10.0f);
	_MessageLabel->SetTop(40.0f);
	_MessageLabel->SetWidth(GetWidth() - 20.0f);
	_MessageLabel->SetHeight(30.0f);
	_MessageLabel->SetTextColor(Graphics::ColorRGBO(1.0f, 0.3, 0.3f, 1.0f));
	_MessageLabel->SetAnchorBottom(false);
	_MessageLabel->SetAnchorLeft(true);
	_MessageLabel->SetAnchorRight(true);
	_MessageLabel->SetAnchorTop(true);
	_MessageLabel->SetWrap(true);
	_MessageLabel->SetWordWrap(true);
	_MessageLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	_ScenarioScrollBox = new UI::ScrollBox{this};
	_ScenarioScrollBox->SetLeft(10.0f);
	_ScenarioScrollBox->SetTop(110.0f);
	_ScenarioScrollBox->SetWidth(GetWidth() - 20.0f);
	_ScenarioScrollBox->SetHeight(GetHeight() - 170.0f);
	_ScenarioScrollBox->SetAnchorBottom(true);
	_ScenarioScrollBox->SetAnchorRight(true);
	_ScenarioScrollBox->SetAnchorTop(true);
	_ScenarioScrollBox->SetHorizontalScrollBarVisible(false);
	_ScenarioScrollBox->GetContent()->SetWidth(_ScenarioScrollBox->GetView()->GetWidth());
	_ScenarioScrollBox->GetContent()->SetAnchorRight(true);
	
	float Top(5.0f);
	
	for(auto ScenarioPair : _ScenarioManager->GetScenarios())
	{
		auto ScenarioItem{new UI::ScenarioItem{_ScenarioScrollBox->GetContent(), ScenarioPair.second}};
		
		ScenarioItem->SetLeft(5.0f);
		ScenarioItem->SetTop(Top);
		ScenarioItem->SetWidth(_ScenarioScrollBox->GetContent()->GetWidth() - 10.0f);
		ScenarioItem->SetAnchorRight(true);
		ScenarioItem->ConnectMouseButtonCallback(std::bind(&UI::LoadScenarioDialog::_OnScenarioItemMouseButton, this, ScenarioItem, std::placeholders::_1));
		Top += 25.0f;
	}
	_ScenarioScrollBox->GetContent()->SetHeight(Top);
}

Scenario * UI::LoadScenarioDialog::GetScenario(void)
{
	if(_SelectedScenarioItem != nullptr)
	{
		return _SelectedScenarioItem->GetScenario();
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

void UI::LoadScenarioDialog::_OnScenarioItemMouseButton(UI::ScenarioItem * ScenarioItem, UI::MouseButtonEvent & MouseButtonEvent)
{
	if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left) && (MouseButtonEvent.IsDown() == true))
	{
		if(_SelectedScenarioItem != nullptr)
		{
			_SelectedScenarioItem->SetSelected(false);
		}
		_SelectedScenarioItem = ScenarioItem;
		_SelectedScenarioItem->SetSelected(true);
	}
}
