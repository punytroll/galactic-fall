/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#include <sys/time.h>

#include "real_time.h"

bool RealTime::m_Valid(false);
double RealTime::m_Time(0.0);

void RealTime::Invalidate(void)
{
	m_Valid = false;
}

double RealTime::GetTime(void)
{
	if(m_Valid == false)
	{
		timeval TimeValue;
		
		gettimeofday(&TimeValue, 0);
		m_Time = TimeValue.tv_sec + 0.000001 * TimeValue.tv_usec;
		m_Valid = true;
	}
	
	return m_Time;
}

double RealTime::GetInterval(void)
{
	timeval TimeValue;
	
	gettimeofday(&TimeValue, 0);
	
	if(m_Valid == false)
	{
		m_Time = TimeValue.tv_sec + 0.000001 * TimeValue.tv_usec;
		m_Valid = true;
		
		return 0.0;
	}
	else
	{
		double NewTime(TimeValue.tv_sec + 0.000001 * TimeValue.tv_usec);
		double Interval(NewTime - m_Time);
		
		m_Time = NewTime;
		
		return Interval;
	}
}
