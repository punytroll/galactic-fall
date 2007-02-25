#ifndef MATH_H
#define MATH_H

#include <math3d/vector2f.h>

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

inline float GetRadians(const math3d::vector2f & Vector)
{
	float Radians(acosf(Vector.m_V.m_A[0]));
	
	if(Vector.m_V.m_A[1] >= 0)
	{
		return Radians;
	}
	else
	{
		return NormalizedRadians(-Radians);
	}
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

#endif
