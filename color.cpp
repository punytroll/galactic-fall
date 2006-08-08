#include "color.h"

Color::Color(float Red, float Green, float Blue) :
	m_Color(Red, Green, Blue, 0.0f)
{
}

void Color::Set(float Red, float Green, float Blue)
{
	m_Color.set(Red, Green, Blue, 0.0f);
}
