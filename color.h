#ifndef COLOR_H
#define COLOR_H

#include <math3d/vector4f.h>

class Color
{
public:
	Color(float Red, float Green, float Blue);
	const math3d::vector4f & GetColor(void) const;
	void Set(float Red, float Green, float Blue);
private:
	math3d::vector4f m_Color;
};

inline const math3d::vector4f & Color::GetColor(void) const
{
	return m_Color;
}

#endif
