#include <iostream>
#include <map>

#include <GL/gl.h>
#include <GL/glut.h>

#include "button.h"
#include "color.h"
#include "draw_text.h"
#include "globals.h"
#include "label.h"
#include "map_dialog.h"
#include "system.h"
#include "system_manager.h"

MapDialog::MapDialog(Widget * SupWidget, System * System) :
	Widget(SupWidget),
	m_System(System),
	m_Scale(5.0f),
	m_SelectedSystem(0)
{
	SetPosition(math3d::vector2f(70.0f, 500.0f));
	SetSize(math3d::vector2f(500.0f, 500.0f));
	SetBackgroundColor(Color(0.2f, 0.2f, 0.2f));
	AddMouseButtonListener(this);
	m_OKButton = new Button(this);
	m_OKButton->SetPosition(math3d::vector2f(390.0f, 470.0f));
	m_OKButton->SetSize(math3d::vector2f(100.0f, 20.0f));
	m_OKButton->SetBackgroundColor(Color(0.3f, 0.3f, 0.3f));
	m_OKButton->AddClickedListener(this);
	m_OKButtonLabel = new Label(m_OKButton, "OK");
	m_OKButtonLabel->SetPosition(math3d::vector2f(0.0f, 0.0f));
	m_OKButtonLabel->SetSize(m_OKButton->GetSize());
	m_OKButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	m_OKButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
}

void MapDialog::Draw(void) const
{
	Widget::Draw();
	
	math3d::vector2f Middle(GetSize() / 2);
	float SystemSize(5.0f);
	
	glPushMatrix();
	glTranslatef(Middle.m_V.m_A[0], Middle.m_V.m_A[1], 0.0f);
	glScalef(1.0f, -1.0f, 1.0f);
	
	const std::map< std::string, System * > & Systems(g_SystemManager.GetSystems());
	
	for(std::map< std::string, System * >::const_iterator SystemIterator = Systems.begin(); SystemIterator != Systems.end(); ++SystemIterator)
	{
		math3d::vector2f Position(SystemIterator->second->GetPosition() - m_System->GetPosition());
		
		Position *= m_Scale;
		glPushMatrix();
		glTranslatef(Position.m_V.m_A[0], Position.m_V.m_A[1], 0.0f);
		
		const std::list< System * > & LinkedSystems(SystemIterator->second->GetLinkedSystems());
		
		for(std::list< System * >::const_iterator LinkedSystemIterator = LinkedSystems.begin(); LinkedSystemIterator != LinkedSystems.end(); ++LinkedSystemIterator)
		{
			glColor3f(0.0f, 0.0f, 0.8f);
			glBegin(GL_LINES);
			glVertex2f(0.0f, 0.0f);
			glVertex2fv(((*LinkedSystemIterator)->GetPosition() * m_Scale).m_V.m_A);
			glEnd();
		}
		if((SystemIterator->second == m_SelectedSystem) || (SystemIterator->second == m_System))
		{
			glColor3f(0.0f, 1.0f, 0.0f);
		}
		else
		{
			glColor3f(0.0f, 0.0f, 0.8f);
		}
		glBegin(GL_LINE_LOOP);
		glVertex2f(SystemSize, 0.0f);
		glVertex2f(SystemSize * 0.866f, SystemSize * 0.5f);
		glVertex2f(SystemSize * 0.5f, SystemSize * 0.866f);
		glVertex2f(0.0f, SystemSize);
		glVertex2f(-SystemSize * 0.5f, SystemSize * 0.866f);
		glVertex2f(-SystemSize * 0.866f, SystemSize * 0.5f);
		glVertex2f(-SystemSize, 0.0f);
		glVertex2f(-SystemSize * 0.866f, -SystemSize * 0.5f);
		glVertex2f(-SystemSize * 0.5f, -SystemSize * 0.866f);
		glVertex2f(0.0f, -SystemSize);
		glVertex2f(SystemSize * 0.5f, -SystemSize * 0.866f);
		glVertex2f(SystemSize * 0.866f, -SystemSize * 0.5f);
		glEnd();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(0.0f, -12.0f - SystemSize * 2.0f, 0.0f);
		DrawTextWithoutTranslation(SystemIterator->second->GetName());
		glPopMatrix();
	}
	glPopMatrix();
}

bool MapDialog::OnClicked(Widget * EventSource)
{
	if(EventSource == m_OKButton)
	{
		Destroy();
		
		return true;
	}
	
	return false;
}

bool MapDialog::OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y)
{
	if(Button == GLUT_WHEEL_UP)
	{
		m_Scale *= 1.1f;
	}
	else if(Button == GLUT_WHEEL_DOWN)
	{
		m_Scale *= 0.9f;
	}
	else if(Button == GLUT_LEFT_BUTTON)
	{
		X -= GetSize().m_V.m_A[0] / 2;
		Y -= GetSize().m_V.m_A[1] / 2;
		
		const std::map< std::string, System * > & Systems(g_SystemManager.GetSystems());
		
		for(std::map< std::string, System * >::const_iterator SystemIterator = Systems.begin(); SystemIterator != Systems.end(); ++SystemIterator)
		{
			math3d::vector2f Position(SystemIterator->second->GetPosition() - m_System->GetPosition());
			
			Position *= m_Scale;
			Position.m_V.m_A[0] -= X;
			Position.m_V.m_A[1] += Y;
			if(Position.length_squared() < 40.0f)
			{
				m_SelectedSystem = SystemIterator->second;
				
				break;
			}
		}
	}
	
	return false;
}
