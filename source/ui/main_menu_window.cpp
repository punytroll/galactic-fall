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

#include <stdlib.h>

#include <fstream>

#include "../callbacks/callbacks.h"
#include "../file_handling.h"
#include "../key_event_information.h"
#include "../scenario.h"
#include "button.h"
#include "label.h"
#include "load_game_dialog.h"
#include "load_scenario_dialog.h"
#include "main_menu_window.h"
#include "save_game_dialog.h"

// these functions are defined in main.cpp
void ActionQuitGameLoop(void);
bool LoadGameFromInputStream(std::istream & InputStream);
bool LoadScenario(const Scenario * Scenario);
bool LoadScenarioFromScenarioIdentifier(const std::string & ScenarioIdentifier);
void SaveGame(std::ostream & OStream);

UI::MainMenuWindow::MainMenuWindow(UI::Widget * SupWidget, ScenarioManager * ScenarioManager) :
	UI::Window(SupWidget, "Galactic Fall"),
	_DestroyOnESCAPEKey(true),
	_DestroyOnLoadGameDialogDestroy(false),
	_DestroyOnLoadScenarioDialogDestroy(false),
	_DestroyOnSaveGameDialogDestroy(false),
	_LoadGameButton(0),
	_LoadGameDialog(0),
	_LoadScenarioButton(0),
	_LoadScenarioDialog(0),
	_NewGameButton(0),
	_QuitButton(0),
	_ResumeGameButton(0),
	_SaveGameButton(0),
	_SaveGameDialog(0),
	_ScenarioManager(ScenarioManager)
{
	ConnectKeyCallback(Callback(this, &UI::MainMenuWindow::_OnKey));
	SetSize(Vector2f(200.0f, 300.0f));
	
	// "Resume Game" button
	_ResumeGameButton = new UI::Button(this);
	_ResumeGameButton->SetPosition(Vector2f(20.0f, 60.0f));
	_ResumeGameButton->SetSize(Vector2f(160.0f, 20.0f));
	_ResumeGameButton->SetAnchorBottom(false);
	_ResumeGameButton->SetAnchorLeft(true);
	_ResumeGameButton->SetAnchorRight(true);
	_ResumeGameButton->SetAnchorTop(true);
	_ResumeGameButton->ConnectClickedCallback(Callback(this, &UI::MainMenuWindow::_OnResumeGameButtonClicked));
	
	UI::Label * ResumeGameButtonLabel(new UI::Label(_ResumeGameButton, "Resume Game"));
	
	ResumeGameButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	ResumeGameButtonLabel->SetSize(_ResumeGameButton->GetSize());
	ResumeGameButtonLabel->SetAnchorBottom(true);
	ResumeGameButtonLabel->SetAnchorLeft(true);
	ResumeGameButtonLabel->SetAnchorRight(true);
	ResumeGameButtonLabel->SetAnchorTop(true);
	ResumeGameButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	ResumeGameButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	
	// "New Game" button
	_NewGameButton = new UI::Button(this);
	_NewGameButton->SetPosition(Vector2f(20.0f, _ResumeGameButton->GetPosition()[1] + 40.0f));
	_NewGameButton->SetSize(Vector2f(160.0f, 20.0f));
	_NewGameButton->SetAnchorBottom(false);
	_NewGameButton->SetAnchorLeft(true);
	_NewGameButton->SetAnchorRight(true);
	_NewGameButton->SetAnchorTop(true);
	_NewGameButton->ConnectClickedCallback(Callback(this, &UI::MainMenuWindow::_OnNewGameButtonClicked));
	
	UI::Label * NewGameButtonLabel(new UI::Label(_NewGameButton, "New Game"));
	
	NewGameButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	NewGameButtonLabel->SetSize(_NewGameButton->GetSize());
	NewGameButtonLabel->SetAnchorBottom(true);
	NewGameButtonLabel->SetAnchorLeft(true);
	NewGameButtonLabel->SetAnchorRight(true);
	NewGameButtonLabel->SetAnchorTop(true);
	NewGameButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	NewGameButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	
	// "Load Scenario" button
	_LoadScenarioButton = new UI::Button(this);
	_LoadScenarioButton->SetPosition(Vector2f(20.0f, _NewGameButton->GetPosition()[1] + 40.0f));
	_LoadScenarioButton->SetSize(Vector2f(160.0f, 20.0f));
	_LoadScenarioButton->SetAnchorBottom(false);
	_LoadScenarioButton->SetAnchorLeft(true);
	_LoadScenarioButton->SetAnchorRight(true);
	_LoadScenarioButton->SetAnchorTop(true);
	_LoadScenarioButton->ConnectClickedCallback(Callback(this, &UI::MainMenuWindow::_OnLoadScenarioButtonClicked));
	
	UI::Label * LoadScenarioButtonLabel(new UI::Label(_LoadScenarioButton, "Load Scenario"));
	
	LoadScenarioButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	LoadScenarioButtonLabel->SetSize(_LoadScenarioButton->GetSize());
	LoadScenarioButtonLabel->SetAnchorBottom(true);
	LoadScenarioButtonLabel->SetAnchorLeft(true);
	LoadScenarioButtonLabel->SetAnchorRight(true);
	LoadScenarioButtonLabel->SetAnchorTop(true);
	LoadScenarioButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	LoadScenarioButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	
	// "Load Game" button
	_LoadGameButton = new UI::Button(this);
	_LoadGameButton->SetPosition(Vector2f(20.0f, _LoadScenarioButton->GetPosition()[1] + 40.0f));
	_LoadGameButton->SetSize(Vector2f(160.0f, 20.0f));
	_LoadGameButton->SetAnchorBottom(false);
	_LoadGameButton->SetAnchorLeft(true);
	_LoadGameButton->SetAnchorRight(true);
	_LoadGameButton->SetAnchorTop(true);
	_LoadGameButton->ConnectClickedCallback(Callback(this, &UI::MainMenuWindow::_OnLoadGameButtonClicked));
	
	UI::Label * LoadGameButtonLabel(new UI::Label(_LoadGameButton, "Load Game"));
	
	LoadGameButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	LoadGameButtonLabel->SetSize(_LoadGameButton->GetSize());
	LoadGameButtonLabel->SetAnchorBottom(true);
	LoadGameButtonLabel->SetAnchorLeft(true);
	LoadGameButtonLabel->SetAnchorRight(true);
	LoadGameButtonLabel->SetAnchorTop(true);
	LoadGameButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	LoadGameButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	
	// "Save Game" button
	_SaveGameButton = new UI::Button(this);
	_SaveGameButton->SetPosition(Vector2f(20.0f, _LoadGameButton->GetPosition()[1] + 40.0f));
	_SaveGameButton->SetSize(Vector2f(160.0f, 20.0f));
	_SaveGameButton->SetAnchorBottom(false);
	_SaveGameButton->SetAnchorLeft(true);
	_SaveGameButton->SetAnchorRight(true);
	_SaveGameButton->SetAnchorTop(true);
	_SaveGameButton->ConnectClickedCallback(Callback(this, &UI::MainMenuWindow::_OnSaveGameButtonClicked));
	
	UI::Label * SaveGameButtonLabel(new UI::Label(_SaveGameButton, "Save Game"));
	
	SaveGameButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	SaveGameButtonLabel->SetSize(_SaveGameButton->GetSize());
	SaveGameButtonLabel->SetAnchorBottom(true);
	SaveGameButtonLabel->SetAnchorLeft(true);
	SaveGameButtonLabel->SetAnchorRight(true);
	SaveGameButtonLabel->SetAnchorTop(true);
	SaveGameButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	SaveGameButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	
	// "Quit" button
	_QuitButton = new UI::Button(this);
	_QuitButton->SetPosition(Vector2f(20.0f, _SaveGameButton->GetPosition()[1] + 40.0f));
	_QuitButton->SetSize(Vector2f(160.0f, 20.0f));
	_QuitButton->SetAnchorBottom(false);
	_QuitButton->SetAnchorLeft(true);
	_QuitButton->SetAnchorRight(true);
	_QuitButton->SetAnchorTop(true);
	_QuitButton->ConnectClickedCallback(Callback(this, &UI::MainMenuWindow::_OnQuitButtonClicked));
	
	UI::Label * QuitButtonLabel(new UI::Label(_QuitButton, "Quit"));
	
	QuitButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	QuitButtonLabel->SetSize(_QuitButton->GetSize());
	QuitButtonLabel->SetAnchorBottom(true);
	QuitButtonLabel->SetAnchorLeft(true);
	QuitButtonLabel->SetAnchorRight(true);
	QuitButtonLabel->SetAnchorTop(true);
	QuitButtonLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	QuitButtonLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
}

