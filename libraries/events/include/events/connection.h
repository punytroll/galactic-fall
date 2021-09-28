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

#ifndef CONNECTION_H
#define CONNECTION_H

class EventBase;

class Connection
{
public:
	template < typename ReturnType, typename ... Types >
	friend class Event;
	
	class Core
	{
	public:
		Core(void);
		virtual ~Core(void);
		EventBase * _Event;
		bool _IsValid;
		int _References;
	};
	
	Connection(void);
	Connection(Core * Core);
	Connection(const Connection & Connection);
	Connection & operator=(const Connection & Other);
	~Connection(void);
	void Clear(void);
	void Disconnect(void);
	bool IsValid(void);
	Core * GetCore(void);
private:
	void _Invalidate(void);
	Core * _Core;
};

inline bool Connection::IsValid(void)
{
	return (_Core != nullptr) && (_Core->_IsValid == true);
}

inline Connection::Core * Connection::GetCore(void)
{
	return _Core;
}

#endif
