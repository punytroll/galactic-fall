/**
 * algebra
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
 * This is part of version 1.8.7 of algebra.
 **/

#ifndef ALGEBRA_AXIS_ANGLE_H
#define ALGEBRA_AXIS_ANGLE_H

class Quaternion;

class AxisAngle
{
	friend Quaternion;
private:
	float _[4];
public:
	AxisAngle(void)
	{
	}
	
	AxisAngle(bool)
	{
		_[0] = 0.0f;
		_[1] = 0.0f;
		_[2] = 0.0f;
		_[3] = 0.0f;
	}
	
	AxisAngle(float X, float Y, float Z, float Angle)
	{
		_[0] = X;
		_[1] = Y;
		_[2] = Z;
		_[3] = Angle;
	}
	
	void Set(float X, float Y, float Z, float Angle)
	{
		_[0] = X;
		_[1] = Y;
		_[2] = Z;
		_[3] = Angle;
	}
	
	const float & operator[](int Index) const
	{
		return _[Index];
	}
	
	float & operator[](int Index)
	{
		return _[Index];
	}
};

#endif
