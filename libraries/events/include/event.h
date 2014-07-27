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

#ifndef EVENT_H
#define EVENT_H

#include <assert.h>

#include <functional>
#include <list>

#include "connection.h"

template < typename ReturnType, typename ... Types >
class Event
{
private:
	class Core : public Connection::Core
	{
	public:
		Core(void)
		{
		}
		
		typename std::function< ReturnType (Types ...) > _Callback;
		typename std::list< Event::Core * >::iterator _Iterator;
	};
public:
	~Event(void)
	{
		while(_Cores.empty() == false)
		{
			auto Core(_Cores.back());
			
			_Cores.pop_back();
			Core->_IsValid = false;
			Core->_References -= 1;
			if(Core->_References == 0)
			{
				delete Core;
			}
		}
	}
	
	Connection Connect(std::function< ReturnType (Types ...) > Callback)
	{
		auto Core(new Event::Core());
		
		Core->_Callback = Callback;
		Core->_IsValid = true;
		Core->_References += 1;
		_Cores.push_back(Core);
		Core->_Iterator = --_Cores.end();
		
		return Connection(Core);
	}
	
	void Disconnect(Connection & Connection)
	{
		_Cores.erase(static_cast< Event::Core * >(Connection.GetCore())->_Iterator);
		Connection._Invalidate();
	}
	
	std::list< std::function< ReturnType (Types ...) > > GetCallbacks(void)
	{
		std::list< std::function< ReturnType (Types ...) > > Result;
		
		for(auto Core : _Cores)
		{
			Result.push_back(Core->_Callback);
		}
		
		return Result;
	}
	
	void operator()(Types ... Arguments)
	{
		for(auto & Callback : GetCallbacks())
		{
			Callback(Arguments ...);
		}
	}
private:
	std::list< Event::Core * > _Cores;
};

#endif
