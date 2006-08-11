#ifndef POSITION_H
#define POSITION_H

#include <math3d/vector2f.h>

class Position
{
public:
	void SetPosition(const math3d::vector2f & Position);
	const math3d::vector2f & GetPosition(void) const;
	math3d::vector2f m_Position;
};

inline void Position::SetPosition(const math3d::vector2f & Position)
{
	m_Position = Position;
}

inline const math3d::vector2f & Position::GetPosition(void) const
{
	return m_Position;
}

#endif