bool UI::MainMenuWindow::_OnKey(const KeyEventInformation & KeyEventInformation)
{
	if((KeyEventInformation.GetKeyCode() == 9 /* ESCAPE */) && (KeyEventInformation.IsDown() == true))
	{
		if(_DestroyOnESCAPEKey == true)
		{
			Destroy();
		}
	}
	
	// eat all keys
	return true;
}

void UI::MainMenuWindow::_OnLoadGameButtonClicked(void)
{
	if(_LoadGameDialog == 0)
	{
		_LoadGameDialog = new UI::LoadGameDialog(GetRootWidget());
		_LoadGameDialog->GrabKeyFocus();
		_LoadGameDialog->ConnectClosingCallback(Callback(this, &UI::MainMenuWindow::_OnLoadGameDialogClosing));
		_LoadGameDialog->ConnectDestroyingCallback(Callback(this, &UI::MainMenuWindow::_OnLoadGameDialogDestroying));
		
		std::string DirectoryPath(getenv("HOME"));
		
		DirectoryPath += "/.galactic-fall";
		if(IsExistingDirectory(DirectoryPath) == false)
		{
			CreateDirectory(DirectoryPath);
		}
		_LoadGameDialog->SetDirectoryPath(DirectoryPath);
		_DestroyOnLoadGameDialogDestroy = false;
	}
}

