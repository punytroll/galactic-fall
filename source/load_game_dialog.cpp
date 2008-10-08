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

#include "stdlib.h"

#include <fstream>

#include "button.h"
#include "file_handling.h"
#include "globals.h"
#include "key_event_information.h"
#include "label.h"
#include "load_game_dialog.h"
#include "real_time.h"
#include "scroll_box.h"

// this class is declared here, so we can use it, but defined in save_game_dialog.cpp
class DirectoryEntryItem : public MouseMotionListener, public Widget
{
public:
	DirectoryEntryItem(Widget * SupWidget, const std::string & Caption);
	void Update(void);
	// getters
	const std::string & GetCaption(void) const;
	bool GetSelected(void) const;
	// setters
	void SetSelected(bool Selected);
protected:
	virtual void OnMouseEnter(Widget * EventSource);
	virtual void OnMouseLeave(Widget * EventSource);
private:
	bool m_Selected;
	Label * m_CaptionLabel;
};

LoadGameDialog::LoadGameDialog(Widget * SupWidget, Callback1< bool, std::istream & > LoadGameCallback) :
	WWindow(SupWidget, "Load Game"),
	m_LoadGameCallback(LoadGameCallback),
	m_SelectedDirectoryEntryItem(0)
{
	SetPosition(Vector2f(120.0f, 200.0f));
	SetSize(Vector2f(300.0f, 300.0f));
	AddKeyListener(this);
	AddMouseButtonListener(this);
	m_OKButton = new Button(this);
	m_OKButton->SetPosition(Vector2f(10.0f, 40.0f));
	m_OKButton->SetSize(Vector2f(100.0f, 20.0f));
	m_OKButton->AddClickedListener(this);
	
	Label * OKButtonLabel(new Label(m_OKButton, "OK"));
	
	OKButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	OKButtonLabel->SetSize(m_OKButton->GetSize());
	OKButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	OKButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_CancelButton = new Button(this);
	m_CancelButton->SetPosition(Vector2f(120.0f, 40.0f));
	m_CancelButton->SetSize(Vector2f(100.0f, 20.0f));
	m_CancelButton->AddClickedListener(this);
	
	Label * CancelButtonLabel(new Label(m_CancelButton, "Cancel"));
	
	CancelButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	CancelButtonLabel->SetSize(m_CancelButton->GetSize());
	CancelButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	CancelButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_ErrorMessage = new Label(this);
	m_ErrorMessage->SetPosition(Vector2f(10.0f, 150.0f));
	m_ErrorMessage->SetSize(Vector2f(280.0f, 40.0f));
	m_ErrorMessage->SetTextColor(Color(1.0f, 0.3, 0.3f, 1.0f));
	m_ErrorMessage->SetAnchorBottom(true);
	m_ErrorMessage->SetAnchorRight(true);
	m_ErrorMessage->SetAnchorTop(false);
	m_ErrorMessage->SetWrap(true);
	m_ErrorMessage->SetWordWrap(true);
	m_FileNameLabel = new Label(this);
	m_FileNameLabel->SetPosition(Vector2f(10.0f, 70.0f));
	m_FileNameLabel->SetSize(Vector2f(280.0f, 20.0f));
	m_FileNameLabel->SetTextColor(Color(1.0f, 1.0f, 0.5f, 1.0f));
	m_FileNameLabel->SetBackgroundColor(Color(0.1f, 0.1f, 0.1f, 1.0f));
	m_FileNameLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_FileNameLabel->SetAnchorRight(true);
	m_FileNameLabel->AddKeyListener(this);
	m_FileNameLabel->GrabKeyFocus();
	m_FileScrollBox = new ScrollBox(this);
	m_FileScrollBox->SetPosition(Vector2f(10.0f, 100.0f));
	m_FileScrollBox->SetSize(Vector2f(280.0f, 190.0));
	m_FileScrollBox->SetAnchorBottom(true);
	m_FileScrollBox->SetAnchorRight(true);
	m_FileScrollBox->SetAnchorTop(true);
	m_FileScrollBox->SetHorizontalScrollBarVisible(false);
	
	std::string Path(getenv("HOME"));
	
	if(IsExistingDirectory(Path) == false)
	{
		ShowErrorMessage("Is not an existing directory: \"" + Path + "\".");
		
		return;
	}
	Path += "/.galactic-fall/";
	if(IsExistingDirectory(Path) == false)
	{
		if(CreateDirectory(Path) == false)
		{
			ShowErrorMessage("Could not create the directory: \"" + Path + "\".");
			
			return;
		}
	}
	
	float Top(5.0f);
	std::vector< std::string > Entries(GetDirectoryEntries(Path));
	
	for(std::vector< std::string >::iterator EntryIterator = Entries.begin(); EntryIterator != Entries.end(); ++EntryIterator)
	{
		DirectoryEntryItem * EntryLabel(new DirectoryEntryItem(m_FileScrollBox->GetContent(), EntryIterator->substr(0, EntryIterator->rfind(".xml"))));
		
		EntryLabel->SetPosition(Vector2f(5.0f, Top));
		EntryLabel->SetSize(Vector2f(m_FileScrollBox->GetContent()->GetSize()[0] - 10.0f, 20.0f));
		EntryLabel->SetAnchorRight(true);
		EntryLabel->AddMouseButtonListener(this);
		Top += 25.0f;
	}
	m_FileScrollBox->GetContent()->SetSize(Vector2f(m_FileScrollBox->GetView()->GetSize()[0], std::max(Top, m_FileScrollBox->GetView()->GetSize()[1])));
	m_FileScrollBox->GetContent()->SetAnchorRight(true);
}

