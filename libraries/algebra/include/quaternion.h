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
 * This is part of version 1.5.1 of algebra.
 **/

#ifndef ALGEBRA_QUATERNION_H
#define ALGEBRA_QUATERNION_H

#include <math.h>

#include "axis_angle.h"
#include "details.h"

class Quaternion
{
public:
	enum Initializer
	{
		InitializeRotationX,
		InitializeRotationY,
		InitializeRotationZ
	};
	
	Array4f m_V;
	
	Quaternion(void)
	{
	}
	
	Quaternion(float W, float X, float Y, float Z)
	{
		m_V.m_A[0] = W;
		m_V.m_A[1] = X;
		m_V.m_A[2] = Y;
		m_V.m_A[3] = Z;
	}
	
	Quaternion(const AxisAngle & AxisAngle)
	{
		Rotation(AxisAngle);
	}
	
	Quaternion(Array4f Axis, float AngleInRadians)
	{
		Rotation(Axis.m_A[0], Axis.m_A[1], Axis.m_A[2], AngleInRadians);
	}
	
	Quaternion(bool)
	{
		Identity();
	}
	
	Quaternion(float Radians, Initializer Initializer)
	{
		if(Initializer == InitializeRotationX)
		{
			RotationX(Radians);
		}
		else if(Initializer == InitializeRotationY)
		{
			RotationY(Radians);
		}
		else if(Initializer == InitializeRotationZ)
		{
			RotationZ(Radians);
		}
	}
	
	void Identity(void)
	{
		m_V.m_A[0] = 1.0f;
		m_V.m_A[1] = m_V.m_A[2] = m_V.m_A[3] = 0.0f;
	}
	
	Quaternion & Conjugate(void)
	{
		m_V.m_A[1] = -m_V.m_A[1];
		m_V.m_A[2] = -m_V.m_A[2];
		m_V.m_A[3] = -m_V.m_A[3];
		
		return *this;
	}
	
	Quaternion Conjugated(void) const
	{
		return Quaternion(m_V.m_A[0], -m_V.m_A[1], -m_V.m_A[2], -m_V.m_A[3]);
	}
	
	void RotationX(float Radians)
	{
		Radians /= 2.0f;
		m_V.m_A[0] = cos(Radians);
		m_V.m_A[1] = sin(Radians);
		m_V.m_A[2] = 0.0f;
		m_V.m_A[3] = 0.0f;
	}
	
	void RotationY(float Radians)
	{
		Radians /= 2.0f;
		m_V.m_A[0] = cos(Radians);
		m_V.m_A[1] = 0.0f;
		m_V.m_A[2] = sin(Radians);
		m_V.m_A[3] = 0.0f;
	}
	
	void RotationZ(float Radians)
	{
		Radians /= 2.0f;
		m_V.m_A[0] = cos(Radians);
		m_V.m_A[1] = 0.0f;
		m_V.m_A[2] = 0.0f;
		m_V.m_A[3] = sin(Radians);
	}
	
	void Rotation(const AxisAngle & AxisAngle)
	{
		Rotation(AxisAngle.m_V.m_A[0], AxisAngle.m_V.m_A[1], AxisAngle.m_V.m_A[2], AxisAngle.m_V.m_A[3]);
	}
	
	void Rotation(float X, float Y, float Z, float AngleInRadians)
	{
		AngleInRadians /= 2.0;
		
		float SinusRadians(sin(AngleInRadians));
		
		m_V.m_A[0] = cos(AngleInRadians);
		m_V.m_A[1] = X * SinusRadians;
		m_V.m_A[2] = Y * SinusRadians;
		m_V.m_A[3] = Z * SinusRadians;
	}
	
	Quaternion & Rotate(Array4f Axis, float Radians)
	{
		return *this *= Quaternion(Axis, Radians);
	}
	
	Quaternion Rotated(float X, float Y, float Z, float AngleInRadians)
	{
		AngleInRadians /= 2.0;
		
		float Sinus(sin(AngleInRadians));
		float SinusX(Sinus * X);
		float SinusY(Sinus * Y);
		float SinusZ(Sinus * Z);
		float Cosinus(cos(AngleInRadians));
		
		return Quaternion(m_V.m_A[0] * Cosinus - m_V.m_A[1] * SinusX - m_V.m_A[2] * SinusY - m_V.m_A[3] * SinusZ, m_V.m_A[1] * Cosinus + m_V.m_A[0] * SinusX - m_V.m_A[3] * SinusY + m_V.m_A[2] * SinusZ, m_V.m_A[2] * Cosinus + m_V.m_A[3] * SinusX + m_V.m_A[0] * SinusY - m_V.m_A[1] * SinusZ, m_V.m_A[3] * Cosinus - m_V.m_A[2] * SinusX + m_V.m_A[1] * SinusY + m_V.m_A[0] * SinusZ);
	}
	
