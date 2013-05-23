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

#include "../callbacks/callbacks.h"
#include "../graphics/gl.h"
#include "user_interface.h"
#include "widget.h"

UI::UserInterface::UserInterface(void) :
	m_CaptureWidget(0),
	m_HoverWidget(0),
	m_RootWidget(new UI::Widget(0))
{
	m_RootWidget->SetSize(Vector2f(1280.0f, 1024.0f));
	m_RootWidget->ConnectDestroyingCallback(Callback(this, &UI::UserInterface::OnRootWidgetDestroying));
}

UI::UserInterface::~UserInterface(void)
{
	if(m_CaptureWidgetDestroyingCallbackConnectionHandle.IsValid() == true)
	{
		assert(m_CaptureWidget != 0);
		m_CaptureWidget->DisconnectDestroyingCallback(m_CaptureWidgetDestroyingCallbackConnectionHandle);
	}
	else
	{
		assert(m_CaptureWidget == 0);
	}
	m_RootWidget->Destroy();
	m_RootWidget = 0;
}

void UI::UserInterface::Draw(void) const
{
	if((m_RootWidget != 0) && (m_RootWidget->IsVisible() == true))
	{
		GLPushMatrix();
		UI::Widget::PushClippingRectangle(m_RootWidget->GetPosition(), m_RootWidget->GetSize());
		glTranslatef(m_RootWidget->GetPosition().m_V.m_A[0], m_RootWidget->GetPosition().m_V.m_A[1], 0.0f);
		UI::Widget::DrawClippingRectangle();
		m_RootWidget->Draw();
		UI::Widget::PopClippingRectangle();
		GLPopMatrix();
	}
}

void UI::UserInterface::SetCaptureWidget(UI::Widget * Widget)
{
	if(m_CaptureWidget == 0)
	{
		m_CaptureWidget = Widget;
		m_CaptureWidgetDestroyingCallbackConnectionHandle = m_CaptureWidget->ConnectDestroyingCallback(Callback(this, &UI::UserInterface::OnCaptureWidgetDestroying));
	}
}

void UI::UserInterface::ReleaseCaptureWidget(void)
{
	if(m_CaptureWidget != 0)
	{
		m_CaptureWidget->DisconnectDestroyingCallback(m_CaptureWidgetDestroyingCallbackConnectionHandle);
		m_CaptureWidget = 0;
	}
}

UI::Widget * UI::UserInterface::GetWidget(const std::string & Path)
{
	if(Path[0] != '/')
	{
		return 0;
	}
	
	std::string::size_type Position(1);
	UI::Widget * Root(m_RootWidget);
	
	while((Root != 0) && (Position < Path.length()))
	{
		std::string::size_type SlashPosition(Path.find('/', Position));
		
		Root = Root->GetSubWidget(Path.substr(Position, SlashPosition - Position));
		Position = ((SlashPosition == std::string::npos) ? (Path.length()) : (SlashPosition + 1));
	}
	
	return Root;
}

bool UI::UserInterface::MouseButton(int Button, int State, float X, float Y)
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

bool UI::UserInterface::Key(const KeyEventInformation & KeyEventInformation)
{
	assert(m_RootWidget != 0);
	if(m_RootWidget->GetEnabled() == true)
	{
		return m_RootWidget->Key(KeyEventInformation);
	}
	
	return false;
}

void UI::UserInterface::MouseMoved(float X, float Y)
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
				m_RootWidget->MouseMoved(X, Y);
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
			
			m_CaptureWidget->MouseMoved(X - TopLeftCorner.m_V.m_A[0], Y - TopLeftCorner.m_V.m_A[1]);
		}
	}
}

void UI::UserInterface::Update(float RealTimeSeconds, float GameTimeSeconds)
{
	assert (m_RootWidget != 0);
	
	std::stack< std::pair< UI::Widget *, bool > > ToDo;
	
	ToDo.push(std::make_pair(m_RootWidget, false));
	while(ToDo.size() > 0)
	{
		std::pair< UI::Widget *, bool > & Widget(ToDo.top());
		
		if((Widget.first->m_SubWidgets.size() == 0) || (Widget.second == true))
		{
			Widget.first->_UpdatingEvent(RealTimeSeconds, GameTimeSeconds);
			ToDo.pop();
		}
		else
		{
			Widget.second = true;
			for(std::list< UI::Widget * >::reverse_iterator SubWidgetIterator = Widget.first->m_SubWidgets.rbegin(); SubWidgetIterator != Widget.first->m_SubWidgets.rend(); ++SubWidgetIterator)
			{
				ToDo.push(std::make_pair(*SubWidgetIterator, false));
			}
		}
	}
}

void UI::UserInterface::OnCaptureWidgetDestroying(void)
{
	m_CaptureWidget->DisconnectDestroyingCallback(m_CaptureWidgetDestroyingCallbackConnectionHandle);
	m_CaptureWidget = 0;
}

void UI::UserInterface::OnHoverWidgetDestroying(void)
{
	m_HoverWidget = 0;
}

void UI::UserInterface::OnRootWidgetDestroying(void)
{
	m_RootWidget = 0;
}
