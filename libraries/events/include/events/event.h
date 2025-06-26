/**
 * galactic-fall
 * Copyright (C) 2014-2025  Hagen Möbius
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

#ifndef GALACTIC_FALL__LIBRARIES__EVENTS__INCLUDE__EVENTS__EVENT_H
#define GALACTIC_FALL__LIBRARIES__EVENTS__INCLUDE__EVENTS__EVENT_H

#include <cassert>
#include <functional>
#include <list>

#include <events/connection.h>

class EventBase
{
public:
	virtual ~EventBase(void)
	{
	}
	
	virtual void Disconnect(Connection & Connection) = 0;
};

template < typename ReturnType, typename ... Types >
class Event : public EventBase
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
	virtual ~Event(void) override
	{
		while(_Cores.empty() == false)
		{
			auto Core(_Cores.back());
			
			_Cores.pop_back();
			assert(Core->_Event == this);
			Core->_Event = nullptr;
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
		Core->_Event = this;
		Core->_IsValid = true;
		Core->_References += 1;
		_Cores.push_back(Core);
		Core->_Iterator = --_Cores.end();
		
		return Connection(Core);
	}
	
	virtual void Disconnect(Connection & Connection) override
	{
		auto Core(static_cast< Event::Core * >(Connection.GetCore()));
		
		assert(Core->_References > 0);
		Core->_References -= 1;
		assert(Core->_References > 0);
		_Cores.erase(Core->_Iterator);
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
