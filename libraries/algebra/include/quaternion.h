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
 * This is part of version 1.8.2 of algebra.
 **/

#ifndef ALGEBRA_QUATERNION_H
#define ALGEBRA_QUATERNION_H

#include <math.h>

#include "axis_angle.h"

class Matrix4f;
class Vector3f;
class Vector4f;

class Quaternion
{
	friend Matrix4f;
	friend Vector3f;
	friend Vector4f;
private:
	float _[4];
public:
	Quaternion(void)
	{
	}
	
	Quaternion(bool)
	{
		_[0] = 1.0f;
		_[1] = 0.0f;
		_[2] = 0.0f;
		_[3] = 0.0f;
	}
	
	Quaternion(float W, float X, float Y, float Z)
	{
		_[0] = W;
		_[1] = X;
		_[2] = Y;
		_[3] = Z;
	}
	
	static Quaternion CreateFromAxisAngle(const AxisAngle & AxisAngle)
	{
		return Quaternion().Rotation(AxisAngle);
	}
	
	static Quaternion CreateFromAxisComponentsAndAngle(float X, float Y, float Z, float AngleInRadians)
	{
		return Quaternion().Rotation(X, Y, Z, AngleInRadians);
	}
	
	static Quaternion CreateAsRotationX(float AngleInRadians)
	{
		return Quaternion().RotationX(AngleInRadians);
	}
	
	static Quaternion CreateAsRotationY(float AngleInRadians)
	{
		return Quaternion().RotationY(AngleInRadians);
	}
	
	static Quaternion CreateAsRotationZ(float AngleInRadians)
	{
		return Quaternion().RotationZ(AngleInRadians);
	}
	
	Quaternion & Identity(void)
	{
		_[0] = 1.0f;
		_[1] = 0.0f;
		_[2] = 0.0f;
		_[3] = 0.0f;
		
		return *this;
	}
	
	Quaternion & Conjugate(void)
	{
		_[1] = -_[1];
		_[2] = -_[2];
		_[3] = -_[3];
		
		return *this;
	}
	
	Quaternion Conjugated(void) const
	{
		return Quaternion(_[0], -_[1], -_[2], -_[3]);
	}
	
	Quaternion & RotationX(float Radians)
	{
		Radians /= 2.0f;
		_[0] = cos(Radians);
		_[1] = sin(Radians);
		_[2] = 0.0f;
		_[3] = 0.0f;
		
		return *this;
	}
	
	Quaternion & RotationY(float Radians)
	{
		Radians /= 2.0f;
		_[0] = cos(Radians);
		_[1] = 0.0f;
		_[2] = sin(Radians);
		_[3] = 0.0f;
		
		return *this;
	}
	
	Quaternion & RotationZ(float Radians)
	{
		Radians /= 2.0f;
		_[0] = cos(Radians);
		_[1] = 0.0f;
		_[2] = 0.0f;
		_[3] = sin(Radians);
		
		return *this;
	}
	
	Quaternion & Rotation(const AxisAngle & AxisAngle)
	{
		float AngleInRadians(AxisAngle._[3] / 2.0f);
		float SinusRadians(sin(AngleInRadians));
		
		_[0] = cos(AngleInRadians);
		_[1] = AxisAngle._[0] * SinusRadians;
		_[2] = AxisAngle._[1] * SinusRadians;
		_[3] = AxisAngle._[2] * SinusRadians;
		
		return *this;
	}
	
	Quaternion & Rotation(float X, float Y, float Z, float AngleInRadians)
	{
		AngleInRadians /= 2.0f;
		
		float SinusRadians(sin(AngleInRadians));
		
		_[0] = cos(AngleInRadians);
		_[1] = X * SinusRadians;
		_[2] = Y * SinusRadians;
		_[3] = Z * SinusRadians;
		
		return *this;
	}
	
