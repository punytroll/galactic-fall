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

#include "../callbacks/callbacks.h"
#include "../globals.h"
#include "border.h"
#include "label.h"
#include "user_interface.h"
#include "window.h"

UI::Window::Window(Widget * SupWidget, const std::string & Title) :
	Widget(SupWidget)
{
	SetBackgroundColor(Color(0.2f, 0.2f, 0.2f, 1.0f));
	m_Border = new UI::Border(this);
	m_Border->SetPosition(Vector2f(0.0f, 0.0f));
	m_Border->SetSize(GetSize());
	m_Border->SetAnchorBottom(true);
	m_Border->SetAnchorRight(true);
	m_Border->SetWidth(1.0f);
	m_Border->SetForegroundColor(Color(0.4f, 0.4f, 0.4f, 1.0f));
	m_TitleLabel = new UI::Label(this, Title);
	m_TitleLabel->SetPosition(Vector2f(10.0f, 10.0f));
	m_TitleLabel->SetSize(Vector2f(GetSize()[0] - 20.0f, 20.0f));
	m_TitleLabel->SetAnchorRight(true);
	m_TitleLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	m_TitleLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	m_TitleLabel->SetBackgroundColor(Color(0.2f, 0.2f, 0.4f, 1.0f));
	m_TitleLabel->ConnectMouseButtonCallback(Callback(this, &UI::Window::OnTitleLabelMouseButton));
	m_TitleLabel->ConnectMouseMovedCallback(Callback(this, &UI::Window::OnTitleLabelMouseMoved));
	m_ResizeDragBox = new Widget(this);
	m_ResizeDragBox->SetPosition(Vector2f(GetSize()[0] - 9.0f, GetSize()[1] - 9.0f));
	m_ResizeDragBox->SetSize(Vector2f(7.0f, 7.0f));
	m_ResizeDragBox->SetAnchorBottom(true);
	m_ResizeDragBox->SetAnchorLeft(false);
	m_ResizeDragBox->SetAnchorRight(true);
	m_ResizeDragBox->SetAnchorTop(false);
	m_ResizeDragBox->SetBackgroundColor(Color(0.2f, 0.2f, 0.4f, 1.0f));
	m_ResizeDragBox->ConnectMouseButtonCallback(Callback(this, &UI::Window::OnResizeDragBoxMouseButton));
	m_ResizeDragBox->ConnectMouseMovedCallback(Callback(this, &UI::Window::OnResizeDragBoxMouseMoved));
}

bool UI::Window::OnTitleLabelMouseButton(int Button, int State, float X, float Y)
{
	GetSupWidget()->RaiseSubWidget(this);
	if(Button == 1)
	{
		if(State == EV_DOWN)
		{
			m_GrabPosition.Set(X, Y);
			g_UserInterface->SetCaptureWidget(m_TitleLabel);
		}
		else
		{
			g_UserInterface->ReleaseCaptureWidget();
		}
		
		return true;
	}
	
	return false;
}

bool UI::Window::OnResizeDragBoxMouseButton(int Button, int State, float X, float Y)
{
	GetSupWidget()->RaiseSubWidget(this);
	if(Button == 1)
	{
		if(State == EV_DOWN)
		{
			m_GrabPosition.Set(X, Y);
			g_UserInterface->SetCaptureWidget(m_ResizeDragBox);
		}
		else
		{
			g_UserInterface->ReleaseCaptureWidget();
		}
		
		return true;
	}
	
	return false;
}

void UI::Window::OnTitleLabelMouseMoved(float X, float Y)
{
	if(g_UserInterface->GetCaptureWidget() == m_TitleLabel)
	{
		SetPosition(GetPosition() + Vector2f(X, Y) - m_GrabPosition);
	}
}

void UI::Window::OnResizeDragBoxMouseMoved(float X, float Y)
{
	if(g_UserInterface->GetCaptureWidget() == m_ResizeDragBox)
	{
		SetSize(GetSize() + Vector2f(X, Y) - m_GrabPosition);
	}
}
