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

#include <stdlib.h>

#include <cassert>
#include <fstream>

#include <expressions/operators.h>

#include "../file_handling.h"
#include "../scenario.h"
#include "key_event.h"
#include "label.h"
#include "load_game_dialog.h"
#include "load_scenario_dialog.h"
#include "main_menu_window.h"
#include "save_game_dialog.h"
#include "text_button.h"

using namespace Expressions::Operators;

// these functions are defined in main.cpp
void ActionQuitGameLoop(void);
bool LoadGameFromInputStream(std::istream & InputStream);
bool LoadScenario(const Scenario * Scenario);
bool LoadScenarioFromScenarioIdentifier(const std::string & ScenarioIdentifier);
void SaveGame(std::ostream & OStream);

UI::MainMenuWindow::MainMenuWindow(ScenarioManager * ScenarioManager) :
	_DestroyOnESCAPEKey(true),
	_DestroyOnLoadGameDialogDestroy(false),
	_DestroyOnLoadScenarioDialogDestroy(false),
	_DestroyOnSaveGameDialogDestroy(false),
	_LoadGameButton(nullptr),
	_LoadGameDialog(nullptr),
	_LoadScenarioButton(nullptr),
	_LoadScenarioDialog(nullptr),
	_NewGameButton(nullptr),
	_QuitButton(nullptr),
	_ResumeGameButton(nullptr),
	_SaveGameButton(nullptr),
	_SaveGameDialog(nullptr),
	_ScenarioManager(ScenarioManager)
{
	SetTitle("Galactic Fall");
	ConnectKeyCallback(std::bind(&UI::MainMenuWindow::_OnKey, this, std::placeholders::_1));
	HideResizeDragBox();
	// create components
	_LoadGameButton = new UI::TextButton{};
	_LoadScenarioButton = new UI::TextButton{};
	_NewGameButton = new UI::TextButton{};
	_QuitButton = new UI::TextButton{};
	_ResumeGameButton = new UI::TextButton{};
	_SaveGameButton = new UI::TextButton{};
	
	// initialize components
	_ResumeGameButton->SetName("resume_game");
	_ResumeGameButton->SetLeft(20.0_c);
	_ResumeGameButton->SetTop(60.0_c);
	_ResumeGameButton->SetWidth(width(this) - 40.0_c);
	_ResumeGameButton->SetHeight(20.0_c);
	_ResumeGameButton->SetText("Resume Game");
	_ResumeGameButton->ConnectClickedCallback(std::bind(&UI::MainMenuWindow::_OnResumeGameButtonClicked, this));
	_NewGameButton->SetName("new_game");
	_NewGameButton->SetLeft(20.0_c);
	_NewGameButton->SetTop(bottom(_ResumeGameButton) + 20.0_c);
	_NewGameButton->SetWidth(width(this) - 40.0_c);
	_NewGameButton->SetHeight(20.0_c);
	_NewGameButton->SetText("New Game");
	_NewGameButton->ConnectClickedCallback(std::bind(&UI::MainMenuWindow::_OnNewGameButtonClicked, this));
	_LoadScenarioButton->SetName("load_scenario");
	_LoadScenarioButton->SetLeft(20.0_c);
	_LoadScenarioButton->SetTop(bottom(_NewGameButton) + 20.0_c);
	_LoadScenarioButton->SetWidth(width(this) - 40.0_c);
	_LoadScenarioButton->SetHeight(20.0_c);
	_LoadScenarioButton->SetText("Load Scenario");
	_LoadScenarioButton->ConnectClickedCallback(std::bind(&UI::MainMenuWindow::_OnLoadScenarioButtonClicked, this));
	_LoadGameButton->SetName("load_game");
	_LoadGameButton->SetLeft(20.0_c);
	_LoadGameButton->SetTop(bottom(_LoadScenarioButton) + 20.0_c);
	_LoadGameButton->SetWidth(width(this) - 40.0_c);
	_LoadGameButton->SetHeight(20.0_c);
	_LoadGameButton->SetText("Load Game");
	_LoadGameButton->ConnectClickedCallback(std::bind(&UI::MainMenuWindow::_OnLoadGameButtonClicked, this));
	_SaveGameButton->SetName("save_game");
	_SaveGameButton->SetLeft(20.0_c);
	_SaveGameButton->SetTop(bottom(_LoadGameButton) + 20.0_c);
	_SaveGameButton->SetWidth(width(this) - 40.0_c);
	_SaveGameButton->SetHeight(20.0_c);
	_SaveGameButton->SetText("Save Game");
	_SaveGameButton->ConnectClickedCallback(std::bind(&UI::MainMenuWindow::_OnSaveGameButtonClicked, this));
	_QuitButton->SetName("quit");
	_QuitButton->SetLeft(20.0_c);
	_QuitButton->SetTop(bottom(_SaveGameButton) + 20.0_c);
	_QuitButton->SetWidth(width(this) - 40.0_c);
	_QuitButton->SetHeight(20.0_c);
	_QuitButton->SetText("Quit");
	_QuitButton->ConnectClickedCallback(std::bind(&UI::MainMenuWindow::_OnQuitButtonClicked, this));
	// add components
	AddSubWidget(_LoadGameButton);
	AddSubWidget(_LoadScenarioButton);
	AddSubWidget(_NewGameButton);
	AddSubWidget(_QuitButton);
	AddSubWidget(_ResumeGameButton);
	AddSubWidget(_SaveGameButton);
}

