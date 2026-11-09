/**
 * galactic-fall
 * Copyright (C) 2007-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__UI__SAVE_GAME_DIALOG_H
#define GALACTIC_FALL__SOURCE__UI__SAVE_GAME_DIALOG_H

#include "../timeout_notifications.h"
#include "dialog.h"

namespace UI
{
	class Label;
	class ListBox;
	class ListBoxTextItem;
	
	class SaveGameDialog : public UI::Dialog
	{
	public:
		SaveGameDialog();
		auto GetFilePath() -> std::string;
		auto SetDirectoryPath(std::string const & DirectoryPath) -> void;
		auto ShowErrorMessage(std::string const & ErrorMessage) -> void;
	private:
		auto m_OnFileListBoxSelectedItemChanged() -> void;
		auto m_OnFileNameLabelKey(UI::KeyEvent & KeyEvent) -> void;
		auto m_OnFileNameLabelTextChanged() -> void;
		auto m_OnKey(UI::KeyEvent & KeyEvent) -> void;
        
		std::string m_DirectoryPath;
		UI::ListBox * m_FileListBox = nullptr;
		UI::Label * m_FileNameLabel = nullptr;
		UI::Label * m_MessageLabel = nullptr;
		TimeoutNotification m_MessageTimeoutNotification;
	};
}

#endif
