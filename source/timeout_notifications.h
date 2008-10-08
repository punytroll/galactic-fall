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

#ifndef TIMEOUT_NOTIFICATIONS_H
#define TIMEOUT_NOTIFICATIONS_H

#include <map>

#include "callbacks/callbacks.h"

class TimeoutNotificationCore;
class TimeoutNotificationManager;

class TimeoutNotification
{
public:
	TimeoutNotification(void);
	TimeoutNotification(const TimeoutNotification & TimeoutNotification);
	~TimeoutNotification(void);
	TimeoutNotification & operator=(const TimeoutNotification & TimeoutNotification);
	bool IsValid(void) const;
	void Dismiss(void);
	void Call(void);
private:
	friend class TimeoutNotificationManager;
	void Invalidate(void);
	TimeoutNotification(TimeoutNotificationManager * Manager, std::multimap< double, TimeoutNotification * >::iterator Iterator, Callback0< void > Callback);
	TimeoutNotificationCore * m_Core;
};

class TimeoutNotificationManager
{
public:
	~TimeoutNotificationManager(void);
	void Process(double Time);
	TimeoutNotification Add(double Time, Callback0< void > Callback);
private:
	friend class TimeoutNotificationCore;
	void Remove(std::multimap< double, TimeoutNotification * >::iterator Iterator);
	std::multimap< double, TimeoutNotification * > m_TimeoutNotifications;
};

#endif
