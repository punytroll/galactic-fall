/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#include <GL/gl.h>

#include "user_interface.h"
#include "widget.h"

UserInterface::UserInterface(void) :
	m_RootWidget(new Widget(0))
{
	m_RootWidget->SetSize(Vector2f(1280.0f, 1024.0f));
	m_RootWidget->AddDestroyListener(this);
}

void UserInterface::Draw(void) const
{
	if((m_RootWidget != 0) && (m_RootWidget->IsVisible() == true))
	{
		glPushMatrix();
		Widget::PushClippingRectangle(m_RootWidget->GetPosition(), m_RootWidget->GetSize());
		glTranslatef(m_RootWidget->GetPosition().m_V.m_A[0], m_RootWidget->GetPosition().m_V.m_A[1], 0.0f);
		Widget::DrawClippingRectangle();
		m_RootWidget->Draw();
		glPopMatrix();
	}
}

void UserInterface::SetCaptureWidget(Widget * Widget)
{
	if(m_CaptureWidget == 0)
	{
		m_CaptureWidget = Widget;
		m_CaptureWidget->AddDestroyListener(this);
	}
}

void UserInterface::ReleaseCaptureWidget(void)
{
	if(m_CaptureWidget != 0)
	{
		m_CaptureWidget->RemoveDestroyListener(this);
		m_CaptureWidget = 0;
	}
}

Widget * UserInterface::GetWidget(const std::string & Path)
{
	if(Path[0] != '/')
	{
		return 0;
	}
	
	std::string::size_type Position(1);
	Widget * Root(m_RootWidget);
	
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
	if(m_CaptureWidget == 0)
	{
		const Vector2f & LeftTopCorner(m_RootWidget->GetPosition());
		Vector2f RightBottomCorner(LeftTopCorner + m_RootWidget->GetSize());
		
		if((X >= LeftTopCorner.m_V.m_A[0]) && (X < RightBottomCorner.m_V.m_A[0]) && (Y >= LeftTopCorner.m_V.m_A[1]) && (Y < RightBottomCorner.m_V.m_A[1]))
		{
			return m_RootWidget->MouseButton(Button, State, X - LeftTopCorner.m_V.m_A[0], Y - LeftTopCorner.m_V.m_A[1]);
		}
		else
		{
			return false;
		}
	}
	else
	{
		Vector2f TopLeftCorner(m_CaptureWidget->GetGlobalPosition());
		
		return m_CaptureWidget->MouseButton(Button, State, X - TopLeftCorner.m_V.m_A[0], Y - TopLeftCorner.m_V.m_A[1]);
	}
}

bool UserInterface::Key(int Key, int State)
{
	return m_RootWidget->Key(Key, State);
}

void UserInterface::MouseMotion(float X, float Y)
{
	if(m_CaptureWidget == 0)
	{
		const Vector2f & LeftTopCorner(m_RootWidget->GetPosition());
		Vector2f RightBottomCorner(LeftTopCorner + m_RootWidget->GetSize());
		
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
	else
	{
		Vector2f TopLeftCorner(m_CaptureWidget->GetGlobalPosition());
		
		m_CaptureWidget->MouseMotion(X - TopLeftCorner.m_V.m_A[0], Y - TopLeftCorner.m_V.m_A[1]);
	}
}

void UserInterface::OnDestroy(Widget * EventSource)
{
	if(EventSource == m_CaptureWidget)
	{
		m_CaptureWidget = 0;
	}
	else if(EventSource == m_RootWidget)
	{
		m_RootWidget = 0;
		m_HoverWidget = 0;
	}
}
