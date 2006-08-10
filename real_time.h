#ifndef REAL_TIME_H
#define REAL_TIME_H

class RealTime
{
public:
	static void Invalidate(void);
	static double GetTime(void);
	static double GetInterval(void);
private:
	static bool m_Valid;
	static double m_Time;
};

#endif