void UI::MainMenuWindow::_ActionNewGame(void)
{
	LoadScenarioFromScenarioIdentifier("new_game");
	Destroy();
}

void UI::MainMenuWindow::_ActionQuit(void)
{
	ActionQuitGameLoop();
	Destroy();
}

void UI::MainMenuWindow::_OnKey(UI::KeyEvent & KeyEvent)
{
	if(KeyEvent.GetPhase() == UI::Event::Phase::Target)
	{
		if((KeyEvent.GetKeyCode() == 9 /* ESCAPE */) && (KeyEvent.IsDown() == true))
		{
			if(_DestroyOnESCAPEKey == true)
			{
				Destroy();
			}
		}
		else if((KeyEvent.GetKeyCode() == 24 /* Q */) && (KeyEvent.IsDown() == true))
		{
			_ActionQuit();
		}
		else if((KeyEvent.GetKeyCode() == 57 /* N */) && (KeyEvent.IsDown() == true))
		{
			_ActionNewGame();
		}
	}
}

void UI::MainMenuWindow::_OnLoadGameButtonClicked(void)
{
	if(_LoadGameDialog == nullptr)
	{
		_LoadGameDialog = new UI::LoadGameDialog{};
		_LoadGameDialog->SetName("load_game");
		_LoadGameDialog->SetLeft(120.0_c);
		_LoadGameDialog->SetTop(200.0_c);
		_LoadGameDialog->SetWidth(300.0_c);
		_LoadGameDialog->SetHeight(400.0_c);
		_LoadGameDialog->ConnectClosingCallback(std::bind(&UI::MainMenuWindow::_OnLoadGameDialogClosing, this, std::placeholders::_1));
		_LoadGameDialog->ConnectDestroyingCallback(std::bind(&UI::MainMenuWindow::_OnLoadGameDialogDestroying, this, std::placeholders::_1));
		
		std::string DirectoryPath(getenv("HOME"));
		
		DirectoryPath += "/.galactic-fall";
		if(IsExistingDirectory(DirectoryPath) == false)
		{
			CreateDirectory(DirectoryPath);
		}
		_LoadGameDialog->SetDirectoryPath(DirectoryPath);
		_DestroyOnLoadGameDialogDestroy = false;
		GetRootWidget()->AddSubWidget(_LoadGameDialog);
		_LoadGameDialog->GrabKeyFocus();
	}
}

bool UI::MainMenuWindow::_OnLoadGameDialogClosing(UI::Dialog::ClosingReason ClosingReason)
{
	if((ClosingReason == UI::Dialog::ClosingReason::CANCEL_BUTTON) || (ClosingReason == UI::Dialog::ClosingReason::ESCAPE_KEY))
	{
		return true;
	}
	else if((ClosingReason == UI::Dialog::ClosingReason::OK_BUTTON) || (ClosingReason == UI::Dialog::ClosingReason::RETURN_KEY))
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
		std::cerr << "Unknown closing reason '" << static_cast< std::underlying_type< UI::Dialog::ClosingReason >::type >(ClosingReason) << "'." << std::endl;
		assert(false);
	}
	
	return false;
}

void UI::MainMenuWindow::_OnLoadGameDialogDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		_LoadGameDialog = nullptr;
		if(_DestroyOnLoadGameDialogDestroy == true)
		{
			Destroy();
		}
	}
}

