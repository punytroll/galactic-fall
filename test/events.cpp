/**
 * galactic-fall
 * Copyright (C) 2014  Hagen Möbius
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

#include <assert.h>

#include "../source/callbacks/callbacks.h"
#include "../source/callbacks/events.h"

void EmptyFunction(void)
{
}

class SelfDisconnecting
{
public:
	void Register(Event0< void > & Event)
	{
		_ConnectionHandle = Event.Connect(Callback(this, &SelfDisconnecting::_Notify));
		_Event = &Event;
	}
private:
	void _Notify(void)
	{
		_Event->Disconnect(_ConnectionHandle);
		_Event = nullptr;
	}
	Event0< void > * _Event;
	ConnectionHandle _ConnectionHandle;
};

int main(int argc, char ** argv)
{
	std::cout << "-----------------------------" << std::endl;
	
	{
		Event0< void > Event;
		
		Event();
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Event0< void > Event;
		
		Event.Connect(Callback(EmptyFunction));
		Event();
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Event0< void > Event;
		SelfDisconnecting SelfDisconnecting;
		
		SelfDisconnecting.Register(Event);
		Event();
		Event();
	}
}
