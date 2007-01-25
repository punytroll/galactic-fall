#include <GL/gl.h>

#include "user_interface.h"
#include "widget.h"

UserInterface::UserInterface(void) :
	m_RootWidget(new Widget(0))
{
}

void UserInterface::Draw(void) const
{
	if(m_RootWidget->IsVisible() == true)
	{
		glPushMatrix();
		glTranslatef(m_RootWidget->GetPosition().m_V.m_A[0], m_RootWidget->GetPosition().m_V.m_A[1], 0.0f);
		m_RootWidget->Draw();
		glPopMatrix();
	}
}

Widget * UserInterface::GetWidget(const std::string & Path)
{
	if(Path[0] != '/')
	{
		return 0;
	}
	
	std::string::size_type Position(1);
	Widget * Root(GetRootWidget());
	
	while((Root != 0) && (Position < Path.length()))
	{
		std::string::size_type SlashPosition(Path.find('/', Position));
		
		Root = Root->GetSubWidget(Path.substr(Position, SlashPosition - Position));
		Position = ((SlashPosition == std::string::npos) ? (Path.length()) : (SlashPosition + 1));
	}
	
	return Root;
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

bool UserInterface::Key(int Key, int State)
{
	return m_RootWidget->Key(Key, State);
}

void UserInterface::MouseMotion(float X, float Y)
{
	const math3d::vector2f & LeftTopCorner(m_RootWidget->GetPosition());
	math3d::vector2f RightBottomCorner(LeftTopCorner + m_RootWidget->GetSize());
	
	if((X >= LeftTopCorner.m_V.m_A[0]) && (X < RightBottomCorner.m_V.m_A[0]) && (Y >= LeftTopCorner.m_V.m_A[1]) && (Y < RightBottomCorner.m_V.m_A[1]))
	{
		if(m_HoverWidget != m_RootWidget)
		{
			m_HoverWidget = m_RootWidget;
			m_RootWidget->MouseEnter();
		}
		m_RootWidget->MouseMotion(X, Y);
	}
	else
	{
		if(m_HoverWidget == m_RootWidget)
		{
			m_HoverWidget = 0;
			m_RootWidget->MouseLeave();
		}
	}
}
