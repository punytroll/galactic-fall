#include "color.h"

Color::Color(float Red, float Green, float Blue, float Alpha) :
	m_Color(Red, Green, Blue, Alpha)
{
}

void Color::Set(float Red, float Green, float Blue, float Alpha)
{
	m_Color.set(Red, Green, Blue, Alpha);
}
