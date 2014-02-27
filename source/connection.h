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

class Connection
{
public:
	class Core
	{
	public:
		Core(void);
		~Core(void);
		int _References;
	};
	
	Connection(void);
	Connection(Core * Core);
	Connection(const Connection & Connection);
	Connection & operator=(const Connection & Other);
	~Connection(void);
	bool IsValid(void);
	void * GetCore(void);
	void Invalidate(void);
private:
	Core * _Core;
};

#endif
