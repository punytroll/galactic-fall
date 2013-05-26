#include <GL/gl.h>

#include "position.h"

Position::Position(void)
{
}

Position::Position(float fX, float fY, float fZ) :
	m_Position(fX, fY, fZ, 1.0f)
{
}

Position::Position(const Position & Position) :
	m_Position(Position.m_Position)
{
}

Position::~Position(void)
{
}

void Position::vDraw(void)
{
	glVertex3fv(m_Position.m_V.m_A);
}

void Position::vDrawSelection(void)
{
	glVertex3fv(m_Position.m_V.m_A);
}