	Quaternion RotatedX(float AngleInRadians) const
	{
		AngleInRadians /= 2.0;
		
		float Sinus(sin(AngleInRadians));
		float Cosinus(cos(AngleInRadians));
		
		return Quaternion(m_V.m_A[0] * Cosinus - m_V.m_A[1] * Sinus, m_V.m_A[1] * Cosinus + m_V.m_A[0] * Sinus, m_V.m_A[2] * Cosinus + m_V.m_A[3] * Sinus, m_V.m_A[3] * Cosinus - m_V.m_A[2] * Sinus);
	}
	
	Quaternion RotatedY(float AngleInRadians) const
	{
		AngleInRadians /= 2.0;
		
		float Sinus(sin(AngleInRadians));
		float Cosinus(cos(AngleInRadians));
		
		return Quaternion(m_V.m_A[0] * Cosinus - m_V.m_A[2] * Sinus, m_V.m_A[1] * Cosinus - m_V.m_A[3] * Sinus, m_V.m_A[2] * Cosinus + m_V.m_A[0] * Sinus, m_V.m_A[3] * Cosinus + m_V.m_A[1] * Sinus);
	}
	
	Quaternion RotatedZ(float AngleInRadians) const
	{
		AngleInRadians /= 2.0;
		
		float Sinus(sin(AngleInRadians));
		float Cosinus(cos(AngleInRadians));
		
		return Quaternion(m_V.m_A[0] * Cosinus - m_V.m_A[3] * Sinus, m_V.m_A[1] * Cosinus + m_V.m_A[2] * Sinus, m_V.m_A[2] * Cosinus - m_V.m_A[1] * Sinus, m_V.m_A[3] * Cosinus + m_V.m_A[0] * Sinus);
	}
	
	Quaternion & RotateX(float Radians)
	{
		return *this *= Quaternion(Radians, InitializeRotationX);
	}
	
	Quaternion & RotateY(float Radians)
	{
		return *this *= Quaternion(Radians, InitializeRotationY);
	}
	
	Quaternion & RotateZ(float Radians)
	{
		return *this *= Quaternion(Radians, InitializeRotationZ);
	}
	
	Quaternion operator*(const Quaternion & Other) const
	{
		return Quaternion(
			m_V.m_A[0] * Other.m_V.m_A[0] - m_V.m_A[1] * Other.m_V.m_A[1] - m_V.m_A[2] * Other.m_V.m_A[2] - m_V.m_A[3] * Other.m_V.m_A[3],
			m_V.m_A[1] * Other.m_V.m_A[0] + m_V.m_A[0] * Other.m_V.m_A[1] - m_V.m_A[3] * Other.m_V.m_A[2] + m_V.m_A[2] * Other.m_V.m_A[3],
			m_V.m_A[2] * Other.m_V.m_A[0] + m_V.m_A[3] * Other.m_V.m_A[1] + m_V.m_A[0] * Other.m_V.m_A[2] - m_V.m_A[1] * Other.m_V.m_A[3],
			m_V.m_A[3] * Other.m_V.m_A[0] - m_V.m_A[2] * Other.m_V.m_A[1] + m_V.m_A[1] * Other.m_V.m_A[2] + m_V.m_A[0] * Other.m_V.m_A[3]
		);
	}
	
	Quaternion & operator*=(const Quaternion & Other)
	{
		float W(m_V.m_A[0] * Other.m_V.m_A[0] - m_V.m_A[1] * Other.m_V.m_A[1] - m_V.m_A[2] * Other.m_V.m_A[2] - m_V.m_A[3] * Other.m_V.m_A[3]);
		float X(m_V.m_A[1] * Other.m_V.m_A[0] + m_V.m_A[0] * Other.m_V.m_A[1] - m_V.m_A[3] * Other.m_V.m_A[2] + m_V.m_A[2] * Other.m_V.m_A[3]);
		float Y(m_V.m_A[2] * Other.m_V.m_A[0] + m_V.m_A[3] * Other.m_V.m_A[1] + m_V.m_A[0] * Other.m_V.m_A[2] - m_V.m_A[1] * Other.m_V.m_A[3]);
		float Z(m_V.m_A[3] * Other.m_V.m_A[0] - m_V.m_A[2] * Other.m_V.m_A[1] + m_V.m_A[1] * Other.m_V.m_A[2] + m_V.m_A[0] * Other.m_V.m_A[3]);
		
		m_V.m_A[0] = W;
		m_V.m_A[1] = X;
		m_V.m_A[2] = Y;
		m_V.m_A[3] = Z;
		
		return *this;
	}
	
	void Set(float W, float X, float Y, float Z)
	{
		m_V.m_A[0] = W;
		m_V.m_A[1] = X;
		m_V.m_A[2] = Y;
		m_V.m_A[3] = Z;
	}
	
	float operator[](int Index) const
	{
		return m_V.m_A[Index];
	}
	
	float & operator[](int Index)
	{
		return m_V.m_A[Index];
	}
};

#endif
