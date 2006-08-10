#include <sys/time.h>

#include "real_time.h"

bool RealTime::m_Valid(false);
double RealTime::m_Time(0.0);

void RealTime::Invalidate(void)
{
	m_Valid = false;
}

double RealTime::GetTime(void)
{
	if(m_Valid == false)
	{
		timeval TimeValue;
		
		gettimeofday(&TimeValue, 0);
		m_Time = TimeValue.tv_sec + 0.000001 * TimeValue.tv_usec;
		m_Valid = true;
	}
	
	return m_Time;
}

double RealTime::GetInterval(void)
{
	timeval TimeValue;
	
	gettimeofday(&TimeValue, 0);
	
	if(m_Valid == false)
	{
		m_Time = TimeValue.tv_sec + 0.000001 * TimeValue.tv_usec;
		m_Valid = true;
		
		return 0.0;
	}
	else
	{
		double NewTime(TimeValue.tv_sec + 0.000001 * TimeValue.tv_usec);
		double Interval(NewTime - m_Time);
		
		m_Time = NewTime;
		
		return Interval;
	}
}
