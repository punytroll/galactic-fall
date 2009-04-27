/**
 * Copyright (C) 2009  Hagen Möbius
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

class Button;
class LoadGameDialog;
class SaveGameDialog;

class MainMenuWindow : public WWindow
{
public:
	// constructor and destructor
	MainMenuWindow(Widget * SupWidget);
	// getters
	Button * GetLoadGameButton(void);
	Button * GetNewGameButton(void);
	Button * GetQuitButton(void);
	Button * GetResumeGameButton(void);
	Button * GetSaveGameButton(void);
private:
	// callbacks
	bool _OnKey(const KeyEventInformation & KeyEventInformation);
	void _OnLoadGameButtonClicked(void);
	bool _OnLoadGameDialogClosing(Dialog::ClosingReason ClosingReason);
	void _OnLoadGameDialogDestroying(void);
	void _OnNewGameButtonClicked(void);
	void _OnQuitButtonClicked(void);
	void _OnResumeGameButtonClicked(void);
	void _OnSaveGameButtonClicked(void);
	bool _OnSaveGameDialogClosing(Dialog::ClosingReason ClosingReasong);
	void _OnSaveGameDialogDestroying(void);
	// member variables
	bool _DestroyOnLoadGameDialogDestroy;
	bool _DestroyOnSaveGameDialogDestroy;
	Button * _LoadGameButton;
	LoadGameDialog * _LoadGameDialog;
	Button * _NewGameButton;
	Button * _QuitButton;
	Button * _ResumeGameButton;
	Button * _SaveGameButton;
	SaveGameDialog * _SaveGameDialog;
};

inline Button * MainMenuWindow::GetLoadGameButton(void)
{
	return _LoadGameButton;
}

inline Button * MainMenuWindow::GetNewGameButton(void)
{
	return _NewGameButton;
}

inline Button * MainMenuWindow::GetQuitButton(void)
{
	return _QuitButton;
}

inline Button * MainMenuWindow::GetResumeGameButton(void)
{
	return _ResumeGameButton;
}

inline Button * MainMenuWindow::GetSaveGameButton(void)
{
	return _SaveGameButton;
}

#endif
