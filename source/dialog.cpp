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

#include "callbacks/callbacks.h"
#include "dialog.h"

Dialog::Dialog(Widget * SupWidget) :
	WWindow(SupWidget)
{
}

ConnectionHandle Dialog::ConnectClosingCallback(Callback1< bool, Dialog::ClosingReason > Callback)
{
	return _ClosingEvent.Connect(Callback);
}

void Dialog::DisconnectClosingCallback(ConnectionHandle & ConnectionHandle)
{
	_ClosingEvent.Disconnect(ConnectionHandle);
}

void Dialog::_Close(Dialog::ClosingReason ClosingReason)
{
	bool CallDestroy(true);
	
	for(Event1< bool, Dialog::ClosingReason >::CallbackIterator CallbackIterator = _ClosingEvent.GetCallbackIterator(); CallbackIterator.IsValid() == true; ++CallbackIterator)
	{
		CallDestroy &= CallbackIterator(ClosingReason);
	}
	if(CallDestroy == true)
	{
		Destroy();
	}
}
