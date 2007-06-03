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

inline long int GetRandomInteger(long int HighestValue)
{
	return random() % (HighestValue + 1);
}

inline float GetRandomUniform(void)
{
	return static_cast< float >(random()) / RAND_MAX;
}

inline float GetRandomFloat(float LowestValue, float HighestValue)
{
	return ((HighestValue - LowestValue) * GetRandomUniform()) + LowestValue;
}

inline long int GetRandomBoolean(float Probability = 0.5f)
{
	return GetRandomUniform() <= Probability;
}

#endif
