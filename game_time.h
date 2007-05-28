#ifndef GAME_TIME_H
#define GAME_TIME_H

class GameTime
{
public:
	static void Set(double Time);
	static double Get(void);
private:
	static double m_Time;
};

inline void GameTime::Set(double Time)
{
	m_Time = Time;
}

inline double GameTime::Get(void)
{
	return m_Time;
}

#endif