bool UI::MainMenuWindow::_OnLoadGameDialogClosing(UI::Dialog::ClosingReason ClosingReason)
{
	if((ClosingReason == UI::Dialog::CANCEL_BUTTON) || (ClosingReason == UI::Dialog::ESCAPE_KEY))
	{
		return true;
	}
	else if((ClosingReason == UI::Dialog::OK_BUTTON) || (ClosingReason == UI::Dialog::RETURN_KEY))
	{
		std::string FilePath(_LoadGameDialog->GetFilePath());
		
		if(FilePath != "")
		{
			std::ifstream InputFileStream;
	
			InputFileStream.open(FilePath.c_str());
			if(!InputFileStream)
			{
				_LoadGameDialog->ShowErrorMessage("Could not find or read \"" + FilePath + "\".");
				
				return false;
			}
			else
			{
				if(LoadGameFromInputStream(InputFileStream) == false)
				{
					_LoadGameDialog->ShowErrorMessage("Loading the game file \"" + FilePath + "\" failed.");
					
					return false;
				}
				else
				{
					_DestroyOnLoadGameDialogDestroy = true;
					
					return true;
				}
			}
		}
		else
		{
			_LoadGameDialog->ShowErrorMessage("You have not selected any file .");
			
			return false;
		}
	}
	else
	{
		std::cerr << "Unknown closing reason '" << ClosingReason << "'." << std::endl;
		assert(false);
	}
	
	return false;
}

void UI::MainMenuWindow::_OnLoadGameDialogDestroying(void)
{
	_LoadGameDialog = 0;
	if(_DestroyOnLoadGameDialogDestroy == true)
	{
		Destroy();
	}
}

void UI::MainMenuWindow::_OnLoadScenarioButtonClicked(void)
{
	if(_LoadScenarioDialog == 0)
	{
		_LoadScenarioDialog = new UI::LoadScenarioDialog(GetRootWidget(), _ScenarioManager);
		_LoadScenarioDialog->GrabKeyFocus();
		_LoadScenarioDialog->ConnectClosingCallback(Callback(this, &UI::MainMenuWindow::_OnLoadScenarioDialogClosing));
		_LoadScenarioDialog->ConnectDestroyingCallback(Callback(this, &UI::MainMenuWindow::_OnLoadScenarioDialogDestroying));
		_DestroyOnLoadScenarioDialogDestroy = false;
	}
}

