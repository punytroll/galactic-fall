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

#ifndef LOAD_SCENARIO_DIALOG_H
#define LOAD_SCENARIO_DIALOG_H

#include "../timeout_notifications.h"
#include "dialog.h"

class ScenarioManager;

namespace UI
{
	class Button;
	class ScenarioItem;
	class Label;
	class ScrollBox;
	
	class LoadScenarioDialog : public UI::Dialog
	{
	public:
		LoadScenarioDialog(UI::Widget * SupWidget, ScenarioManager * ScenarioManager);
		// getters
		Scenario * GetScenario(void);
		// modifiers
		void ShowErrorMessage(const std::string & ErrorMessage);
	private:
		// callbacks
		bool _OnScenarioItemMouseButton(UI::ScenarioItem * ScenarioItem, int Button, int State, float X, float Y);
		bool _OnKey(const KeyEventInformation & KeyEventInformation);
		// helper functions and actions
		void _HideErrorMessage(void);
		// member variables
		UI::Button * _CancelButton;
		UI::Label * _ErrorMessage;
		TimeoutNotification _ErrorMessageTimeoutNotification;
		UI::Button * _OKButton;
		ScenarioManager * _ScenarioManager;
		UI::ScrollBox * _ScenarioScrollBox;
		UI::ScenarioItem * _SelectedScenarioItem;
	};
}

#endif