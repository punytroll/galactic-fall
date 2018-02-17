/**
 * galactic-fall
 * Copyright (C) 2006-2018  Hagen Möbius
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
#include <time.h>

#include "real_time.h"

bool RealTime::_Valid(false);
double RealTime::_Time(0.0);

void RealTime::Invalidate(void)
{
	_Valid = false;
}

double RealTime::Get(void)
{
	if(_Valid == false)
	{
		timeval TimeValue;
		
		gettimeofday(&TimeValue, 0);
		_Time = TimeValue.tv_sec + 0.000001 * TimeValue.tv_usec;
		_Valid = true;
	}
	
	return _Time;
}

double RealTime::GetSecondsSinceEpoche(void)
{
	auto CurrentSeconds{time(nullptr)};
	tm Epoche;
	
	Epoche.tm_sec = 0;
	Epoche.tm_min = 17;
	Epoche.tm_hour = 4;
	Epoche.tm_mday = 7;
	Epoche.tm_mon = 6;
	Epoche.tm_year = 108;
	Epoche.tm_isdst = -1;
	
	auto EpocheSeconds{mktime(&Epoche)};
	
	return CurrentSeconds - EpocheSeconds;
}
