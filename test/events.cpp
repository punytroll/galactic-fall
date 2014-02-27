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

#include "../source/events.h"

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
	void Register(Event< void > & Event)
	{
		_Connection = Event.Connect(std::bind(&SelfDisconnecting::_Notify, this));
		_Event = &Event;
	}
private:
	void _Notify(void)
	{
		_Event->Disconnect(_Connection);
		_Event = nullptr;
	}
	Event< void > * _Event;
	Connection _Connection;
};

int main(int argc, char ** argv)
{
	std::cout << "-----------------------------" << std::endl;
	
	{
		Event< void > Event;
		
		Event();
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Event< void > Event;
		
		Event.Connect(EmptyFunction);
		Event();
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		g_Global = 32;
		
		Event< void > Event;
		
		Event.Connect(std::bind(IncrementingFunction, 8));
		Event();
		assert(g_Global == 40);
		Event();
		assert(g_Global == 48);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Event< void > Event;
		Connection Connection;
		
		Connection = Event.Connect(std::bind(EmptyFunction));
		Event.Disconnect(Connection);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		g_Global = 64;
		
		Event< void > Event;
		Connection Connection(Event.Connect(std::bind(IncrementingFunction, 78)));
		
		Event.Disconnect(Connection);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		g_Global = 64;
		
		Event< void > Event;
		Connection Connection1(Event.Connect(std::bind(IncrementingFunction, 8)));
		Connection Connection2(Event.Connect(std::bind(IncrementingFunction, 12)));
		Connection Connection3(Event.Connect(std::bind(IncrementingFunction, 7)));
		
		Event();
		assert(g_Global == 91);
		Event.Disconnect(Connection3);
		Event();
		assert(g_Global == 111);
		Event.Disconnect(Connection1);
		Event();
		assert(g_Global == 123);
		Event.Disconnect(Connection2);
		Event();
		assert(g_Global == 123);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Event< void > Event;
		SelfDisconnecting SelfDisconnecting;
		
		SelfDisconnecting.Register(Event);
		Event();
		Event();
	}
}
