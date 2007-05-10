/**
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

/**
 * This is version 1.0.0 of the c++ component "time_value".
 **/

#ifndef TIME_VALUE_H
#define TIME_VALUE_H

#include <stdint.h>

#include <iostream>

class TimeValue
{
public:
	TimeValue(void);
	TimeValue(double Seconds);
	TimeValue(uint32_t Seconds, uint32_t MicroSeconds);
	TimeValue(const TimeValue & TimeValue);
	uint32_t Seconds(void) const;
	uint32_t MicroSeconds(void) const;
	TimeValue & operator=(const TimeValue & TimeValue);
	TimeValue & operator+=(const TimeValue & TimeValue);
	TimeValue & operator-=(const TimeValue & TimeValue);
private:
	int32_t m_Seconds;
	int32_t m_MicroSeconds;
};

inline uint32_t TimeValue::Seconds(void) const
{
	return m_Seconds;
}

inline uint32_t TimeValue::MicroSeconds(void) const
{
	return m_MicroSeconds;
}

inline TimeValue & TimeValue::operator=(const TimeValue & TimeValue)
{
	m_Seconds = TimeValue.Seconds();
	m_MicroSeconds = TimeValue.MicroSeconds();
}

inline TimeValue & TimeValue::operator+=(const TimeValue & TimeValue)
{
	m_Seconds += TimeValue.Seconds();
	m_MicroSeconds += TimeValue.MicroSeconds();
	if(m_MicroSeconds >= 1000000)
	{
		++m_Seconds;
		m_MicroSeconds -= 1000000;
	}
	
	return *this;
}

inline TimeValue & TimeValue::operator-=(const TimeValue & TimeValue)
{
	m_Seconds -= TimeValue.Seconds();
	m_MicroSeconds -= TimeValue.MicroSeconds();
	if(m_MicroSeconds < 0)
	{
		--m_Seconds;
		m_MicroSeconds += 1000000;
	}
	
	return *this;
}

inline bool operator<(const TimeValue & TimeValue1, const TimeValue & TimeValue2)
{
	return (TimeValue1.Seconds() < TimeValue2.Seconds()) || ((TimeValue1.Seconds() == TimeValue2.Seconds()) && (TimeValue1.MicroSeconds() < TimeValue2.MicroSeconds()));
}

inline bool operator>(const TimeValue & TimeValue1, const TimeValue & TimeValue2)
{
	return (TimeValue1.Seconds() > TimeValue2.Seconds()) || ((TimeValue1.Seconds() == TimeValue2.Seconds()) && (TimeValue1.MicroSeconds() > TimeValue2.MicroSeconds()));
}

inline bool operator<=(const TimeValue & TimeValue1, const TimeValue & TimeValue2)
{
	return (TimeValue1.Seconds() < TimeValue2.Seconds()) || ((TimeValue1.Seconds() == TimeValue2.Seconds()) && (TimeValue1.MicroSeconds() <= TimeValue2.MicroSeconds()));
}

inline bool operator>=(const TimeValue & TimeValue1, const TimeValue & TimeValue2)
{
	return (TimeValue1.Seconds() > TimeValue2.Seconds()) || ((TimeValue1.Seconds() == TimeValue2.Seconds()) && (TimeValue1.MicroSeconds() >= TimeValue2.MicroSeconds()));
}

inline bool operator==(const TimeValue & TimeValue1, const TimeValue & TimeValue2)
{
	return (TimeValue1.Seconds() == TimeValue2.Seconds()) && (TimeValue1.MicroSeconds() == TimeValue2.MicroSeconds());
}

inline bool operator!=(const TimeValue & TimeValue1, const TimeValue & TimeValue2)
{
	return (TimeValue1.Seconds() != TimeValue2.Seconds()) || (TimeValue1.MicroSeconds() != TimeValue2.MicroSeconds());
}

#endif
