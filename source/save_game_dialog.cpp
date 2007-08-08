/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
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

#include <fstream>

#include "button.h"
#include "file_handling.h"
#include "globals.h"
#include "key_event_information.h"
#include "label.h"
#include "real_time.h"
#include "save_game_dialog.h"

SaveGameDialog::SaveGameDialog(Widget * SupWidget, Callback1< void, std::ostream & > * SaveGameCallback) :
	WWindow(SupWidget, "Save Game"),
	m_SaveGameCallback(SaveGameCallback)
{
	SetPosition(Vector2f(120.0f, 200.0f));
	SetSize(Vector2f(300.0f, 200.0f));
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
	m_ErrorMessage->SetForegroundColor(Color(1.0f, 0.3, 0.3f, 1.0f));
	m_ErrorMessage->SetAnchorBottom(true);
	m_ErrorMessage->SetAnchorRight(true);
	m_ErrorMessage->SetAnchorTop(false);
	m_ErrorMessage->SetWrap(true);
	m_ErrorMessage->SetWordWrap(true);
	m_FileNameLabel = new Label(this);
	m_FileNameLabel->SetPosition(Vector2f(10.0f, 70.0f));
	m_FileNameLabel->SetSize(Vector2f(280.0f, 20.0f));
	m_FileNameLabel->SetForegroundColor(Color(1.0f, 1.0f, 0.5f, 1.0f));
	m_FileNameLabel->SetBackgroundColor(Color(0.1f, 0.1f, 0.1f, 1.0f));
	m_FileNameLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_FileNameLabel->SetAnchorRight(true);
	m_FileNameLabel->AddKeyListener(this);
	m_FileNameLabel->GrabKeyFocus();
}

void SaveGameDialog::ShowErrorMessage(const std::string & ErrorMessage)
{
	if(m_ErrorMessageTimeoutNotification.IsValid() == true)
	{
		m_ErrorMessageTimeoutNotification.Dismiss();
	}
	m_ErrorMessage->Show();
	m_ErrorMessage->SetString(ErrorMessage);
	m_ErrorMessageTimeoutNotification = g_RealTimeTimeoutNotifications->Add(RealTime::Get() + 2.0f, Method(this, &SaveGameDialog::HideErrorMessage));
}

void SaveGameDialog::HideErrorMessage(void)
{
	m_ErrorMessage->Hide();
}

bool SaveGameDialog::Save(void)
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
	Path += m_FileNameLabel->GetString() + ".xml";
	/// @todo check Path (doesn't exist, if exists overwrite if it's a file?)
	
	std::ofstream OFStream;
	
	OFStream.open(Path.c_str());
	if(OFStream == false)
	{
		ShowErrorMessage("Could not create or write \"" + Path + "\".");
		
		return false;
	}
	else
	{
		(*m_SaveGameCallback)(OFStream);
		
		return true;
	}
}

bool SaveGameDialog::OnClicked(Widget * EventSource)
{
	if(EventSource == m_OKButton)
	{
		if(Save() == true)
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

bool SaveGameDialog::OnKey(Widget * EventSource, const KeyEventInformation & KeyEventInformation)
{
	if((KeyEventInformation.GetKeyCode() == 9 /* ESCAPE */) && (KeyEventInformation.IsDown() == true))
	{
		Destroy();
	}
	else if(EventSource == m_FileNameLabel)
	{
		if((KeyEventInformation.GetKeyCode() == 22 /* BACKSPACE */) && (KeyEventInformation.IsDown() == true))
		{
			if(m_FileNameLabel->GetString().length() > 0)
			{
				m_FileNameLabel->SetString(m_FileNameLabel->GetString().substr(0, m_FileNameLabel->GetString().length() - 1));
			}
		}
		else if((KeyEventInformation.GetKeyCode() == 36 /* RETURN */) && (KeyEventInformation.IsDown() == true))
		{
			if(m_FileNameLabel->GetString().length() > 0)
			{
				if(Save() == true)
				{
					Destroy();
				}
			}
		}
		else if((KeyEventInformation.GetString().empty() == false) && (KeyEventInformation.IsDown() == true))
		{
			m_FileNameLabel->SetString(m_FileNameLabel->GetString() + KeyEventInformation.GetString());
		}
	}
	// eat all other input
	return true;
}

bool SaveGameDialog::OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y)
{
	return WWindow::OnMouseButton(EventSource, Button, State, X, Y);
}
