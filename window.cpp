#include "globals.h"
#include "label.h"
#include "user_interface.h"
#include "window.h"

WWindow::WWindow(Widget * SupWidget, const std::string & Title) :
	Widget(SupWidget)
{
	SetBackgroundColor(Color(0.2f, 0.2f, 0.2f));
	m_TitleLabel = new Label(this, Title);
	m_TitleLabel->SetPosition(math3d::vector2f(10.0f, 10.0f));
	m_TitleLabel->SetSize(math3d::vector2f(480.0f, 20.0f));
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
		if(Button == 1)
		{
			if(State == EV_DOWN)
			{
				m_GrabPosition.set(X, Y);
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
		SetPosition(GetPosition() + math3d::vector2f(X, Y) - m_GrabPosition);
	}
}
