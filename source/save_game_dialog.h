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

#ifndef SAVE_GAME_DIALOG_H
#define SAVE_GAME_DIALOG_H

#include "mouse_button_listener.h"
#include "timeout_notifications.h"
#include "window.h"

class Button;
class DirectoryEntryItem;
class Label;
class ScrollBox;
template < typename ReturnType, typename Argument1Type > class Callback1;

class SaveGameDialog : virtual public MouseButtonListener, public WWindow
{
public:
	SaveGameDialog(Widget * SupWidget, Callback1< void, std::ostream & > SaveGameCallback);
protected:
	// callbacks
	void OnCancelClicked(void);
	bool OnFileNameLabelKey(const KeyEventInformation & KeyEventInformation);
	bool OnKey(const KeyEventInformation & KeyEventInformation);
	void OnOKClicked(void);
	// listeners
	virtual bool OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y);
private:
	void ShowErrorMessage(const std::string & ErrorMessage);
	void HideErrorMessage(void);
	bool Save(void);
	Callback1< void, std::ostream & > m_SaveGameCallback;
	Button * m_CancelButton;
	Button * m_OKButton;
	Label * m_ErrorMessage;
	TimeoutNotification m_ErrorMessageTimeoutNotification;
	Label * m_FileNameLabel;
	ScrollBox * m_FileScrollBox;
	DirectoryEntryItem * m_SelectedDirectoryEntryItem;
};

#endif
