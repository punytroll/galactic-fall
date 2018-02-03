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

#ifndef UI_LOAD_GAME_DIALOG_H
#define UI_LOAD_GAME_DIALOG_H

#include "../timeout_notifications.h"
#include "dialog.h"

namespace UI
{
	class Label;
	class ListBox;
	class ListBoxTextItem;
	
	class LoadGameDialog : public UI::Dialog
	{
	public:
		// constructor
		LoadGameDialog(void);
		// getters
		std::string GetFilePath(void);
		// setters
		void SetDirectoryPath(const std::string & DirectoryPath);
		// modifiers
		void ShowErrorMessage(const std::string & ErrorMessage);
	private:
		// callbacks
		void _OnFileListBoxSelectedItemChanged(void);
		void _OnFileNameLabelKey(UI::KeyEvent & KeyEvent);
		void _OnKey(UI::KeyEvent & KeyEvent);
		// helper functions and actions
		void _OnFileNameLabelTextChanged(void);
		// member variables
		std::string _DirectoryPath;
		UI::ListBox * _FileListBox;
		UI::Label * _FileNameLabel;
		UI::Label * _MessageLabel;
		TimeoutNotification _MessageTimeoutNotification;
	};
}

#endif
