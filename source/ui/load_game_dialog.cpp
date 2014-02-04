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
#include "../key_event_information.h"
#include "../real_time.h"
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
	m_SelectedDirectoryEntryItem(0)
{
	GetTitleLabel()->SetText("Load Game");
	SetPosition(Vector2f(120.0f, 200.0f));
	SetSize(Vector2f(300.0f, 400.0f));
	ConnectKeyCallback(Callback(this, &LoadGameDialog::OnKey));
	m_OKButton = new UI::TextButton(this, "OK");
	m_OKButton->SetSize(Vector2f(100.0f, 20.0f));
	m_OKButton->SetPosition(Vector2f(GetSize()[0] - 10.0f - m_OKButton->GetSize()[0], GetSize()[1] - 10.0f - m_OKButton->GetSize()[1]));
	m_OKButton->SetAnchorBottom(true);
	m_OKButton->SetAnchorLeft(false);
	m_OKButton->SetAnchorRight(true);
	m_OKButton->SetAnchorTop(false);
	m_OKButton->ConnectClickedCallback(Bind1(Callback(dynamic_cast< Dialog * >(this), &LoadGameDialog::_Close), Dialog::OK_BUTTON));
	m_CancelButton = new UI::TextButton(this, "Cancel");
	m_CancelButton->SetSize(Vector2f(100.0f, 20.0f));
	m_CancelButton->SetPosition(Vector2f(GetSize()[0] - 10.0f - m_OKButton->GetSize()[0] - 10.0f - m_CancelButton->GetSize()[0], GetSize()[1] - 10.0f - m_CancelButton->GetSize()[1]));
	m_CancelButton->SetAnchorBottom(true);
	m_CancelButton->SetAnchorLeft(false);
	m_CancelButton->SetAnchorRight(true);
	m_CancelButton->SetAnchorTop(false);
	m_CancelButton->ConnectClickedCallback(Bind1(Callback(dynamic_cast< Dialog * >(this), &LoadGameDialog::_Close), Dialog::CANCEL_BUTTON));
	m_ErrorMessage = new UI::Label(this);
	m_ErrorMessage->SetPosition(Vector2f(10.0f, 40.0f));
	m_ErrorMessage->SetSize(Vector2f(GetSize()[0] - 10.0f - 10.0f, 30.0f));
	m_ErrorMessage->SetTextColor(Color(1.0f, 0.3, 0.3f, 1.0f));
	m_ErrorMessage->SetAnchorBottom(false);
	m_ErrorMessage->SetAnchorLeft(true);
	m_ErrorMessage->SetAnchorRight(true);
	m_ErrorMessage->SetAnchorTop(true);
	m_ErrorMessage->SetWrap(true);
	m_ErrorMessage->SetWordWrap(true);
	m_ErrorMessage->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	m_FileNameLabel = new UI::Label(this);
	m_FileNameLabel->SetPosition(Vector2f(10.0f, 80.0f));
	m_FileNameLabel->SetSize(Vector2f(GetSize()[0] - 10.0f - 10.0f, 20.0f));
	m_FileNameLabel->SetTextColor(Color(1.0f, 1.0f, 0.5f, 1.0f));
	m_FileNameLabel->SetBackgroundColor(Color(0.1f, 0.1f, 0.1f, 1.0f));
	m_FileNameLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	m_FileNameLabel->SetAnchorRight(true);
	m_FileNameLabel->ConnectKeyCallback(Callback(this, &LoadGameDialog::OnFileNameLabelKey));
	m_FileNameLabel->GrabKeyFocus();
	m_FileScrollBox = new UI::ScrollBox(this);
	m_FileScrollBox->SetPosition(Vector2f(10.0f, 110.0f));
	m_FileScrollBox->SetSize(Vector2f(GetSize()[0] - 10.0f - 10.0f, GetSize()[1] - 110.0f - 30.0f - 10.0f - m_OKButton->GetSize()[1]));
	m_FileScrollBox->SetAnchorBottom(true);
	m_FileScrollBox->SetAnchorRight(true);
	m_FileScrollBox->SetAnchorTop(true);
	m_FileScrollBox->SetHorizontalScrollBarVisible(false);
}

std::string UI::LoadGameDialog::GetFilePath(void)
{
	if(m_FileNameLabel->GetText() == "")
	{
		return "";
	}
	else
	{
		return _DirectoryPath + '/' + m_FileNameLabel->GetText() + ".xml";
	}
}

