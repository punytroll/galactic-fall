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

#include "globals.h"
#include "label.h"
#include "user_interface.h"
#include "window.h"

WWindow::WWindow(Widget * SupWidget, const std::string & Title) :
	Widget(SupWidget)
{
	SetBackgroundColor(Color(0.2f, 0.2f, 0.2f));
	m_TitleLabel = new Label(this, Title);
	m_TitleLabel->SetPosition(Vector2f(10.0f, 10.0f));
	m_TitleLabel->SetSize(Vector2f(480.0f, 20.0f));
	m_TitleLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	m_TitleLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_TitleLabel->SetBackgroundColor(Color(0.2f, 0.2f, 0.4f));
	m_TitleLabel->AddMouseButtonListener(this);
	m_TitleLabel->AddMouseMotionListener(this);
}

bool WWindow::OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y)
{
	if(EventSource == m_TitleLabel)
	{
		GetSupWidget()->RaiseSubWidget(this);
		if(Button == 1)
		{
			if(State == EV_DOWN)
			{
				m_GrabPosition.Set(X, Y);
				g_UserInterface.SetCaptureWidget(m_TitleLabel);
			}
			else
			{
				g_UserInterface.ReleaseCaptureWidget();
			}
			
			return true;
		}
	}
	
	return false;
}

void WWindow::OnMouseMotion(Widget * EventSource, float X, float Y)
{
	if((EventSource == m_TitleLabel) && (g_UserInterface.GetCaptureWidget() == m_TitleLabel))
	{
		SetPosition(GetPosition() + Vector2f(X, Y) - m_GrabPosition);
	}
}