bool UI::MainMenuWindow::_OnLoadScenarioDialogClosing(UI::Dialog::ClosingReason ClosingReason)
{
	if((ClosingReason == UI::Dialog::CANCEL_BUTTON) || (ClosingReason == UI::Dialog::ESCAPE_KEY))
	{
		return true;
	}
	else if((ClosingReason == UI::Dialog::OK_BUTTON) || (ClosingReason == UI::Dialog::RETURN_KEY))
	{
		Scenario * Scenario(_LoadScenarioDialog->GetScenario());
		
		if(Scenario != 0)
		{
			if(LoadScenario(Scenario) == false)
			{
				_LoadScenarioDialog->ShowErrorMessage("Loading the scenario \"" + Scenario->GetName() + "\" failed.");
				
				return false;
			}
			else
			{
				_DestroyOnLoadScenarioDialogDestroy = true;
				
				return true;
			}
		}
		else
		{
			_LoadScenarioDialog->ShowErrorMessage("You have not selected a scenario.");
			
			return false;
		}
	}
	else
	{
		std::cerr << "Unknown closing reason '" << ClosingReason << "'." << std::endl;
		assert(false);
	}
	
	return false;
}

void UI::MainMenuWindow::_OnLoadScenarioDialogDestroying(void)
{
	_LoadScenarioDialog = 0;
	if(_DestroyOnLoadScenarioDialogDestroy == true)
	{
		Destroy();
	}
}

void UI::MainMenuWindow::_OnNewGameButtonClicked(void)
{
	LoadScenarioFromScenarioIdentifier("new_game");
	Destroy();
}

void UI::MainMenuWindow::_OnResumeGameButtonClicked(void)
{
	Destroy();
}

void UI::MainMenuWindow::_OnSaveGameButtonClicked(void)
{
	if(_SaveGameDialog == 0)
	{
		_SaveGameDialog = new UI::SaveGameDialog(GetRootWidget());
		_SaveGameDialog->GrabKeyFocus();
		_SaveGameDialog->ConnectClosingCallback(Callback(this, &UI::MainMenuWindow::_OnSaveGameDialogClosing));
		_SaveGameDialog->ConnectDestroyingCallback(Callback(this, &UI::MainMenuWindow::_OnSaveGameDialogDestroying));
		
		std::string DirectoryPath(getenv("HOME"));
		
		DirectoryPath += "/.galactic-fall";
		if(IsExistingDirectory(DirectoryPath) == false)
		{
			CreateDirectory(DirectoryPath);
		}
		_SaveGameDialog->SetDirectoryPath(DirectoryPath);
		_DestroyOnSaveGameDialogDestroy = false;
	}
}

bool UI::MainMenuWindow::_OnSaveGameDialogClosing(UI::Dialog::ClosingReason ClosingReason)
{
	if((ClosingReason == UI::Dialog::CANCEL_BUTTON) || (ClosingReason == UI::Dialog::ESCAPE_KEY))
	{
		return true;
	}
	else if((ClosingReason == UI::Dialog::OK_BUTTON) || (ClosingReason == UI::Dialog::RETURN_KEY))
	{
		std::string FilePath(_SaveGameDialog->GetFilePath());
		
		if(FilePath != "")
		{
			std::ofstream OutputFileStream;
	
			OutputFileStream.open(FilePath.c_str());
			if(!OutputFileStream)
			{
				_SaveGameDialog->ShowErrorMessage("Could not find or write to \"" + FilePath + "\".");
				
				return false;
			}
			else
			{
				SaveGame(OutputFileStream);
				_DestroyOnSaveGameDialogDestroy = true;
				
				return true;
			}
		}
		else
		{
			_SaveGameDialog->ShowErrorMessage("You have not selected any file .");
			
			return false;
		}
	}
	else
	{
		std::cerr << "Unknown closing reason '" << ClosingReason << "'." << std::endl;
		assert(false);
	}
	
	return false;
}

void UI::MainMenuWindow::_OnSaveGameDialogDestroying(void)
{
	_SaveGameDialog = 0;
	if(_DestroyOnSaveGameDialogDestroy == true)
	{
		Destroy();
	}
}

void UI::MainMenuWindow::_OnQuitButtonClicked(void)
{
	ActionQuitGameLoop();
	Destroy();
}
