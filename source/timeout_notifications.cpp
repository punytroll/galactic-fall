/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
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

#include <functional>

#include "timeout_notifications.h"
#include "type_definitions.h"

class TimeoutNotificationCore
{
public:
	TimeoutNotificationCore(TimeoutNotificationManager * Manager, std::multimap< double, TimeoutNotification * >::iterator Iterator, std::function< void (void) > Callback) :
		_Manager(Manager),
		_Iterator(Iterator),
		_References(0),
		_Callback(Callback)
	{
	}
	
	~TimeoutNotificationCore(void)
	{
		assert(IsValid() == false);
	}
	
	void Reference(void)
	{
		_References++;
	}
	
	void Unreference(void)
	{
		assert(_References > 0);
		_References--;
	}
	
	unsigned_numeric GetReferences(void) const
	{
		return _References;
	}
	
	void Dismiss(void)
	{
		assert(_Manager != 0);
		_Manager->Remove(_Iterator);
		Invalidate();
	}
	
	void Invalidate(void)
	{
		_Manager = 0;
	}
	
	bool IsValid(void) const
	{
		return _Manager != 0;
	}
	
	void Call(void)
	{
		_Callback();
	}
private:
	TimeoutNotificationManager * _Manager;
	std::multimap< double, TimeoutNotification * >::iterator _Iterator;
	unsigned_numeric _References;
	std::function< void (void) > _Callback;
};

TimeoutNotification::TimeoutNotification(void) :
	_Core(0)
{
}

TimeoutNotification::TimeoutNotification(const TimeoutNotification & TimeoutNotification) :
	_Core(0)
{
	_Core = TimeoutNotification._Core;
	if(_Core != 0)
	{
		_Core->Reference();
	}
}

TimeoutNotification::TimeoutNotification(TimeoutNotificationManager * Manager, std::multimap< double, TimeoutNotification * >::iterator Iterator, std::function< void (void) > Callback) :
	_Core(0)
{
	_Core = new TimeoutNotificationCore(Manager, Iterator, Callback);
	_Core->Reference();
}

TimeoutNotification::~TimeoutNotification(void)
{
	if(_Core != 0)
	{
		_Core->Unreference();
		if(_Core->GetReferences() == 0)
		{
			delete _Core;
		}
		_Core = 0;
	}
}

TimeoutNotification & TimeoutNotification::operator=(const TimeoutNotification & TimeoutNotification)
{
	// dereference old core
	if(_Core != 0)
	{
		_Core->Unreference();
		if(_Core->GetReferences() == 0)
		{
			delete _Core;
		}
		_Core = 0;
	}
	// set and reference new core
	_Core = TimeoutNotification._Core;
	if(_Core != 0)
	{
		_Core->Reference();
	}
	
	return *this;
}

bool TimeoutNotification::IsValid(void) const
{
	return (_Core != 0) && (_Core->IsValid() == true);
}

void TimeoutNotification::Dismiss(void)
{
	assert(_Core != 0);
	_Core->Dismiss();
}

void TimeoutNotification::Call(void)
{
	assert(_Core != 0);
	_Core->Call();
}

void TimeoutNotification::Invalidate(void)
{
	assert(_Core != 0);
	_Core->Invalidate();
	_Core->Unreference();
	if(_Core->GetReferences() == 0)
	{
		delete _Core;
	}
	_Core = 0;
}

TimeoutNotificationManager::~TimeoutNotificationManager(void)
{
	while(_TimeoutNotifications.size() > 0)
	{
		// remove the timeout notification object
		Remove(_TimeoutNotifications.begin());
	}
}

void TimeoutNotificationManager::Process(double Time)
{
	while((_TimeoutNotifications.size() > 0) && (Time > _TimeoutNotifications.begin()->first))
	{
		std::multimap< double, TimeoutNotification * >::iterator Iterator(_TimeoutNotifications.begin());
		
		// only valid timeout notifications in here
		assert(Iterator->second != 0);
		assert(Iterator->second->IsValid() == true);
		// call the notification callback object
		Iterator->second->Call();
		// remove the timeout notification object
		Remove(Iterator);
	}
}

TimeoutNotification TimeoutNotificationManager::Add(double Time, std::function< void (void) > Callback)
{
	std::multimap< double, TimeoutNotification * >::iterator Iterator(_TimeoutNotifications.insert(std::make_pair(Time, reinterpret_cast< TimeoutNotification * >(0))));
	TimeoutNotification * Result(new TimeoutNotification(this, Iterator, Callback));
	
	Iterator->second = Result;
	
	return *Result;
}

void TimeoutNotificationManager::Remove(std::multimap< double, TimeoutNotification * >::iterator Iterator)
{
	// invalidate the timeout notification object
	Iterator->second->Invalidate();
	// delete the notification callback object
	delete Iterator->second;
	// remove the notification callback from the multimap
	_TimeoutNotifications.erase(Iterator);
}
