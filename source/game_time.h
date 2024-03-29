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

#ifndef GAME_TIME_H
#define GAME_TIME_H

#include <cstdint>

class GameTime
{
public:
	static void Set(double Time);
	static double Get(void);
	static std::uint32_t GetCycle(void);
	static double GetCycleFraction(void);
private:
	static double _Cycle;
	static double _Time;
};

inline void GameTime::Set(double Time)
{
	_Cycle = Time / 2971.0;
	_Time = Time;
}

inline double GameTime::Get(void)
{
	return _Time;
}

#endif
