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

#ifndef UI_LOAD_SCENARIO_DIALOG_H
#define UI_LOAD_SCENARIO_DIALOG_H

#include "../timeout_notifications.h"
#include "dialog.h"

class ScenarioManager;

namespace UI
{
	class Label;
	class ListBox;
	
	class LoadScenarioDialog : public UI::Dialog
	{
	public:
		LoadScenarioDialog(ScenarioManager * ScenarioManager);
		// getters
		Scenario * GetScenario(void);
		// modifiers
		void ShowErrorMessage(const std::string & ErrorMessage);
	private:
		// callbacks
		void _OnKey(UI::KeyEvent & KeyEvent);
		// member variables
		UI::Label * _MessageLabel;
		TimeoutNotification _MessageTimeoutNotification;
		UI::ListBox * _ScenarioListBox;
		ScenarioManager * _ScenarioManager;
	};
}

#endif