	Quaternion Rotated(float X, float Y, float Z, float AngleInRadians)
	{
		AngleInRadians /= 2.0f;
		
		float Sinus(sin(AngleInRadians));
		float SinusX(Sinus * X);
		float SinusY(Sinus * Y);
		float SinusZ(Sinus * Z);
		float Cosinus(cos(AngleInRadians));
		
		return Quaternion(_[0] * Cosinus - _[1] * SinusX - _[2] * SinusY - _[3] * SinusZ, _[1] * Cosinus + _[0] * SinusX - _[3] * SinusY + _[2] * SinusZ, _[2] * Cosinus + _[3] * SinusX + _[0] * SinusY - _[1] * SinusZ, _[3] * Cosinus - _[2] * SinusX + _[1] * SinusY + _[0] * SinusZ);
	}
	
	Quaternion RotatedX(float AngleInRadians) const
	{
		AngleInRadians /= 2.0f;
		
		float Sinus(sin(AngleInRadians));
		float Cosinus(cos(AngleInRadians));
		
		return Quaternion(_[0] * Cosinus - _[1] * Sinus, _[1] * Cosinus + _[0] * Sinus, _[2] * Cosinus + _[3] * Sinus, _[3] * Cosinus - _[2] * Sinus);
	}
	
	Quaternion RotatedY(float AngleInRadians) const
	{
		AngleInRadians /= 2.0f;
		
		float Sinus(sin(AngleInRadians));
		float Cosinus(cos(AngleInRadians));
		
		return Quaternion(_[0] * Cosinus - _[2] * Sinus, _[1] * Cosinus - _[3] * Sinus, _[2] * Cosinus + _[0] * Sinus, _[3] * Cosinus + _[1] * Sinus);
	}
	
	Quaternion RotatedZ(float AngleInRadians) const
	{
		AngleInRadians /= 2.0f;
		
		float Sinus(sin(AngleInRadians));
		float Cosinus(cos(AngleInRadians));
		
		return Quaternion(_[0] * Cosinus - _[3] * Sinus, _[1] * Cosinus + _[2] * Sinus, _[2] * Cosinus - _[1] * Sinus, _[3] * Cosinus + _[0] * Sinus);
	}
	
	Quaternion & RotateX(float Radians)
	{
		return *this *= Quaternion::CreateAsRotationX(Radians);
	}
	
	Quaternion & RotateY(float Radians)
	{
		return *this *= Quaternion::CreateAsRotationY(Radians);
	}
	
	Quaternion & RotateZ(float Radians)
	{
		return *this *= Quaternion::CreateAsRotationZ(Radians);
	}
	
	Quaternion operator*(const Quaternion & Other) const
	{
		return Quaternion(
			_[0] * Other._[0] - _[1] * Other._[1] - _[2] * Other._[2] - _[3] * Other._[3],
			_[1] * Other._[0] + _[0] * Other._[1] - _[3] * Other._[2] + _[2] * Other._[3],
			_[2] * Other._[0] + _[3] * Other._[1] + _[0] * Other._[2] - _[1] * Other._[3],
			_[3] * Other._[0] - _[2] * Other._[1] + _[1] * Other._[2] + _[0] * Other._[3]
		);
	}
	
	Quaternion & operator*=(const Quaternion & Other)
	{
		float W(_[0] * Other._[0] - _[1] * Other._[1] - _[2] * Other._[2] - _[3] * Other._[3]);
		float X(_[1] * Other._[0] + _[0] * Other._[1] - _[3] * Other._[2] + _[2] * Other._[3]);
		float Y(_[2] * Other._[0] + _[3] * Other._[1] + _[0] * Other._[2] - _[1] * Other._[3]);
		float Z(_[3] * Other._[0] - _[2] * Other._[1] + _[1] * Other._[2] + _[0] * Other._[3]);
		
		_[0] = W;
		_[1] = X;
		_[2] = Y;
		_[3] = Z;
		
		return *this;
	}
	
	Quaternion & Set(float W, float X, float Y, float Z)
	{
		_[0] = W;
		_[1] = X;
		_[2] = Y;
		_[3] = Z;
		
		return *this;
	}
	
	float operator[](int Index) const
	{
		return _[Index];
	}
	
	float & operator[](int Index)
	{
		return _[Index];
	}
};

#endif