void UI::MainMenuWindow::_OnLoadScenarioButtonClicked(void)
{
	if(_LoadScenarioDialog == nullptr)
	{
		_LoadScenarioDialog = new UI::LoadScenarioDialog{_ScenarioManager};
		_LoadScenarioDialog->SetName("load_scenario");
		_LoadScenarioDialog->SetLeft(120.0_c);
		_LoadScenarioDialog->SetTop(200.0_c);
		_LoadScenarioDialog->SetWidth(300.0_c);
		_LoadScenarioDialog->SetHeight(400.0_c);
		_LoadScenarioDialog->ConnectClosingCallback(std::bind(&UI::MainMenuWindow::_OnLoadScenarioDialogClosing, this, std::placeholders::_1));
		_LoadScenarioDialog->ConnectDestroyingCallback(std::bind(&UI::MainMenuWindow::_OnLoadScenarioDialogDestroying, this, std::placeholders::_1));
		_DestroyOnLoadScenarioDialogDestroy = false;
		GetRootWidget()->AddSubWidget(_LoadScenarioDialog);
		_LoadScenarioDialog->GrabKeyFocus();
	}
}

bool UI::MainMenuWindow::_OnLoadScenarioDialogClosing(UI::Dialog::ClosingReason ClosingReason)
{
	if((ClosingReason == UI::Dialog::ClosingReason::CANCEL_BUTTON) || (ClosingReason == UI::Dialog::ClosingReason::ESCAPE_KEY))
	{
		return true;
	}
	else if((ClosingReason == UI::Dialog::ClosingReason::OK_BUTTON) || (ClosingReason == UI::Dialog::ClosingReason::RETURN_KEY))
	{
		auto Scenario(_LoadScenarioDialog->GetScenario());
		
		if(Scenario != nullptr)
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
		std::cerr << "Unknown closing reason '" << static_cast< std::underlying_type< UI::Dialog::ClosingReason >::type >(ClosingReason) << "'." << std::endl;
		assert(false);
	}
	
	return false;
}

void UI::MainMenuWindow::_OnLoadScenarioDialogDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		_LoadScenarioDialog = nullptr;
		if(_DestroyOnLoadScenarioDialogDestroy == true)
		{
			Destroy();
		}
	}
}

void UI::MainMenuWindow::_OnNewGameButtonClicked(void)
{
	_ActionNewGame();
}

void UI::MainMenuWindow::_OnResumeGameButtonClicked(void)
{
	Destroy();
}

void UI::MainMenuWindow::_OnSaveGameButtonClicked(void)
{
	if(_SaveGameDialog == nullptr)
	{
		_SaveGameDialog = new UI::SaveGameDialog{};
		_SaveGameDialog->SetName("save_game");
		_SaveGameDialog->SetLeft(120.0_c);
		_SaveGameDialog->SetTop(200.0_c);
		_SaveGameDialog->SetWidth(300.0_c);
		_SaveGameDialog->SetHeight(400.0_c);
		_SaveGameDialog->ConnectClosingCallback(std::bind(&UI::MainMenuWindow::_OnSaveGameDialogClosing, this, std::placeholders::_1));
		_SaveGameDialog->ConnectDestroyingCallback(std::bind(&UI::MainMenuWindow::_OnSaveGameDialogDestroying, this, std::placeholders::_1));
		
		std::string DirectoryPath(getenv("HOME"));
		
		DirectoryPath += "/.galactic-fall";
		if(IsExistingDirectory(DirectoryPath) == false)
		{
			CreateDirectory(DirectoryPath);
		}
		_SaveGameDialog->SetDirectoryPath(DirectoryPath);
		_DestroyOnSaveGameDialogDestroy = false;
		GetRootWidget()->AddSubWidget(_SaveGameDialog);
		_SaveGameDialog->GrabKeyFocus();
	}
}

bool UI::MainMenuWindow::_OnSaveGameDialogClosing(UI::Dialog::ClosingReason ClosingReason)
{
	if((ClosingReason == UI::Dialog::ClosingReason::CANCEL_BUTTON) || (ClosingReason == UI::Dialog::ClosingReason::ESCAPE_KEY))
	{
		return true;
	}
	else if((ClosingReason == UI::Dialog::ClosingReason::OK_BUTTON) || (ClosingReason == UI::Dialog::ClosingReason::RETURN_KEY))
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
		std::cerr << "Unknown closing reason '" << static_cast< std::underlying_type< UI::Dialog::ClosingReason >::type >(ClosingReason) << "'." << std::endl;
		assert(false);
	}
	
	return false;
}

void UI::MainMenuWindow::_OnSaveGameDialogDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		_SaveGameDialog = nullptr;
		if(_DestroyOnSaveGameDialogDestroy == true)
		{
			Destroy();
		}
	}
}

void UI::MainMenuWindow::_OnQuitButtonClicked(void)
{
	_ActionQuit();
}
