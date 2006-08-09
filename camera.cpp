#include <GL/gl.h>

#include "camera.h"
#include "position.h"

Camera::Camera(void) :
	m_Focus(0)
{
}

void Camera::Draw(void)
{
	glTranslatef(-m_Position.m_V.m_A[0], -m_Position.m_V.m_A[1], -m_Position.m_V.m_A[2]);
	if(m_Focus != 0)
	{
		glTranslatef(-m_Focus->m_Position.m_V.m_A[0], -m_Focus->m_Position.m_V.m_A[1], 0.0f);
	}
}

void Camera::SetFocus(Position * Focus)
{
	m_Focus = Focus;
}
