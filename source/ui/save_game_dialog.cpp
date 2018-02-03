/**
 * galactic-fall
 * Copyright (C) 2007-2018  Hagen Möbius
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

#include <expressions/operators.h>

#include "../file_handling.h"
#include "../globals.h"
#include "../graphics/color_rgbo.h"
#include "../real_time.h"
#include "key_event.h"
#include "label.h"
#include "list_box.h"
#include "list_box_text_item.h"
#include "mouse_button_event.h"
#include "save_game_dialog.h"
#include "text_button.h"

using namespace Expressions::Operators;

UI::SaveGameDialog::SaveGameDialog(void)
{
	SetTitle("Save Game");
	ConnectKeyCallback(std::bind(&UI::SaveGameDialog::_OnKey, this, std::placeholders::_1));
	
	auto OKButton{new UI::TextButton{this, "OK"}};
	
	OKButton->SetLeft(constant(GetWidth() - 110.0f));
	OKButton->SetTop(constant(GetHeight() - 30.0f));
	OKButton->SetWidth(100.0_c);
	OKButton->SetHeight(20.0_c);
	OKButton->SetAnchorBottom(true);
	OKButton->SetAnchorLeft(false);
	OKButton->SetAnchorRight(true);
	OKButton->SetAnchorTop(false);
	OKButton->ConnectClickedCallback(std::bind(&UI::SaveGameDialog::_Close, this, UI::Dialog::ClosingReason::OK_BUTTON));
	
	auto CancelButton{new UI::TextButton{this, "Cancel"}};
	
	CancelButton->SetLeft(constant(GetWidth() - 220.0f));
	CancelButton->SetTop(constant(GetHeight() - 30.0f));
	CancelButton->SetWidth(100.0_c);
	CancelButton->SetHeight(20.0_c);
	CancelButton->SetAnchorBottom(true);
	CancelButton->SetAnchorLeft(false);
	CancelButton->SetAnchorRight(true);
	CancelButton->SetAnchorTop(false);
	CancelButton->ConnectClickedCallback(std::bind(&UI::SaveGameDialog::_Close, this, UI::Dialog::ClosingReason::CANCEL_BUTTON));
	_MessageLabel = new UI::Label{this};
	_MessageLabel->SetLeft(10.0_c);
	_MessageLabel->SetTop(40.0_c);
	_MessageLabel->SetWidth(constant(GetWidth() - 20.0f));
	_MessageLabel->SetHeight(30.0_c);
	_MessageLabel->SetTextColor(Graphics::ColorRGBO(1.0f, 0.3, 0.3f, 1.0f));
	_MessageLabel->SetAnchorBottom(false);
	_MessageLabel->SetAnchorLeft(true);
	_MessageLabel->SetAnchorRight(true);
	_MessageLabel->SetAnchorTop(true);
	_MessageLabel->SetWrap(true);
	_MessageLabel->SetWordWrap(true);
	_MessageLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	_FileNameLabel = new UI::Label{this};
	_FileNameLabel->SetLeft(10.0_c);
	_FileNameLabel->SetTop(80.0_c);
	_FileNameLabel->SetWidth(constant(GetWidth() - 20.0f));
	_FileNameLabel->SetHeight(20.0_c);
	_FileNameLabel->SetTextColor(Graphics::ColorRGBO(1.0f, 1.0f, 0.5f, 1.0f));
	_FileNameLabel->SetBackgroundColor(Graphics::ColorRGBO(0.1f, 0.1f, 0.1f, 1.0f));
	_FileNameLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	_FileNameLabel->SetAnchorRight(true);
	_FileNameLabel->ConnectKeyCallback(std::bind(&UI::SaveGameDialog::_OnFileNameLabelKey, this, std::placeholders::_1));
	_FileNameLabel->GrabKeyFocus();
	_FileListBox = new UI::ListBox{};
	_FileListBox->SetLeft(10.0_c);
	_FileListBox->SetTop(110.0_c);
	_FileListBox->SetWidth(constant(GetWidth() - 20.0f));
	_FileListBox->SetHeight(constant(GetHeight() - 170.0f));
	_FileListBox->SetAnchorBottom(true);
	_FileListBox->SetAnchorRight(true);
	_FileListBox->SetAnchorTop(true);
	_FileListBox->SetHorizontalScrollBarVisible(false);
	_FileListBox->GetContent()->SetWidth(constant(_FileListBox->GetView()->GetWidth()));
	_FileListBox->GetContent()->SetAnchorRight(true);
	_FileListBox->ConnectSelectedItemChangedCallback(std::bind(&UI::SaveGameDialog::_OnFileListBoxSelectedItemChanged, this));
	AddSubWidget(_FileListBox);
}

std::string UI::SaveGameDialog::GetFilePath(void)
{
	if(_FileNameLabel->GetText() == "")
	{
		return "";
	}
	else
	{
		return _DirectoryPath + '/' + _FileNameLabel->GetText() + ".xml";
	}
}

void UI::SaveGameDialog::SetDirectoryPath(const std::string & DirectoryPath)
{
	_DirectoryPath = DirectoryPath;
	while(_FileListBox->GetContent()->GetSubWidgets().empty() == false)
	{
		_FileListBox->GetContent()->GetSubWidgets().front()->Destroy();
	}
	if(IsExistingDirectory(_DirectoryPath) == false)
	{
		ShowErrorMessage("Is not an existing directory: \"" + _DirectoryPath + "\".");
	}
	for(auto DirectoryEntry : GetDirectoryEntries(_DirectoryPath))
	{
		auto ListBoxTextItem{new UI::ListBoxTextItem{}};
		
		ListBoxTextItem->SetText(DirectoryEntry.substr(0, DirectoryEntry.rfind(".xml")));
		_FileListBox->GetContent()->AddSubWidget(ListBoxTextItem);
	}
}

void UI::SaveGameDialog::ShowErrorMessage(const std::string & ErrorMessage)
{
	if(_MessageTimeoutNotification.IsValid() == true)
	{
		_MessageTimeoutNotification.Dismiss();
	}
	_MessageLabel->SetVisible(true);
	_MessageLabel->SetText(ErrorMessage);
	_MessageTimeoutNotification = g_RealTimeTimeoutNotifications->Add(RealTime::Get() + 2.0f, std::bind(&UI::Label::SetVisible, _MessageLabel, false));
}

void UI::SaveGameDialog::_OnFileNameLabelTextChanged(void)
{
	for(auto ListBoxItem : _FileListBox->GetContent()->GetSubWidgets())
	{
		auto ListBoxTextItem(dynamic_cast< UI::ListBoxTextItem * >(ListBoxItem));
		
		if(ListBoxTextItem != nullptr)
		{
			if(ListBoxTextItem->GetText() == _FileNameLabel->GetText())
			{
				_FileListBox->SetSelectedItem(ListBoxTextItem);
				
				break;
			}
		}
	}
}

void UI::SaveGameDialog::_OnFileListBoxSelectedItemChanged(void)
{
	auto SelectedFileListBoxItem{dynamic_cast< UI::ListBoxTextItem * >(_FileListBox->GetSelectedItem())};
	
	if(SelectedFileListBoxItem != nullptr)
	{
		_FileNameLabel->SetText(SelectedFileListBoxItem->GetText());
	}
	else
	{
		_FileNameLabel->SetText("");
	}
}

void UI::SaveGameDialog::_OnFileNameLabelKey(UI::KeyEvent & KeyEvent)
{
	if(KeyEvent.GetPhase() == UI::Event::Phase::Target)
	{
		if((KeyEvent.GetKeyCode() == 22 /* BACKSPACE */) && (KeyEvent.IsDown() == true))
		{
			if(_FileNameLabel->GetText().length() > 0)
			{
				_FileNameLabel->SetText(_FileNameLabel->GetText().substr(0, _FileNameLabel->GetText().length() - 1));
				_OnFileNameLabelTextChanged();
			}
		}
		else if((KeyEvent.GetString().empty() == false) && (KeyEvent.IsDown() == true))
		{
			_FileNameLabel->SetText(_FileNameLabel->GetText() + KeyEvent.GetString());
			_OnFileNameLabelTextChanged();
		}
		KeyEvent.StopPropagation();
	}
}

void UI::SaveGameDialog::_OnKey(UI::KeyEvent & KeyEvent)
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
