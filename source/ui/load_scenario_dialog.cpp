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

#include "../color.h"
#include "../globals.h"
#include "../key_event_information.h"
#include "../real_time.h"
#include "../scenario.h"
#include "../scenario_manager.h"
#include "label.h"
#include "load_scenario_dialog.h"
#include "scroll_box.h"
#include "text_button.h"

namespace UI
{
	class ScenarioItem : public UI::Widget
	{
	public:
		ScenarioItem(UI::Widget * SupWidget, Scenario * Scenario);
		// getters
		Scenario * GetScenario(void);
		// setters
		void SetSelected(bool Selected);
	private:
		// callbacks
		void _OnMouseEnter(void);
		void _OnMouseLeave(void);
		// member variables
		Scenario * _Scenario;
		bool _Selected;
	};
}

UI::ScenarioItem::ScenarioItem(UI::Widget * SupWidget, Scenario * Scenario) :
	UI::Widget(SupWidget),
	_Scenario(Scenario),
	_Selected(false)
{
	ConnectMouseEnterCallback(std::bind(&UI::ScenarioItem::_OnMouseEnter, this));
	ConnectMouseLeaveCallback(std::bind(&UI::ScenarioItem::_OnMouseLeave, this));
	
	UI::Label * CaptionLabel(new UI::Label(this, Scenario->GetName()));
	
	CaptionLabel->SetPosition(Vector2f(5.0f, 0.0f));
	CaptionLabel->SetSize(Vector2f(GetSize()[0] - 10.0f, 20.0f));
	CaptionLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	CaptionLabel->SetAnchorLeft(true);
	CaptionLabel->SetAnchorRight(true);
	CaptionLabel->SetAnchorTop(true);
}

Scenario * UI::ScenarioItem::GetScenario(void)
{
	return _Scenario;
}

void UI::ScenarioItem::SetSelected(bool Selected)
{
	_Selected = Selected;
	if(_Selected == false)
	{
		UnsetBackgroundColor();
	}
	else
	{
		SetBackgroundColor(Color(0.4f, 0.1f, 0.1f, 1.0f));
	}
}

void UI::ScenarioItem::_OnMouseEnter(void)
{
	if(_Selected == false)
	{
		SetBackgroundColor(Color(0.3f, 0.2f, 0.2f, 1.0f));
	}
}

void UI::ScenarioItem::_OnMouseLeave(void)
{
	if(_Selected == false)
	{
		UnsetBackgroundColor();
	}
}

