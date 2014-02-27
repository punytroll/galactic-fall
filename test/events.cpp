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

#include <iostream>

#include "../source/callbacks/events.h"

int g_Global;

void EmptyFunction(void)
{
}

void IncrementingFunction(int Value)
{
	g_Global += Value;
}

class SelfDisconnecting
{
public:
	void Register(Event0< void > & Event)
	{
		_ConnectionHandle = Event.Connect(std::bind(&SelfDisconnecting::_Notify, this));
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
		
		Event.Connect(EmptyFunction);
		Event();
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		g_Global = 32;
		
		Event0< void > Event;
		
		Event.Connect(std::bind(IncrementingFunction, 8));
		Event();
		assert(g_Global == 40);
		Event();
		assert(g_Global == 48);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		g_Global = 64;
		
		Event0< void > Event;
		ConnectionHandle Handle1(Event.Connect(std::bind(IncrementingFunction, 8)));
		ConnectionHandle Handle2(Event.Connect(std::bind(IncrementingFunction, 12)));
		
		Event();
		assert(g_Global == 84);
		Event.Disconnect(Handle1);
		Event();
		assert(g_Global == 96);
		Event.Disconnect(Handle2);
		Event();
		assert(g_Global == 96);
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