void UI::LoadGameDialog::SetDirectoryPath(const std::string & DirectoryPath)
{
	_DirectoryPath = DirectoryPath;
	while(m_FileScrollBox->GetContent()->GetSubWidgets().empty() == false)
	{
		m_FileScrollBox->GetContent()->GetSubWidgets().front()->Destroy();
	}
	if(IsExistingDirectory(_DirectoryPath) == false)
	{
		ShowErrorMessage("Is not an existing directory: \"" + _DirectoryPath + "\".");
	}
	
	float Top(5.0f);
	std::vector< std::string > Entries(GetDirectoryEntries(_DirectoryPath));
	
	for(std::vector< std::string >::iterator EntryIterator = Entries.begin(); EntryIterator != Entries.end(); ++EntryIterator)
	{
		DirectoryEntryItem * EntryLabel(new DirectoryEntryItem(m_FileScrollBox->GetContent(), EntryIterator->substr(0, EntryIterator->rfind(".xml"))));
		
		EntryLabel->SetPosition(Vector2f(5.0f, Top));
		EntryLabel->SetSize(Vector2f(m_FileScrollBox->GetContent()->GetSize()[0] - 10.0f, 20.0f));
		EntryLabel->SetAnchorRight(true);
		EntryLabel->ConnectMouseButtonCallback(Bind1(Callback(this, &LoadGameDialog::OnDirectoryEntryItemMouseButton), EntryLabel));
		Top += 25.0f;
	}
	m_FileScrollBox->GetContent()->SetSize(Vector2f(m_FileScrollBox->GetView()->GetSize()[0], std::max(Top, m_FileScrollBox->GetView()->GetSize()[1])));
}

void UI::LoadGameDialog::ShowErrorMessage(const std::string & ErrorMessage)
{
	if(m_ErrorMessageTimeoutNotification.IsValid() == true)
	{
		m_ErrorMessageTimeoutNotification.Dismiss();
	}
	m_ErrorMessage->SetVisible(true);
	m_ErrorMessage->SetText(ErrorMessage);
	m_ErrorMessageTimeoutNotification = g_RealTimeTimeoutNotifications->Add(RealTime::Get() + 2.0f, Callback(this, &LoadGameDialog::HideErrorMessage));
}

void UI::LoadGameDialog::HideErrorMessage(void)
{
	m_ErrorMessage->SetVisible(false);
}

void UI::LoadGameDialog::_OnFileNameLabelTextChanged(void)
{
	if(m_SelectedDirectoryEntryItem != 0)
	{
		m_SelectedDirectoryEntryItem->SetSelected(false);
		m_SelectedDirectoryEntryItem = 0;
	}
	
	const std::list< UI::Widget * > & ContentSubWidgets(m_FileScrollBox->GetContent()->GetSubWidgets());
	
	for(std::list< UI::Widget * >::const_iterator ContentSubWidgetIterator = ContentSubWidgets.begin(); ContentSubWidgetIterator != ContentSubWidgets.end(); ++ContentSubWidgetIterator)
	{
		DirectoryEntryItem * EntryLabel(dynamic_cast< DirectoryEntryItem * >(*ContentSubWidgetIterator));
		
		if(EntryLabel != 0)
		{
			if(EntryLabel->GetCaption() == m_FileNameLabel->GetText())
			{
				m_SelectedDirectoryEntryItem = EntryLabel;
				m_SelectedDirectoryEntryItem->SetSelected(true);
				
				return;
			}
		}
	}
}

bool UI::LoadGameDialog::OnFileNameLabelKey(const KeyEventInformation & KeyEventInformation)
{
	if((KeyEventInformation.GetKeyCode() == 9 /* ESCAPE */) || (KeyEventInformation.GetKeyCode() == 36 /* RETURN */))
	{
		// do not eat RETURN or ESCAPE keys
		return false;
	}
	if((KeyEventInformation.GetKeyCode() == 22 /* BACKSPACE */) && (KeyEventInformation.IsDown() == true))
	{
		if(m_FileNameLabel->GetText().length() > 0)
		{
			m_FileNameLabel->SetText(m_FileNameLabel->GetText().substr(0, m_FileNameLabel->GetText().length() - 1));
			_OnFileNameLabelTextChanged();
		}
	}
	else if((KeyEventInformation.GetString().empty() == false) && (KeyEventInformation.IsDown() == true))
	{
		m_FileNameLabel->SetText(m_FileNameLabel->GetText() + KeyEventInformation.GetString());
		_OnFileNameLabelTextChanged();
	}
	
	// eat all input
	return true;
}

bool UI::LoadGameDialog::OnKey(const KeyEventInformation & KeyEventInformation)
{
	if((KeyEventInformation.GetKeyCode() == 9 /* ESCAPE */) && (KeyEventInformation.IsDown() == true))
	{
		_Close(ESCAPE_KEY);
		
		return true;
	}
	else if((KeyEventInformation.GetKeyCode() == 36 /* RETURN */) && (KeyEventInformation.IsDown() == true))
	{
		_Close(RETURN_KEY);
		
		return true;
	}
	
	return false;
}

bool UI::LoadGameDialog::OnDirectoryEntryItemMouseButton(UI::DirectoryEntryItem * DirectoryEntryItem, int Button, int State, float X, float Y)
{
	if((Button == 1 /* LEFT */) && (State == EV_DOWN))
	{
		if(m_SelectedDirectoryEntryItem != 0)
		{
			m_SelectedDirectoryEntryItem->SetSelected(false);
		}
		m_SelectedDirectoryEntryItem = DirectoryEntryItem;
		m_SelectedDirectoryEntryItem->SetSelected(true);
		m_FileNameLabel->SetText(m_SelectedDirectoryEntryItem->GetCaption());
		
		return true;
	}
	
	return false;
}
