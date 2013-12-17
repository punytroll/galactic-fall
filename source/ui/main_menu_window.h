/**
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

#ifndef MAIN_MENU_WINDOW_H
#define MAIN_MENU_WINDOW_H

#include "dialog.h"
#include "window.h"

class ScenarioManager;

namespace UI
{
	class Button;
	class LoadGameDialog;
	class LoadScenarioDialog;
	class SaveGameDialog;
	
	class MainMenuWindow : public UI::Window
	{
	public:
		// constructor and destructor
		MainMenuWindow(UI::Widget * SupWidget, ScenarioManager * ScenarioManager);
		// getters
		UI::Button * GetLoadGameButton(void);
		UI::Button * GetLoadScenarioButton(void);
		UI::Button * GetNewGameButton(void);
		UI::Button * GetQuitButton(void);
		UI::Button * GetResumeGameButton(void);
		UI::Button * GetSaveGameButton(void);
		// setters
		void SetDestroyOnESCAPEKey(bool DestroyOnESCAPEKey);
	private:
		// callbacks
		bool _OnKey(const KeyEventInformation & KeyEventInformation);
		void _OnLoadGameButtonClicked(void);
		bool _OnLoadGameDialogClosing(UI::Dialog::ClosingReason ClosingReason);
		void _OnLoadGameDialogDestroying(void);
		void _OnLoadScenarioButtonClicked(void);
		bool _OnLoadScenarioDialogClosing(UI::Dialog::ClosingReason ClosingReason);
		void _OnLoadScenarioDialogDestroying(void);
		void _OnNewGameButtonClicked(void);
		void _OnQuitButtonClicked(void);
		void _OnResumeGameButtonClicked(void);
		void _OnSaveGameButtonClicked(void);
		bool _OnSaveGameDialogClosing(UI::Dialog::ClosingReason ClosingReasong);
		void _OnSaveGameDialogDestroying(void);
		// member variables
		bool _DestroyOnESCAPEKey;
		bool _DestroyOnLoadGameDialogDestroy;
		bool _DestroyOnLoadScenarioDialogDestroy;
		bool _DestroyOnSaveGameDialogDestroy;
		UI::Button * _LoadGameButton;
		UI::LoadGameDialog * _LoadGameDialog;
		UI::Button * _LoadScenarioButton;
		UI::LoadScenarioDialog * _LoadScenarioDialog;
		UI::Button * _NewGameButton;
		UI::Button * _QuitButton;
		UI::Button * _ResumeGameButton;
		UI::Button * _SaveGameButton;
		UI::SaveGameDialog * _SaveGameDialog;
		ScenarioManager * _ScenarioManager;
	};

	inline UI::Button * MainMenuWindow::GetLoadGameButton(void)
	{
		return _LoadGameButton;
	}
	
	inline UI::Button * MainMenuWindow::GetLoadScenarioButton(void)
	{
		return _LoadScenarioButton;
	}

	inline UI::Button * MainMenuWindow::GetNewGameButton(void)
	{
		return _NewGameButton;
	}

	inline UI::Button * MainMenuWindow::GetQuitButton(void)
	{
		return _QuitButton;
	}

	inline UI::Button * MainMenuWindow::GetResumeGameButton(void)
	{
		return _ResumeGameButton;
	}

	inline UI::Button * MainMenuWindow::GetSaveGameButton(void)
	{
		return _SaveGameButton;
	}
	
	inline void MainMenuWindow::SetDestroyOnESCAPEKey(bool DestroyOnESCAPEKey)
	{
		_DestroyOnESCAPEKey = DestroyOnESCAPEKey;
	}
}

#endif
