#include <GL/gl.h>

#include "user_interface.h"
#include "widget.h"

UserInterface::UserInterface(void) :
	m_RootWidget(new Widget(0))
{
}

void UserInterface::Draw(void) const
{
	glPushMatrix();
	glTranslatef(m_RootWidget->GetPosition().m_V.m_A[0], m_RootWidget->GetPosition().m_V.m_A[1], 0.0f);
	m_RootWidget->Draw();
	glPopMatrix();
}

bool UserInterface::MouseButton(int Button, int State, float X, float Y)
{
	const math3d::vector2f & LeftTopCorner(m_RootWidget->GetPosition());
	math3d::vector2f RightBottomCorner(LeftTopCorner + m_RootWidget->GetSize());
	
	if((X >= LeftTopCorner.m_V.m_A[0]) && (X < RightBottomCorner.m_V.m_A[0]) && (Y >= LeftTopCorner.m_V.m_A[1]) && (Y < RightBottomCorner.m_V.m_A[1]))
	{
		return m_RootWidget->MouseButton(Button, State, X - LeftTopCorner.m_V.m_A[0], Y - LeftTopCorner.m_V.m_A[1]);
	}
	else
	{
		return false;
	}
}
