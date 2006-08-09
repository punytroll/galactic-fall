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

void Camera::SetPosition(float X, float Y)
{
	m_Position.m_V.m_A[0] = X;
	m_Position.m_V.m_A[1] = Y;
}

void Camera::SetPosition(float X, float Y, float Z)
{
	m_Position.m_V.m_A[0] = X;
	m_Position.m_V.m_A[1] = Y;
	m_Position.m_V.m_A[2] = Z;
}

void Camera::ZoomIn(void)
{
	m_Position.m_V.m_A[2] *= 0.95f;
}

void Camera::ZoomOut(void)
{
	m_Position.m_V.m_A[2] *= 1.05f;
}
