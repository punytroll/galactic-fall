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

#include <assert.h>

#include <GL/gl.h>

#include "callbacks/callbacks.h"
#include "user_interface.h"
#include "widget.h"

UserInterface::UserInterface(void) :
	m_CaptureWidget(0),
	m_HoverWidget(0),
	m_RootWidget(new Widget(0))
{
	m_RootWidget->SetSize(Vector2f(1280.0f, 1024.0f));
	m_RootWidget->ConnectDestroyCallback(Callback(this, &UserInterface::OnRootWidgetDestroy));
}

UserInterface::~UserInterface(void)
{
	if(m_CaptureWidgetDestroyCallbackConnectionHandle.IsValid() == true)
	{
		assert(m_CaptureWidget != 0);
		m_CaptureWidget->DisconnectDestroyCallback(m_CaptureWidgetDestroyCallbackConnectionHandle);
	}
	else
	{
		assert(m_CaptureWidget == 0);
	}
	m_RootWidget->Destroy();
	m_RootWidget = 0;
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
		Widget::PopClippingRectangle();
		glPopMatrix();
	}
}

void UserInterface::SetCaptureWidget(Widget * Widget)
{
	if(m_CaptureWidget == 0)
	{
		m_CaptureWidget = Widget;
		m_CaptureWidgetDestroyCallbackConnectionHandle = m_CaptureWidget->ConnectDestroyCallback(Callback(this, &UserInterface::OnCaptureWidgetDestroy));
	}
}

void UserInterface::ReleaseCaptureWidget(void)
{
	if(m_CaptureWidget != 0)
	{
		m_CaptureWidget->DisconnectDestroyCallback(m_CaptureWidgetDestroyCallbackConnectionHandle);
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
		assert(m_RootWidget != 0);
		if(m_RootWidget->GetEnabled() == true)
		{
			const Vector2f & LeftTopCorner(m_RootWidget->GetPosition());
			Vector2f RightBottomCorner(LeftTopCorner + m_RootWidget->GetSize());
			
			if((X >= LeftTopCorner.m_V.m_A[0]) && (X < RightBottomCorner.m_V.m_A[0]) && (Y >= LeftTopCorner.m_V.m_A[1]) && (Y < RightBottomCorner.m_V.m_A[1]))
			{
				return m_RootWidget->MouseButton(Button, State, X - LeftTopCorner.m_V.m_A[0], Y - LeftTopCorner.m_V.m_A[1]);
			}
		}
	}
	else
	{
		if(m_CaptureWidget->GetEnabled() == true)
		{
			Vector2f TopLeftCorner(m_CaptureWidget->GetGlobalPosition());
			
			return m_CaptureWidget->MouseButton(Button, State, X - TopLeftCorner.m_V.m_A[0], Y - TopLeftCorner.m_V.m_A[1]);
		}
	}
	
	return false;
}

bool UserInterface::Key(const KeyEventInformation & KeyEventInformation)
{
	assert(m_RootWidget != 0);
	if(m_RootWidget->GetEnabled() == true)
	{
		return m_RootWidget->Key(KeyEventInformation);
	}
	
	return false;
}

void UserInterface::MouseMotion(float X, float Y)
{
	if(m_CaptureWidget == 0)
	{
		assert(m_RootWidget != 0);
		if(m_RootWidget->GetEnabled() == true)
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
	}
	else
	{
		if(m_CaptureWidget->GetEnabled() == true)
		{
			Vector2f TopLeftCorner(m_CaptureWidget->GetGlobalPosition());
			
			m_CaptureWidget->MouseMotion(X - TopLeftCorner.m_V.m_A[0], Y - TopLeftCorner.m_V.m_A[1]);
		}
	}
}

void UserInterface::OnCaptureWidgetDestroy(void)
{
	m_CaptureWidget->DisconnectDestroyCallback(m_CaptureWidgetDestroyCallbackConnectionHandle);
	m_CaptureWidget = 0;
}

void UserInterface::OnHoverWidgetDestroy(void)
{
	m_HoverWidget = 0;
}

void UserInterface::OnRootWidgetDestroy(void)
{
	m_RootWidget = 0;
}
