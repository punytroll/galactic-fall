/**
 * galactic-fall
 * Copyright (C) 2009-2018  Hagen Möbius
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

#include "dialog.h"

UI::Dialog::Dialog()
{
}

Connection UI::Dialog::ConnectClosingCallback(std::function< bool (UI::Dialog::ClosingReason) > Callback)
{
	return _ClosingEvent.Connect(Callback);
}

void UI::Dialog::DisconnectClosingCallback(Connection & Connection)
{
	_ClosingEvent.Disconnect(Connection);
}

void UI::Dialog::_Close(UI::Dialog::ClosingReason ClosingReason)
{
	bool CallDestroy(true);
	
	for(auto & Callback : _ClosingEvent.GetCallbacks())
	{
		CallDestroy &= Callback(ClosingReason);
	}
	if(CallDestroy == true)
	{
		Destroy();
	}
}