void LoadGameDialog::ShowErrorMessage(const std::string & ErrorMessage)
{
	if(m_ErrorMessageTimeoutNotification.IsValid() == true)
	{
		m_ErrorMessageTimeoutNotification.Dismiss();
	}
	m_ErrorMessage->SetVisible(true);
	m_ErrorMessage->SetText(ErrorMessage);
	m_ErrorMessageTimeoutNotification = g_RealTimeTimeoutNotifications->Add(RealTime::Get() + 2.0f, Callback(this, &LoadGameDialog::HideErrorMessage));
}

void LoadGameDialog::HideErrorMessage(void)
{
	m_ErrorMessage->SetVisible(false);
}

bool LoadGameDialog::Load(void)
{
	std::string Path(getenv("HOME"));
	
	if(IsExistingDirectory(Path) == false)
	{
		ShowErrorMessage("Is not an existing directory: \"" + Path + "\".");
		
		return false;
	}
	Path += "/.galactic-fall/";
	if(IsExistingDirectory(Path) == false)
	{
		if(CreateDirectory(Path) == false)
		{
			ShowErrorMessage("Could not create the directory: \"" + Path + "\".");
			
			return false;
		}
	}
	Path += m_FileNameLabel->GetText() + ".xml";
	/// @todo check Path (doesn't exist, if exists overwrite if it's a file?)
	
	std::ifstream InputFileStream;
	
	InputFileStream.open(Path.c_str());
	if(InputFileStream == false)
	{
		ShowErrorMessage("Could not find or read \"" + Path + "\".");
		
		return false;
	}
	else
	{
		if(m_LoadGameCallback(InputFileStream) == false)
		{
			ShowErrorMessage("Loading the game file \"" + Path + "\" failed.");
			
			return false;
		}
		else
		{
			return true;
		}
	}
}

bool LoadGameDialog::OnClicked(Widget * EventSource)
{
	if(EventSource == m_OKButton)
	{
		if(Load() == true)
		{
			Destroy();
			
			return true;
		}
	}
	if(EventSource == m_CancelButton)
	{
		Destroy();
		
		return true;
	}
	
	return false;
}

bool LoadGameDialog::OnKey(Widget * EventSource, const KeyEventInformation & KeyEventInformation)
{
	if((KeyEventInformation.GetKeyCode() == 9 /* ESCAPE */) && (KeyEventInformation.IsDown() == true))
	{
		Destroy();
	}
	else if(EventSource == m_FileNameLabel)
	{
		if((KeyEventInformation.GetKeyCode() == 22 /* BACKSPACE */) && (KeyEventInformation.IsDown() == true))
		{
			if(m_FileNameLabel->GetText().length() > 0)
			{
				m_FileNameLabel->SetText(m_FileNameLabel->GetText().substr(0, m_FileNameLabel->GetText().length() - 1));
			}
		}
		else if((KeyEventInformation.GetKeyCode() == 36 /* RETURN */) && (KeyEventInformation.IsDown() == true))
		{
			if(m_FileNameLabel->GetText().length() > 0)
			{
				if(Load() == true)
				{
					Destroy();
				}
			}
		}
		else if((KeyEventInformation.GetString().empty() == false) && (KeyEventInformation.IsDown() == true))
		{
			m_FileNameLabel->SetText(m_FileNameLabel->GetText() + KeyEventInformation.GetString());
		}
	}
	// eat all other input
	return true;
}

bool LoadGameDialog::OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y)
{
	if(WWindow::OnMouseButton(EventSource, Button, State, X, Y) == true)
	{
		return true;
	}
	if((Button == 1 /* LEFT */) && (State == EV_DOWN))
	{
		DirectoryEntryItem * SelectedDirectoryEntryItem(dynamic_cast< DirectoryEntryItem * >(EventSource));
		
		if(SelectedDirectoryEntryItem != 0)
		{
			if(m_SelectedDirectoryEntryItem != 0)
			{
				m_SelectedDirectoryEntryItem->SetSelected(false);
			}
			m_SelectedDirectoryEntryItem = SelectedDirectoryEntryItem;
			m_SelectedDirectoryEntryItem->SetSelected(true);
			m_FileNameLabel->SetText(m_SelectedDirectoryEntryItem->GetCaption());
			
			return true;
		}
	}
	
	return false;
}
