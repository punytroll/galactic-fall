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

#include "connection.h"

Connection::Core::Core(void) :
	_IsValid(false),
	_References(0)
{
}

Connection::Core::~Core(void)
{
	assert(_IsValid == false);
	assert(_References == 0);
}

Connection::Connection(void) :
	_Core(nullptr)
{
}

Connection::Connection(Core * Core) :
	_Core(Core)
{
	assert(_Core != nullptr);
	_Core->_IsValid = true;
	_Core->_References += 1;
}

Connection::Connection(const Connection & Connection) :
	_Core(Connection._Core)
{
	if(_Core != nullptr)
	{
		assert(_Core->_References > 0);
		_Core->_References += 1;
	}
}

Connection & Connection::operator=(const Connection & Other)
{
	if(this != &Other)
	{
		if(_Core != nullptr)
		{
			assert(_Core->_References > 0);
			_Core->_References -= 1;
			if(_Core->_References == 0)
			{
				_Core->_IsValid = false;
				delete _Core;
			}
			_Core = nullptr;
		}
		if(Other._Core != nullptr)
		{
			assert(Other._Core->_References > 0);
			_Core = Other._Core;
			_Core->_References += 1;
		}
	}
	
	return *this;
}

Connection::~Connection(void)
{
	if(_Core != nullptr)
	{
		assert(_Core->_References > 0);
		_Core->_References -= 1;
		if(_Core->_References == 0)
		{
			_Core->_IsValid = false;
			delete _Core;
		}
		_Core = nullptr;
	}
}

void Connection::Clear(void)
{
	if(_Core != nullptr)
	{
		assert(_Core->_References > 0);
		_Core->_References -= 1;
		if(_Core->_References == 0)
		{
			_Core->_IsValid = false;
			delete _Core;
		}
		_Core = nullptr;
	}
}

void Connection::_Invalidate(void)
{
	if(_Core != nullptr)
	{
		assert(_Core->_References > 0);
		_Core->_IsValid = false;
		_Core->_References -= 1;
		if(_Core->_References == 0)
		{
			delete _Core;
		}
		_Core = nullptr;
	}
}
