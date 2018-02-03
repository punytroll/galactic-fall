/**
 * galactic-fall
 * Copyright (C) 2008-2018  Hagen Möbius
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
#include "list_box_text_item.h"
#include "load_game_dialog.h"
#include "mouse_button_event.h"
#include "scroll_box.h"
#include "text_button.h"

using namespace Expressions::Operators;

UI::LoadGameDialog::LoadGameDialog(UI::Widget * SupWidget) :
	UI::Dialog(SupWidget),
	_SelectedDirectoryEntryItem(nullptr)
{
	SetTitle("Load Game");
	ConnectKeyCallback(std::bind(&UI::LoadGameDialog::_OnKey, this, std::placeholders::_1));
	
	auto OKButton{new UI::TextButton{this, "OK"}};
	
	OKButton->SetLeft(constant(GetWidth() - 110.0f));
	OKButton->SetTop(constant(GetHeight() - 30.0f));
	OKButton->SetWidth(100.0_c);
	OKButton->SetHeight(20.0_c);
	OKButton->SetAnchorBottom(true);
	OKButton->SetAnchorLeft(false);
	OKButton->SetAnchorRight(true);
	OKButton->SetAnchorTop(false);
	OKButton->ConnectClickedCallback(std::bind(&UI::LoadGameDialog::_Close, this, UI::Dialog::ClosingReason::OK_BUTTON));
	
	auto CancelButton{new UI::TextButton{this, "Cancel"}};
	
	CancelButton->SetLeft(constant(GetWidth() - 220.0f));
	CancelButton->SetTop(constant(GetHeight() - 30.0f));
	CancelButton->SetWidth(100.0_c);
	CancelButton->SetHeight(20.0_c);
	CancelButton->SetAnchorBottom(true);
	CancelButton->SetAnchorLeft(false);
	CancelButton->SetAnchorRight(true);
	CancelButton->SetAnchorTop(false);
	CancelButton->ConnectClickedCallback(std::bind(&UI::LoadGameDialog::_Close, this, UI::Dialog::ClosingReason::CANCEL_BUTTON));
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
	_FileNameLabel->ConnectKeyCallback(std::bind(&UI::LoadGameDialog::_OnFileNameLabelKey, this, std::placeholders::_1));
	_FileNameLabel->GrabKeyFocus();
	_FileScrollBox = new UI::ScrollBox{this};
	_FileScrollBox->SetLeft(10.0_c);
	_FileScrollBox->SetTop(110.0_c);
	_FileScrollBox->SetWidth(constant(GetWidth() - 20.0f));
	_FileScrollBox->SetHeight(constant(GetHeight() - 170.0f));
	_FileScrollBox->SetAnchorBottom(true);
	_FileScrollBox->SetAnchorRight(true);
	_FileScrollBox->SetAnchorTop(true);
	_FileScrollBox->SetHorizontalScrollBarVisible(false);
	_FileScrollBox->GetContent()->SetWidth(constant(_FileScrollBox->GetView()->GetWidth()));
	_FileScrollBox->GetContent()->SetAnchorRight(true);
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
		auto EntryLabel{new UI::ListBoxTextItem{}};
		
		EntryLabel->SetLeft(5.0_c);
		EntryLabel->SetTop(constant(Top));
		EntryLabel->SetWidth(constant(_FileScrollBox->GetContent()->GetWidth() - 10.0f));
		EntryLabel->SetHeight(20.0_c);
		EntryLabel->SetAnchorRight(true);
		EntryLabel->SetText(Entry.substr(0, Entry.rfind(".xml")));
		EntryLabel->ConnectMouseButtonCallback(std::bind(&UI::LoadGameDialog::_OnDirectoryEntryItemMouseButton, this, EntryLabel, std::placeholders::_1));
		_FileScrollBox->GetContent()->AddSubWidget(EntryLabel);
		Top += 25.0f;
	}
	_FileScrollBox->GetContent()->SetHeight(constant(Top));
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
		auto EntryLabel(dynamic_cast< UI::ListBoxTextItem * >(SubWidget));
		
		if(EntryLabel != nullptr)
		{
			if(EntryLabel->GetText() == _FileNameLabel->GetText())
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

void UI::LoadGameDialog::_OnDirectoryEntryItemMouseButton(UI::ListBoxTextItem * DirectoryEntryItem, UI::MouseButtonEvent & MouseButtonEvent)
{
	if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left) && (MouseButtonEvent.IsDown() == true))
	{
		if(_SelectedDirectoryEntryItem != nullptr)
		{
			_SelectedDirectoryEntryItem->SetSelected(false);
		}
		_SelectedDirectoryEntryItem = DirectoryEntryItem;
		_SelectedDirectoryEntryItem->SetSelected(true);
		_FileNameLabel->SetText(_SelectedDirectoryEntryItem->GetText());
	}
}
