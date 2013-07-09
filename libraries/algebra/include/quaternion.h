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
 * This is part of version 1.5.5 of algebra.
 **/

#ifndef ALGEBRA_QUATERNION_H
#define ALGEBRA_QUATERNION_H

#include <math.h>

#include "axis_angle.h"
#include "details.h"

class Quaternion
{
public:
	Array4f m_V;
	
	Quaternion(void)
	{
	}
	
	Quaternion(bool)
	{
		m_V.m_A[0] = 1.0f;
		m_V.m_A[1] = 0.0f;
		m_V.m_A[2] = 0.0f;
		m_V.m_A[3] = 0.0f;
	}
	
	Quaternion(float W, float X, float Y, float Z)
	{
		m_V.m_A[0] = W;
		m_V.m_A[1] = X;
		m_V.m_A[2] = Y;
		m_V.m_A[3] = Z;
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
		m_V.m_A[0] = 1.0f;
		m_V.m_A[1] = 0.0f;
		m_V.m_A[2] = 0.0f;
		m_V.m_A[3] = 0.0f;
		
		return *this;
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
	
	Quaternion & RotationX(float Radians)
	{
		Radians /= 2.0f;
		m_V.m_A[0] = cos(Radians);
		m_V.m_A[1] = sin(Radians);
		m_V.m_A[2] = 0.0f;
		m_V.m_A[3] = 0.0f;
		
		return *this;
	}
	
	Quaternion & RotationY(float Radians)
	{
		Radians /= 2.0f;
		m_V.m_A[0] = cos(Radians);
		m_V.m_A[1] = 0.0f;
		m_V.m_A[2] = sin(Radians);
		m_V.m_A[3] = 0.0f;
		
		return *this;
	}
	
	Quaternion & RotationZ(float Radians)
	{
		Radians /= 2.0f;
		m_V.m_A[0] = cos(Radians);
		m_V.m_A[1] = 0.0f;
		m_V.m_A[2] = 0.0f;
		m_V.m_A[3] = sin(Radians);
		
		return *this;
	}
	
	Quaternion & Rotation(const AxisAngle & AxisAngle)
	{
		float AngleInRadians(AxisAngle._[3] / 2.0f);
		float SinusRadians(sin(AngleInRadians));
		
		m_V.m_A[0] = cos(AngleInRadians);
		m_V.m_A[1] = AxisAngle._[0] * SinusRadians;
		m_V.m_A[2] = AxisAngle._[1] * SinusRadians;
		m_V.m_A[3] = AxisAngle._[2] * SinusRadians;
		
		return *this;
	}
	
	Quaternion & Rotation(float X, float Y, float Z, float AngleInRadians)
	{
		AngleInRadians /= 2.0f;
		
		float SinusRadians(sin(AngleInRadians));
		
		m_V.m_A[0] = cos(AngleInRadians);
		m_V.m_A[1] = X * SinusRadians;
		m_V.m_A[2] = Y * SinusRadians;
		m_V.m_A[3] = Z * SinusRadians;
		
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
		
		return Quaternion(m_V.m_A[0] * Cosinus - m_V.m_A[1] * SinusX - m_V.m_A[2] * SinusY - m_V.m_A[3] * SinusZ, m_V.m_A[1] * Cosinus + m_V.m_A[0] * SinusX - m_V.m_A[3] * SinusY + m_V.m_A[2] * SinusZ, m_V.m_A[2] * Cosinus + m_V.m_A[3] * SinusX + m_V.m_A[0] * SinusY - m_V.m_A[1] * SinusZ, m_V.m_A[3] * Cosinus - m_V.m_A[2] * SinusX + m_V.m_A[1] * SinusY + m_V.m_A[0] * SinusZ);
	}
	
	Quaternion RotatedX(float AngleInRadians) const
	{
		AngleInRadians /= 2.0f;
		
		float Sinus(sin(AngleInRadians));
		float Cosinus(cos(AngleInRadians));
		
		return Quaternion(m_V.m_A[0] * Cosinus - m_V.m_A[1] * Sinus, m_V.m_A[1] * Cosinus + m_V.m_A[0] * Sinus, m_V.m_A[2] * Cosinus + m_V.m_A[3] * Sinus, m_V.m_A[3] * Cosinus - m_V.m_A[2] * Sinus);
	}
	
	Quaternion RotatedY(float AngleInRadians) const
	{
		AngleInRadians /= 2.0f;
		
		float Sinus(sin(AngleInRadians));
		float Cosinus(cos(AngleInRadians));
		
		return Quaternion(m_V.m_A[0] * Cosinus - m_V.m_A[2] * Sinus, m_V.m_A[1] * Cosinus - m_V.m_A[3] * Sinus, m_V.m_A[2] * Cosinus + m_V.m_A[0] * Sinus, m_V.m_A[3] * Cosinus + m_V.m_A[1] * Sinus);
	}
	
	Quaternion RotatedZ(float AngleInRadians) const
	{
		AngleInRadians /= 2.0f;
		
		float Sinus(sin(AngleInRadians));
		float Cosinus(cos(AngleInRadians));
		
		return Quaternion(m_V.m_A[0] * Cosinus - m_V.m_A[3] * Sinus, m_V.m_A[1] * Cosinus + m_V.m_A[2] * Sinus, m_V.m_A[2] * Cosinus - m_V.m_A[1] * Sinus, m_V.m_A[3] * Cosinus + m_V.m_A[0] * Sinus);
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
	
	Quaternion & Set(float W, float X, float Y, float Z)
	{
		m_V.m_A[0] = W;
		m_V.m_A[1] = X;
		m_V.m_A[2] = Y;
		m_V.m_A[3] = Z;
		
		return *this;
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
