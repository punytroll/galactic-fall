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

#ifndef LOAD_GAME_DIALOG_H
#define LOAD_GAME_DIALOG_H

#include "timeout_notifications.h"
#include "window.h"

class Button;
class DirectoryEntryItem;
class Label;
class ScrollBox;
template < typename ReturnType, typename Argument1Type > class Callback1;

class LoadGameDialog : public WWindow
{
public:
	enum ClosingReason
	{
		OK_BUTTON,
		CANCEL_BUTTON,
		RETURN_KEY,
		ESCAPE_KEY
	};
	
	LoadGameDialog(Widget * SupWidget);
	// getters
	std::string GetFilePath(void);
	// setters
	void SetDirectoryPath(const std::string & DirectoryPath);
	// modifiers
	void ShowErrorMessage(const std::string & ErrorMessage);
	// connecting and disconnecting event callbacks
	ConnectionHandle ConnectClosingCallback(Callback1< bool, LoadGameDialog::ClosingReason > Callback);
	void DisconnectClosingCallback(ConnectionHandle & ConnectionHandle);
private:
	// callbacks
	bool OnDirectoryEntryItemMouseButton(DirectoryEntryItem * DirectoryEntryItem, int Button, int State, float X, float Y);
	bool OnFileNameLabelKey(const KeyEventInformation & KeyEventInformation);
	bool OnKey(const KeyEventInformation & KeyEventInformation);
	// helper functions and actions
	void HideErrorMessage(void);
	void _Close(LoadGameDialog::ClosingReason ClosingReason);
	void _OnFileNameLabelTextChanged(void);
	// member variables
	std::string _DirectoryPath;
	Button * m_CancelButton;
	Button * m_OKButton;
	Label * m_ErrorMessage;
	TimeoutNotification m_ErrorMessageTimeoutNotification;
	Label * m_FileNameLabel;
	ScrollBox * m_FileScrollBox;
	DirectoryEntryItem * m_SelectedDirectoryEntryItem;
	//events
	Event1< bool, LoadGameDialog::ClosingReason > _ClosingEvent;
};

#endif
