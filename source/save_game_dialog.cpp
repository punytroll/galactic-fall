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
#include "globals.h"
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
}

void SaveGameDialog::HideErrorMessage(void)
{
	m_ErrorMessage->Hide();
}

bool SaveGameDialog::OnClicked(Widget * EventSource)
{
	if(EventSource == m_OKButton)
	{
		std::string Path(getenv("HOME"));
		
		/// @todo check Path (exists and is directory)
		Path += "/.galactic-fall";
		/// @todo check Path (exists and is directory)
		Path += "/save_game.xml";
		
		std::ofstream OFStream;
		
		OFStream.open(Path.c_str());
		if(OFStream == false)
		{
			if(m_ErrorMessageTimeoutNotification.IsValid() == true)
			{
				m_ErrorMessageTimeoutNotification.Dismiss();
			}
			m_ErrorMessage->Show();
			m_ErrorMessage->SetString("Could not create or write \"" + Path + "\".");
			m_ErrorMessageTimeoutNotification = g_RealTimeTimeoutNotifications->Add(RealTime::Get() + 2.0f, Method(this, &SaveGameDialog::HideErrorMessage));
		}
		else
		{
			(*m_SaveGameCallback)(OFStream);
			Destroy();
		}
		
		return true;
	}
	if(EventSource == m_CancelButton)
	{
		Destroy();
		
		return true;
	}
	
	return false;
}

bool SaveGameDialog::OnKey(Widget * EventSource, int KeyCode, int State)
{
	if(((KeyCode == 9 /* ESCAPE */) || (KeyCode == 36 /* RETURN */) || (KeyCode == 58 /* M */)) && (State == EV_DOWN))
	{
		Destroy();
	}
	// eat all other input
	return true;
}

bool SaveGameDialog::OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y)
{
}
