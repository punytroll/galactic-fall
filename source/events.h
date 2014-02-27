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

#ifndef EVENTS_H
#define EVENTS_H

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
		Core(typename std::list< std::function < ReturnType (Types ...) > >::iterator Iterator) :
			_Iterator(Iterator)
		{
		}
		
		typename std::list< std::function< ReturnType (Types ...) > >::iterator _Iterator;
	};
public:
	Connection Connect(std::function< ReturnType (Types ...) > Callback)
	{
		_Callbacks.push_back(Callback);
		
		return Connection(new Core(--_Callbacks.end()));
	}
	
	void Disconnect(Connection & Connection)
	{
		_Callbacks.erase(static_cast< Core * >(Connection.GetCore())->_Iterator);
		Connection.Invalidate();
	}
	
	const std::list< std::function< ReturnType (Types ...) > > & GetCallbacks(void)
	{
		return _Callbacks;
	}
	
	void operator()(Types ... Arguments)
	{
		decltype(_Callbacks) Callbacks;
		
		for(auto & Callback : _Callbacks)
		{
			Callbacks.push_back(Callback);
		}
		for(auto & Callback : Callbacks)
		{
			Callback(Arguments ...);
		}
	}
private:
	std::list< std::function< ReturnType (Types ...) > > _Callbacks;
};

#endif
