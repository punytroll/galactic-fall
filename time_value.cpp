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

#include "time_value.h"

TimeValue::TimeValue(void) :
	m_Seconds(0),
	m_MicroSeconds(0)
{
}

TimeValue::TimeValue(double Seconds) :
	m_Seconds(static_cast< int32_t >(Seconds)),
	m_MicroSeconds(static_cast< int32_t >((Seconds - m_Seconds) * 1000000))
{
}

TimeValue::TimeValue(uint32_t Seconds, uint32_t MicroSeconds) :
	m_Seconds(Seconds),
	m_MicroSeconds(MicroSeconds)
{
}

TimeValue::TimeValue(const TimeValue & TimeValue) :
	m_Seconds(TimeValue.Seconds()),
	m_MicroSeconds(TimeValue.MicroSeconds())
{
}
