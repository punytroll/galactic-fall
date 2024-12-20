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

#ifndef MATH_H
#define MATH_H

#include <stdlib.h>

#include <cstdint>
#include <cassert>

#include <algebra/vector2f.h>
#include <algebra/vector3f.h>
#include <algebra/vector4d.h>

const float g_Epsilon(0.01f);

inline void NormalizeRadians(float & Radians)
{
	while(Radians < 0.0f)
	{
		Radians += 2 * M_PI;
	}
	while(Radians > 2 * M_PI)
	{
		Radians -= 2 * M_PI;
	}
}

inline float NormalizedRadians(float Radians)
{
	while(Radians < 0.0f)
	{
		Radians += 2 * M_PI;
	}
	while(Radians > 2 * M_PI)
	{
		Radians -= 2 * M_PI;
	}
	
	return Radians;
}

inline float GetRadians(float X, float Y)
{
	float Radians{acosf(X)};
	
	if(Y >= 0)
	{
		return Radians;
	}
	else
	{
		return NormalizedRadians(-Radians);
	}
}

inline float GetRadians(const Vector2f & Vector)
{
	return GetRadians(Vector[0], Vector[1]);
}

inline float GetShortestRadians(float Heading1, float Heading2)
{
	float Off(Heading1 - Heading2);
	
	if(Off < -M_PI)
	{
		Off += 2.0f * M_PI;
	}
	else if(Off > M_PI)
	{
		Off -= 2.0f * M_PI;
	}
	
	return Off;
}

template < typename Type >
inline Type Clamp(Type Value, Type Minimum, Type Maximum)
{
	return ((Value < Minimum) ? (Minimum) : ((Value > Maximum) ? (Maximum) : (Value)));
}

inline long int GetRandomInteger(long int HighestValue)
{
	return random() % (HighestValue + 1);
}

inline std::uint32_t GetRandomUnsignedInteger32Bit(std::uint32_t LowestValue, std::uint32_t HighestValue)
{
	assert(LowestValue <= HighestValue);
	
	return LowestValue + (random() % (HighestValue - LowestValue + 1));
}

inline float GetRandomUniform(void)
{
	return static_cast< float >(random()) / RAND_MAX;
}

inline float GetRandomFloat(float LowestValue, float HighestValue)
{
	assert(LowestValue <= HighestValue);
	
	return ((HighestValue - LowestValue) * GetRandomUniform()) + LowestValue;
}

inline float GetRandomRadians(void)
{
	return GetRandomUniform() * 2.0f * M_PI;
}

inline bool GetRandomBoolean(float Probability = 0.5f)
{
	return GetRandomUniform() <= Probability;
}

inline float GetRandomFloatFromExponentialDistribution(float Interval)
{
	return Interval * -log(GetRandomUniform());
}

inline int GetRandomIntegerFromExponentialDistribution(float Interval)
{
	return static_cast< int >(GetRandomFloatFromExponentialDistribution(Interval));
}

inline double GetRandomUniformDouble(void)
{
	return static_cast< double >(random()) / RAND_MAX;
}

inline double GetRandomDouble(double LowestValue, double HighestValue)
{
	assert(LowestValue <= HighestValue);
	
	return ((HighestValue - LowestValue) * GetRandomUniformDouble()) + LowestValue;
}

inline Vector4d GetPlaneEquation(const Vector3f & Point1, const Vector3f & Point2, const Vector3f & Point3)
{
	return Vector4d(Point1[1] * (Point2[2] - Point3[2]) + Point2[1] * (Point3[2] - Point1[2]) + Point3[1] * (Point1[2] - Point2[2]), Point1[2] * (Point2[0] - Point3[0]) + Point2[2] * (Point3[0] - Point1[0]) + Point3[2] * (Point1[0] - Point2[0]), Point1[0] * (Point2[1] - Point3[1]) + Point2[0] * (Point3[1] - Point1[1]) + Point3[0] * (Point1[1] - Point2[1]), -(Point1[0] * (Point2[1] * Point3[2] - Point3[1] * Point2[2]) + Point2[0] * (Point3[1] * Point1[2] - Point1[1] * Point3[2]) + Point3[0] * (Point1[1] * Point2[2] - Point2[1] * Point1[2])));
}

#endif
