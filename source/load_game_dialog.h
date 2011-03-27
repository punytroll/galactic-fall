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

#include "ui/dialog.h"
#include "timeout_notifications.h"

class Button;
class DirectoryEntryItem;
class ScrollBox;

namespace UI
{
	class Label;
}

class LoadGameDialog : public UI::Dialog
{
public:
	LoadGameDialog(Widget * SupWidget);
	// getters
	std::string GetFilePath(void);
	// setters
	void SetDirectoryPath(const std::string & DirectoryPath);
	// modifiers
	void ShowErrorMessage(const std::string & ErrorMessage);
private:
	// callbacks
	bool OnDirectoryEntryItemMouseButton(DirectoryEntryItem * DirectoryEntryItem, int Button, int State, float X, float Y);
	bool OnFileNameLabelKey(const KeyEventInformation & KeyEventInformation);
	bool OnKey(const KeyEventInformation & KeyEventInformation);
	// helper functions and actions
	void HideErrorMessage(void);
	void _OnFileNameLabelTextChanged(void);
	// member variables
	std::string _DirectoryPath;
	Button * m_CancelButton;
	Button * m_OKButton;
	UI::Label * m_ErrorMessage;
	TimeoutNotification m_ErrorMessageTimeoutNotification;
	UI::Label * m_FileNameLabel;
	ScrollBox * m_FileScrollBox;
	DirectoryEntryItem * m_SelectedDirectoryEntryItem;
};

#endif