UI::LoadScenarioDialog::LoadScenarioDialog(UI::Widget * SupWidget, ScenarioManager * ScenarioManager) :
	UI::Dialog(SupWidget),
	_ScenarioManager(ScenarioManager),
	_SelectedScenarioItem(0)
{
	GetTitleLabel()->SetText("Load Scenario");
	SetPosition(Vector2f(120.0f, 200.0f));
	SetSize(Vector2f(300.0f, 400.0f));
	ConnectKeyCallback(std::bind(&UI::LoadScenarioDialog::_OnKey, this, std::placeholders::_1));
	_OKButton = new UI::TextButton(this, "OK");
	_OKButton->SetSize(Vector2f(100.0f, 20.0f));
	_OKButton->SetPosition(Vector2f(GetSize()[0] - 10.0f - _OKButton->GetSize()[0], GetSize()[1] - 10.0f - _OKButton->GetSize()[1]));
	_OKButton->SetAnchorBottom(true);
	_OKButton->SetAnchorLeft(false);
	_OKButton->SetAnchorRight(true);
	_OKButton->SetAnchorTop(false);
	_OKButton->ConnectClickedCallback(std::bind(&UI::LoadScenarioDialog::_Close, this, UI::Dialog::ClosingReason::OK_BUTTON));
	_CancelButton = new UI::TextButton(this, "Cancel");
	_CancelButton->SetSize(Vector2f(100.0f, 20.0f));
	_CancelButton->SetPosition(Vector2f(GetSize()[0] - 10.0f - _OKButton->GetSize()[0] - 10.0f - _CancelButton->GetSize()[0], GetSize()[1] - 10.0f - _CancelButton->GetSize()[1]));
	_CancelButton->SetAnchorBottom(true);
	_CancelButton->SetAnchorLeft(false);
	_CancelButton->SetAnchorRight(true);
	_CancelButton->SetAnchorTop(false);
	_CancelButton->ConnectClickedCallback(std::bind(&UI::LoadScenarioDialog::_Close, this, UI::Dialog::ClosingReason::CANCEL_BUTTON));
	_ErrorMessage = new UI::Label(this);
	_ErrorMessage->SetPosition(Vector2f(10.0f, 40.0f));
	_ErrorMessage->SetSize(Vector2f(GetSize()[0] - 10.0f - 10.0f, 30.0f));
	_ErrorMessage->SetTextColor(Color(1.0f, 0.3, 0.3f, 1.0f));
	_ErrorMessage->SetAnchorBottom(false);
	_ErrorMessage->SetAnchorLeft(true);
	_ErrorMessage->SetAnchorRight(true);
	_ErrorMessage->SetAnchorTop(true);
	_ErrorMessage->SetWrap(true);
	_ErrorMessage->SetWordWrap(true);
	_ErrorMessage->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	_ScenarioScrollBox = new UI::ScrollBox(this);
	_ScenarioScrollBox->SetPosition(Vector2f(10.0f, 110.0f));
	_ScenarioScrollBox->SetSize(Vector2f(GetSize()[0] - 10.0f - 10.0f, GetSize()[1] - 110.0f - 30.0f - 10.0f - _OKButton->GetSize()[1]));
	_ScenarioScrollBox->SetAnchorBottom(true);
	_ScenarioScrollBox->SetAnchorRight(true);
	_ScenarioScrollBox->SetAnchorTop(true);
	_ScenarioScrollBox->SetHorizontalScrollBarVisible(false);
	
	float Top(5.0f);
	const std::map< std::string, Scenario * > & Scenarios(_ScenarioManager->GetScenarios());
	
	for(std::map< std::string, Scenario * >::const_iterator ScenarioIterator = Scenarios.begin(); ScenarioIterator != Scenarios.end(); ++ScenarioIterator)
	{
		UI::ScenarioItem * ScenarioItem(new UI::ScenarioItem(_ScenarioScrollBox->GetContent(), ScenarioIterator->second));
		
		ScenarioItem->SetPosition(Vector2f(5.0f, Top));
		ScenarioItem->SetSize(Vector2f(_ScenarioScrollBox->GetContent()->GetSize()[0] - 10.0f, 20.0f));
		ScenarioItem->SetAnchorRight(true);
		ScenarioItem->ConnectMouseButtonCallback(std::bind(&UI::LoadScenarioDialog::_OnScenarioItemMouseButton, this, ScenarioItem, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
		Top += 25.0f;
	}
	_ScenarioScrollBox->GetContent()->SetSize(Vector2f(_ScenarioScrollBox->GetView()->GetSize()[0], std::max(Top, _ScenarioScrollBox->GetView()->GetSize()[1])));
}

Scenario * UI::LoadScenarioDialog::GetScenario(void)
{
	if(_SelectedScenarioItem != 0)
	{
		return _SelectedScenarioItem->GetScenario();
	}
	else
	{
		return 0;
	}
}

void UI::LoadScenarioDialog::ShowErrorMessage(const std::string & ErrorMessage)
{
	if(_ErrorMessageTimeoutNotification.IsValid() == true)
	{
		_ErrorMessageTimeoutNotification.Dismiss();
	}
	_ErrorMessage->SetVisible(true);
	_ErrorMessage->SetText(ErrorMessage);
	_ErrorMessageTimeoutNotification = g_RealTimeTimeoutNotifications->Add(RealTime::Get() + 2.0f, std::bind(&LoadScenarioDialog::_HideErrorMessage, this));
}

void UI::LoadScenarioDialog::_HideErrorMessage(void)
{
	_ErrorMessage->SetVisible(false);
}

bool UI::LoadScenarioDialog::_OnKey(const KeyEventInformation & KeyEventInformation)
{
	if((KeyEventInformation.GetKeyCode() == 9 /* ESCAPE */) && (KeyEventInformation.IsDown() == true))
	{
		_Close(UI::Dialog::ClosingReason::ESCAPE_KEY);
		
		return true;
	}
	else if((KeyEventInformation.GetKeyCode() == 36 /* RETURN */) && (KeyEventInformation.IsDown() == true))
	{
		_Close(UI::Dialog::ClosingReason::RETURN_KEY);
		
		return true;
	}
	
	return false;
}

bool UI::LoadScenarioDialog::_OnScenarioItemMouseButton(UI::ScenarioItem * ScenarioItem, int Button, int State, float X, float Y)
{
	if((Button == 1 /* LEFT */) && (State == EV_DOWN))
	{
		if(_SelectedScenarioItem != 0)
		{
			_SelectedScenarioItem->SetSelected(false);
		}
		_SelectedScenarioItem = ScenarioItem;
		_SelectedScenarioItem->SetSelected(true);
		
		return true;
	}
	
	return false;
}
