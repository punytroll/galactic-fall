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

#include "timeout_notifications.h"
#include "type_definitions.h"

class TimeoutNotificationCore
{
public:
	TimeoutNotificationCore(TimeoutNotificationManager * Manager, std::multimap< double, TimeoutNotification * >::iterator Iterator, Callback0< void > * Callback) :
		m_Manager(Manager),
		m_Iterator(Iterator),
		m_References(0),
		m_Callback(Callback)
	{
	}
	
	~TimeoutNotificationCore(void)
	{
		assert(IsValid() == false);
		delete m_Callback;
	}
	
	void Reference(void)
	{
		m_References++;
	}
	
	void Unreference(void)
	{
		assert(m_References > 0);
		m_References--;
	}
	
	unsigned_numeric GetReferences(void) const
	{
		return m_References;
	}
	
	void Dismiss(void)
	{
		assert(m_Manager != 0);
		m_Manager->Remove(m_Iterator);
		Invalidate();
	}
	
	void Invalidate(void)
	{
		m_Manager = 0;
	}
	
	bool IsValid(void) const
	{
		return m_Manager != 0;
	}
	
	void Call(void)
	{
		(*m_Callback)();
	}
private:
	TimeoutNotificationManager * m_Manager;
	std::multimap< double, TimeoutNotification * >::iterator m_Iterator;
	unsigned_numeric m_References;
	Callback0< void > * m_Callback;
};

TimeoutNotification::TimeoutNotification(void) :
	m_Core(0)
{
}

TimeoutNotification::TimeoutNotification(const TimeoutNotification & TimeoutNotification) :
	m_Core(0)
{
	m_Core = TimeoutNotification.m_Core;
	if(m_Core != 0)
	{
		m_Core->Reference();
	}
}

TimeoutNotification::TimeoutNotification(TimeoutNotificationManager * Manager, std::multimap< double, TimeoutNotification * >::iterator Iterator, Callback0< void > * Callback) :
	m_Core(0)
{
	m_Core = new TimeoutNotificationCore(Manager, Iterator, Callback);
	m_Core->Reference();
}

TimeoutNotification::~TimeoutNotification(void)
{
	if(m_Core != 0)
	{
		m_Core->Unreference();
		if(m_Core->GetReferences() == 0)
		{
			delete m_Core;
		}
		m_Core = 0;
	}
}

TimeoutNotification & TimeoutNotification::operator=(const TimeoutNotification & TimeoutNotification)
{
	// dereference old core
	if(m_Core != 0)
	{
		m_Core->Unreference();
		if(m_Core->GetReferences() == 0)
		{
			delete m_Core;
		}
		m_Core = 0;
	}
	// set and reference new core
	m_Core = TimeoutNotification.m_Core;
	if(m_Core != 0)
	{
		m_Core->Reference();
	}
}

bool TimeoutNotification::IsValid(void) const
{
	return (m_Core != 0) && (m_Core->IsValid() == true);
}

void TimeoutNotification::Dismiss(void)
{
	assert(m_Core != 0);
	m_Core->Dismiss();
}

void TimeoutNotification::Call(void)
{
	assert(m_Core != 0);
	m_Core->Call();
}

void TimeoutNotification::Invalidate(void)
{
	assert(m_Core != 0);
	m_Core->Invalidate();
	m_Core->Unreference();
	if(m_Core->GetReferences() == 0)
	{
		delete m_Core;
	}
	m_Core = 0;
}

TimeoutNotificationManager::~TimeoutNotificationManager(void)
{
	while(m_TimeoutNotifications.size() > 0)
	{
		// remove the timeout notification object
		Remove(m_TimeoutNotifications.begin());
	}
}

void TimeoutNotificationManager::Process(double Time)
{
	while((m_TimeoutNotifications.size() > 0) && (Time > m_TimeoutNotifications.begin()->first))
	{
		std::multimap< double, TimeoutNotification * >::iterator Iterator(m_TimeoutNotifications.begin());
		
		// only valid timeout notifications in here
		assert(Iterator->second != 0);
		assert(Iterator->second->IsValid() == true);
		// call the notification callback object
		Iterator->second->Call();
		// remove the timeout notification object
		Remove(Iterator);
	}
}

TimeoutNotification TimeoutNotificationManager::Add(double Time, Callback0< void > * Callback)
{
	std::multimap< double, TimeoutNotification * >::iterator Iterator(m_TimeoutNotifications.insert(std::make_pair(Time, reinterpret_cast< TimeoutNotification * >(0))));
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
	m_TimeoutNotifications.erase(Iterator);
}
