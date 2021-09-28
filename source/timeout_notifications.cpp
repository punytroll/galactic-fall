/**
 * galactic-fall
 * Copyright (C) 2007-2019  Hagen Möbius
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

#include <cassert>
#include <cstdint>
#include <functional>

#include "timeout_notifications.h"

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
	
	std::uint32_t GetReferences(void) const
	{
		return _References;
	}
	
	void Dismiss(void)
	{
		assert(_Manager != nullptr);
		_Manager->Remove(_Iterator);
		Invalidate();
	}
	
	void Invalidate(void)
	{
		_Manager = nullptr;
	}
	
	bool IsValid(void) const
	{
		return _Manager != nullptr;
	}
	
	void Call(void)
	{
		_Callback();
	}
private:
	TimeoutNotificationManager * _Manager;
	std::multimap< double, TimeoutNotification * >::iterator _Iterator;
	std::uint32_t _References;
	std::function< void (void) > _Callback;
};

TimeoutNotification::TimeoutNotification(void) :
	_Core(nullptr)
{
}

TimeoutNotification::TimeoutNotification(const TimeoutNotification & TimeoutNotification) :
	_Core(nullptr)
{
	_Core = TimeoutNotification._Core;
	if(_Core != nullptr)
	{
		_Core->Reference();
	}
}

TimeoutNotification::TimeoutNotification(TimeoutNotificationManager * Manager, std::multimap< double, TimeoutNotification * >::iterator Iterator, std::function< void (void) > Callback) :
	_Core(nullptr)
{
	_Core = new TimeoutNotificationCore{Manager, Iterator, Callback};
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
	if(_Core != nullptr)
	{
		_Core->Unreference();
		if(_Core->GetReferences() == 0)
		{
			delete _Core;
		}
		_Core = nullptr;
	}
	// set and reference new core
	_Core = TimeoutNotification._Core;
	if(_Core != nullptr)
	{
		_Core->Reference();
	}
	
	return *this;
}

bool TimeoutNotification::IsValid(void) const
{
	return (_Core != nullptr) && (_Core->IsValid() == true);
}

void TimeoutNotification::Dismiss(void)
{
	assert(_Core != nullptr);
	_Core->Dismiss();
}

void TimeoutNotification::Call(void)
{
	assert(_Core != nullptr);
	_Core->Call();
}

void TimeoutNotification::Invalidate(void)
{
	assert(_Core != nullptr);
	_Core->Invalidate();
	_Core->Unreference();
	if(_Core->GetReferences() == 0)
	{
		delete _Core;
	}
	_Core = nullptr;
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
		auto Iterator{_TimeoutNotifications.begin()};
		
		// only valid timeout notifications in here
		assert(Iterator->second != nullptr);
		assert(Iterator->second->IsValid() == true);
		// call the notification callback object
		Iterator->second->Call();
		// remove the timeout notification object
		Remove(Iterator);
	}
}

TimeoutNotification TimeoutNotificationManager::Add(double Time, std::function< void (void) > Callback)
{
	auto Iterator{_TimeoutNotifications.insert(std::make_pair(Time, static_cast< TimeoutNotification * >(nullptr)))};
	auto Result{new TimeoutNotification{this, Iterator, Callback}};
	
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
