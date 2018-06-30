/**
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

#ifndef MAIN_MENU_WINDOW_H
#define MAIN_MENU_WINDOW_H

#include "dialog.h"
#include "window.h"

class ScenarioManager;

namespace UI
{
	class LoadGameDialog;
	class LoadScenarioDialog;
	class SaveGameDialog;
	class TextButton;
	
	class MainMenuWindow : public UI::Window
	{
	public:
		// constructor and destructor
		MainMenuWindow(ScenarioManager * ScenarioManager);
		// getters
		UI::TextButton * GetLoadGameButton(void);
		UI::TextButton * GetLoadScenarioButton(void);
		UI::TextButton * GetNewGameButton(void);
		UI::TextButton * GetQuitButton(void);
		UI::TextButton * GetResumeGameButton(void);
		UI::TextButton * GetSaveGameButton(void);
		// setters
		void SetDestroyOnESCAPEKey(bool DestroyOnESCAPEKey);
	private:
		// actions
		void _ActionNewGame(void);
		void _ActionQuit(void);
		// callbacks
		void _OnKey(UI::KeyEvent & KeyEvent);
		void _OnLoadGameButtonClicked(void);
		bool _OnLoadGameDialogClosing(UI::Dialog::ClosingReason ClosingReason);
		void _OnLoadGameDialogDestroying(UI::Event & DestroyingEvent);
		void _OnLoadScenarioButtonClicked(void);
		bool _OnLoadScenarioDialogClosing(UI::Dialog::ClosingReason ClosingReason);
		void _OnLoadScenarioDialogDestroying(UI::Event & DestroyingEvent);
		void _OnNewGameButtonClicked(void);
		void _OnQuitButtonClicked(void);
		void _OnResumeGameButtonClicked(void);
		void _OnSaveGameButtonClicked(void);
		bool _OnSaveGameDialogClosing(UI::Dialog::ClosingReason ClosingReasong);
		void _OnSaveGameDialogDestroying(UI::Event & DestroyingEvent);
		// member variables
		bool _DestroyOnESCAPEKey;
		bool _DestroyOnLoadGameDialogDestroy;
		bool _DestroyOnLoadScenarioDialogDestroy;
		bool _DestroyOnSaveGameDialogDestroy;
		UI::TextButton * _LoadGameButton;
		UI::LoadGameDialog * _LoadGameDialog;
		UI::TextButton * _LoadScenarioButton;
		UI::LoadScenarioDialog * _LoadScenarioDialog;
		UI::TextButton * _NewGameButton;
		UI::TextButton * _QuitButton;
		UI::TextButton * _ResumeGameButton;
		UI::TextButton * _SaveGameButton;
		UI::SaveGameDialog * _SaveGameDialog;
		ScenarioManager * _ScenarioManager;
	};

	inline UI::TextButton * MainMenuWindow::GetLoadGameButton(void)
	{
		return _LoadGameButton;
	}
	
	inline UI::TextButton * MainMenuWindow::GetLoadScenarioButton(void)
	{
		return _LoadScenarioButton;
	}

	inline UI::TextButton * MainMenuWindow::GetNewGameButton(void)
	{
		return _NewGameButton;
	}

	inline UI::TextButton * MainMenuWindow::GetQuitButton(void)
	{
		return _QuitButton;
	}

	inline UI::TextButton * MainMenuWindow::GetResumeGameButton(void)
	{
		return _ResumeGameButton;
	}

	inline UI::TextButton * MainMenuWindow::GetSaveGameButton(void)
	{
		return _SaveGameButton;
	}
	
	inline void MainMenuWindow::SetDestroyOnESCAPEKey(bool DestroyOnESCAPEKey)
	{
		_DestroyOnESCAPEKey = DestroyOnESCAPEKey;
	}
}

#endif
