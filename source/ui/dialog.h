/**
 * galactic-fall
 * Copyright (C) 2009  Hagen Möbius
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

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include "window.h"

template < typename ReturnType, typename Argument1Type >
class Callback1;

namespace UI
{
	class Dialog : public Window
	{
	public:
		enum ClosingReason
		{
			OK_BUTTON,
			CANCEL_BUTTON,
			RETURN_KEY,
			ESCAPE_KEY
		};
		// constructors & destructor
		Dialog(Widget * SupWidget);
		// connecting and disconnecting event callbacks
		ConnectionHandle ConnectClosingCallback(Callback1< bool, Dialog::ClosingReason > Callback);
		void DisconnectClosingCallback(ConnectionHandle & ConnectionHandle);
	protected:
		// helper functions and actions
		void _Close(Dialog::ClosingReason ClosingReason);
	private:
		// member variables
		//events
		Event1< bool, Dialog::ClosingReason > _ClosingEvent;
	};
}

#endif
