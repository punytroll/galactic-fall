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

#ifndef CONNECTION_HANDLE_H
#define CONNECTION_HANDLE_H

class ConnectionHandle
{
public:
	ConnectionHandle(void) :
		_Handle(0)
	{
	}
	
	ConnectionHandle(void * Handle) :
		_Handle(Handle)
	{
	}
	
	bool IsValid(void) const
	{
		return _Handle != 0;
	}
	
	void * GetHandle(void)
	{
		return _Handle;
	}
	
	void Invalidate(void)
	{
		assert(_Handle != 0);
		_Handle = 0;
	}
private:
	void * _Handle;
};

#endif
