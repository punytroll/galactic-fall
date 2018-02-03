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

#ifndef UI_SAVE_GAME_DIALOG_H
#define UI_SAVE_GAME_DIALOG_H

#include "../timeout_notifications.h"
#include "dialog.h"

namespace UI
{
	class Label;
	class ListBoxTextItem;
	class ScrollBox;
	
	class SaveGameDialog : public UI::Dialog
	{
	public:
		SaveGameDialog(UI::Widget * SupWidget);
		// getters
		std::string GetFilePath(void);
		// setters
		void SetDirectoryPath(const std::string & DirectoryPath);
		// modifiers
		void ShowErrorMessage(const std::string & ErrorMessage);
	private:
		// callbacks
		void _OnDirectoryEntryItemMouseButton(UI::ListBoxTextItem * DirectoryEntryItem, UI::MouseButtonEvent & MouseButtonEvent);
		void _OnFileNameLabelKey(UI::KeyEvent & KeyEvent);
		void _OnKey(UI::KeyEvent & KeyEvent);
		// helper functions and actions
		void _OnFileNameLabelTextChanged(void);
		// member variables
		std::string _DirectoryPath;
		UI::Label * _MessageLabel;
		TimeoutNotification _MessageTimeoutNotification;
		UI::Label * _FileNameLabel;
		UI::ScrollBox * _FileScrollBox;
		UI::ListBoxTextItem * _SelectedDirectoryEntryItem;
	};
}

#endif
