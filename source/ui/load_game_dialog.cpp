/**
 * galactic-fall
 * Copyright (C) 2008  Hagen Möbius
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

#include "../color.h"
#include "../file_handling.h"
#include "../globals.h"
#include "../real_time.h"
#include "key_event.h"
#include "label.h"
#include "load_game_dialog.h"
#include "scroll_box.h"
#include "text_button.h"

// this class is declared here, so we can use it, but defined in save_game_dialog.cpp
namespace UI
{
	class DirectoryEntryItem : public UI::Widget
	{
	public:
		DirectoryEntryItem(UI::Widget * SupWidget, const std::string & Caption);
		// getters
		const std::string & GetCaption(void) const;
		// setters
		void SetSelected(bool Selected);
	private:
		// callbacks
		void _OnMouseEnter(void);
		void _OnMouseLeave(void);
		// member variables
		bool _Selected;
		UI::Label * _CaptionLabel;
	};
}

UI::LoadGameDialog::LoadGameDialog(UI::Widget * SupWidget) :
	UI::Dialog(SupWidget),
	_SelectedDirectoryEntryItem(nullptr)
{
	GetTitleLabel()->SetText("Load Game");
	SetPosition(Vector2f(120.0f, 200.0f));
	SetSize(Vector2f(300.0f, 400.0f));
	ConnectKeyCallback(std::bind(&UI::LoadGameDialog::_OnKey, this, std::placeholders::_1));
	
	auto OKButton(new UI::TextButton(this, "OK"));
	
	OKButton->SetSize(Vector2f(100.0f, 20.0f));
	OKButton->SetPosition(Vector2f(GetSize()[0] - 10.0f - OKButton->GetSize()[0], GetSize()[1] - 10.0f - OKButton->GetSize()[1]));
	OKButton->SetAnchorBottom(true);
	OKButton->SetAnchorLeft(false);
	OKButton->SetAnchorRight(true);
	OKButton->SetAnchorTop(false);
	OKButton->ConnectClickedCallback(std::bind(&UI::LoadGameDialog::_Close, this, UI::Dialog::ClosingReason::OK_BUTTON));
	
	auto CancelButton(new UI::TextButton(this, "Cancel"));
	
	CancelButton->SetSize(Vector2f(100.0f, 20.0f));
	CancelButton->SetPosition(Vector2f(GetSize()[0] - 10.0f - OKButton->GetSize()[0] - 10.0f - CancelButton->GetSize()[0], GetSize()[1] - 10.0f - CancelButton->GetSize()[1]));
	CancelButton->SetAnchorBottom(true);
	CancelButton->SetAnchorLeft(false);
	CancelButton->SetAnchorRight(true);
	CancelButton->SetAnchorTop(false);
	CancelButton->ConnectClickedCallback(std::bind(&UI::LoadGameDialog::_Close, this, UI::Dialog::ClosingReason::CANCEL_BUTTON));
	_MessageLabel = new UI::Label(this);
	_MessageLabel->SetPosition(Vector2f(10.0f, 40.0f));
	_MessageLabel->SetSize(Vector2f(GetSize()[0] - 10.0f - 10.0f, 30.0f));
	_MessageLabel->SetTextColor(Color(1.0f, 0.3, 0.3f, 1.0f));
	_MessageLabel->SetAnchorBottom(false);
	_MessageLabel->SetAnchorLeft(true);
	_MessageLabel->SetAnchorRight(true);
	_MessageLabel->SetAnchorTop(true);
	_MessageLabel->SetWrap(true);
	_MessageLabel->SetWordWrap(true);
	_MessageLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	_FileNameLabel = new UI::Label(this);
	_FileNameLabel->SetPosition(Vector2f(10.0f, 80.0f));
	_FileNameLabel->SetSize(Vector2f(GetSize()[0] - 10.0f - 10.0f, 20.0f));
	_FileNameLabel->SetTextColor(Color(1.0f, 1.0f, 0.5f, 1.0f));
	_FileNameLabel->SetBackgroundColor(Color(0.1f, 0.1f, 0.1f, 1.0f));
	_FileNameLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	_FileNameLabel->SetAnchorRight(true);
	_FileNameLabel->ConnectKeyCallback(std::bind(&UI::LoadGameDialog::_OnFileNameLabelKey, this, std::placeholders::_1));
	_FileNameLabel->GrabKeyFocus();
	_FileScrollBox = new UI::ScrollBox(this);
	_FileScrollBox->SetPosition(Vector2f(10.0f, 110.0f));
	_FileScrollBox->SetSize(Vector2f(GetSize()[0] - 10.0f - 10.0f, GetSize()[1] - 110.0f - 30.0f - 10.0f - OKButton->GetSize()[1]));
	_FileScrollBox->SetAnchorBottom(true);
	_FileScrollBox->SetAnchorRight(true);
	_FileScrollBox->SetAnchorTop(true);
	_FileScrollBox->SetHorizontalScrollBarVisible(false);
}

std::string UI::LoadGameDialog::GetFilePath(void)
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

void UI::LoadGameDialog::SetDirectoryPath(const std::string & DirectoryPath)
{
	_DirectoryPath = DirectoryPath;
	while(_FileScrollBox->GetContent()->GetSubWidgets().empty() == false)
	{
		_FileScrollBox->GetContent()->GetSubWidgets().front()->Destroy();
	}
	if(IsExistingDirectory(_DirectoryPath) == false)
	{
		ShowErrorMessage("Is not an existing directory: \"" + _DirectoryPath + "\".");
	}
	
	float Top(5.0f);
	
	for(auto Entry : GetDirectoryEntries(_DirectoryPath))
	{
		auto EntryLabel(new DirectoryEntryItem(_FileScrollBox->GetContent(), Entry.substr(0, Entry.rfind(".xml"))));
		
		EntryLabel->SetPosition(Vector2f(5.0f, Top));
		EntryLabel->SetSize(Vector2f(_FileScrollBox->GetContent()->GetSize()[0] - 10.0f, 20.0f));
		EntryLabel->SetAnchorRight(true);
		EntryLabel->ConnectMouseButtonCallback(std::bind(&UI::LoadGameDialog::_OnDirectoryEntryItemMouseButton, this, EntryLabel, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
		Top += 25.0f;
	}
	_FileScrollBox->GetContent()->SetSize(Vector2f(_FileScrollBox->GetView()->GetSize()[0], std::max(Top, _FileScrollBox->GetView()->GetSize()[1])));
}

void UI::LoadGameDialog::ShowErrorMessage(const std::string & ErrorMessage)
{
	if(_MessageTimeoutNotification.IsValid() == true)
	{
		_MessageTimeoutNotification.Dismiss();
	}
	_MessageLabel->SetVisible(true);
	_MessageLabel->SetText(ErrorMessage);
	_MessageTimeoutNotification = g_RealTimeTimeoutNotifications->Add(RealTime::Get() + 2.0f, std::bind(&UI::Label::SetVisible, _MessageLabel, false));
}

void UI::LoadGameDialog::_OnFileNameLabelTextChanged(void)
{
	if(_SelectedDirectoryEntryItem != nullptr)
	{
		_SelectedDirectoryEntryItem->SetSelected(false);
		_SelectedDirectoryEntryItem = nullptr;
	}
	for(auto SubWidget : _FileScrollBox->GetContent()->GetSubWidgets())
	{
		auto EntryLabel(dynamic_cast< DirectoryEntryItem * >(SubWidget));
		
		if(EntryLabel != nullptr)
		{
			if(EntryLabel->GetCaption() == _FileNameLabel->GetText())
			{
				_SelectedDirectoryEntryItem = EntryLabel;
				_SelectedDirectoryEntryItem->SetSelected(true);
				
				return;
			}
		}
	}
}

void UI::LoadGameDialog::_OnFileNameLabelKey(UI::KeyEvent & KeyEvent)
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

void UI::LoadGameDialog::_OnKey(UI::KeyEvent & KeyEvent)
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

bool UI::LoadGameDialog::_OnDirectoryEntryItemMouseButton(UI::DirectoryEntryItem * DirectoryEntryItem, int Button, int State, float X, float Y)
{
	if((Button == 1 /* LEFT */) && (State == EV_DOWN))
	{
		if(_SelectedDirectoryEntryItem != nullptr)
		{
			_SelectedDirectoryEntryItem->SetSelected(false);
		}
		_SelectedDirectoryEntryItem = DirectoryEntryItem;
		_SelectedDirectoryEntryItem->SetSelected(true);
		_FileNameLabel->SetText(_SelectedDirectoryEntryItem->GetCaption());
		
		return true;
	}
	
	return false;
}
