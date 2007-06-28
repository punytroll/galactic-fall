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

#include <algorithm>

#include <GL/gl.h>

#include "color.h"
#include "key_listener.h"
#include "mouse_button_listener.h"
#include "mouse_motion_listener.h"
#include "widget.h"

std::list< Widget * > Widget::m_DestroyedWidgets;

Widget::Widget(Widget * SupWidget, const std::string & Name) :
	m_Name(Name),
	m_SupWidget(0),
	m_HoverWidget(0),
	m_BackgroundColor(0),
	m_Position(true),
	m_Size(true),
	m_Visible(true),
	m_AnchorBottom(false),
	m_AnchorLeft(true),
	m_AnchorRight(false),
	m_AnchorTop(true),
	m_KeyFocus(0)
{
	if(SupWidget != 0)
	{
		SupWidget->AddSubWidget(this);
	}
}

Widget::~Widget(void)
{
	assert(m_SupWidget == 0);
	assert(m_SubWidgets.size() == 0);
	delete m_BackgroundColor;
}

void Widget::Hide(void)
{
	m_Visible = false;
}

void Widget::Show(void)
{
	m_Visible = true;
}

void Widget::Draw(void) const
{
	if(m_BackgroundColor != 0)
	{
		glColor4fv(m_BackgroundColor->GetColor().m_V.m_A);
		glBegin(GL_QUADS);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.0f, m_Size.m_V.m_A[1]);
		glVertex2f(m_Size.m_V.m_A[0], m_Size.m_V.m_A[1]);
		glVertex2f(m_Size.m_V.m_A[0], 0.0f);
		glEnd();
	}
	for(std::list< Widget * >::const_reverse_iterator SubWidgetIterator = m_SubWidgets.rbegin(); SubWidgetIterator != m_SubWidgets.rend(); ++SubWidgetIterator)
	{
		Widget * SubWidget(*SubWidgetIterator);
		
		if(SubWidget->IsVisible() == true)
		{
			glPushMatrix();
			glTranslatef(SubWidget->m_Position.m_V.m_A[0], SubWidget->m_Position.m_V.m_A[1], 0.0f);
			SubWidget->Draw();
			glPopMatrix();
		}
	}
}

Vector2f Widget::GetGlobalPosition(void) const
{
	const Widget * CurrentWidget(this);
	Vector2f Result(true);
	
	while(CurrentWidget != 0)
	{
		Result += CurrentWidget->GetPosition();
		CurrentWidget = CurrentWidget->m_SupWidget;
	}
	
	return Result;
}

void Widget::SetBackgroundColor(const Color & BackgroundColor)
{
	m_BackgroundColor = new Color(BackgroundColor);
}

void Widget::SetPosition(const Vector2f & Position)
{
	Vector2f Offset(GetPosition() - Position);
	
	m_Position = Position;
	
	std::list< Widget * >::iterator SubWidgetIterator(m_SubWidgets.begin());
	
	// iterate through the list of sub widgets and correct widget positions and sizes
	while(SubWidgetIterator != m_SubWidgets.end())
	{
		Widget * SubWidget(*SubWidgetIterator);
		Vector2f SubWidgetNewPosition(SubWidget->GetPosition());
		Vector2f SubWidgetNewSize(SubWidget->GetSize());
		
		if(SubWidget->GetAnchorLeft() == false)
		{
			SubWidgetNewPosition[0] += Offset[0];
		}
		if(SubWidget->GetAnchorTop() == false)
		{
			SubWidgetNewPosition[1] += Offset[1];
		}
		SubWidget->SetPosition(SubWidgetNewPosition);
		SubWidget->SetSize(SubWidgetNewSize);
		++SubWidgetIterator;
	}
}

void Widget::SetSize(const Vector2f & Size)
{
	Vector2f Offset(GetSize() - Size);
	
	m_Size = Size;
	
	std::list< Widget * >::iterator SubWidgetIterator(m_SubWidgets.begin());
	
	// iterate through the list of sub widgets and correct widget positions and sizes
	while(SubWidgetIterator != m_SubWidgets.end())
	{
		Widget * SubWidget(*SubWidgetIterator);
		Vector2f SubWidgetNewPosition(SubWidget->GetPosition());
		Vector2f SubWidgetNewSize(SubWidget->GetSize());
		
		if(SubWidget->GetAnchorRight() == true)
		{
			SubWidgetNewSize[0] -= Offset[0];
		}
		if(SubWidget->GetAnchorBottom() == true)
		{
			SubWidgetNewSize[1] -= Offset[1];
		}
		SubWidget->SetPosition(SubWidgetNewPosition);
		SubWidget->SetSize(SubWidgetNewSize);
		++SubWidgetIterator;
	}
}

void Widget::SetName(const std::string & Name)
{
	m_Name = Name;
}

void Widget::SetKeyFocus(Widget * KeyFocus)
{
	assert(find(m_SubWidgets.begin(), m_SubWidgets.end(), KeyFocus) != m_SubWidgets.end());
	assert(KeyFocus->m_SupWidget == this);
	m_KeyFocus = KeyFocus;
}

void Widget::GrabKeyFocus(void)
{
	if(m_SupWidget != 0)
	{
		m_SupWidget->m_KeyFocus = this;
		m_SupWidget->GrabKeyFocus();
	}
}

void Widget::AddSubWidget(Widget * SubWidget)
{
	assert(SubWidget->m_SupWidget == 0);
	SubWidget->m_SupWidget = this;
	m_SubWidgets.push_front(SubWidget);
}

void Widget::RemoveSubWidget(Widget * SubWidget)
{
	assert(SubWidget->m_SupWidget == this);
	SubWidget->m_SupWidget = 0;
	m_SubWidgets.erase(find(m_SubWidgets.begin(), m_SubWidgets.end(), SubWidget));
	if(SubWidget == m_KeyFocus)
	{
		m_KeyFocus = 0;
	}
}

void Widget::RaiseSubWidget(Widget * SubWidget)
{
	assert(SubWidget->m_SupWidget == this);
	m_SubWidgets.remove(SubWidget);
	m_SubWidgets.push_front(SubWidget);
}

void Widget::Destroy(void)
{
	// fire DestroyListeners before aything is actually destroyed
	for(std::list< DestroyListener * >::iterator DestroyListenerIterator = m_DestroyListeners.begin(); DestroyListenerIterator != m_DestroyListeners.end(); ++DestroyListenerIterator)
	{
		(*DestroyListenerIterator)->OnDestroy(this);
	}
	// now destroy
	while(m_SubWidgets.size() > 0)
	{
		m_SubWidgets.front()->Destroy();
	}
	if(m_SupWidget != 0)
	{
		m_SupWidget->RemoveSubWidget(this);
	}
	m_DestroyedWidgets.push_back(this);
}

bool Widget::Key(int Key, int State)
{
	if(m_KeyFocus != 0)
	{
		if(m_KeyFocus->Key(Key, State) == true)
		{
			return true;
		}
	}
	for(std::list< KeyListener * >::iterator KeyListenerIterator = m_KeyListeners.begin(); KeyListenerIterator != m_KeyListeners.end(); ++KeyListenerIterator)
	{
		if((*KeyListenerIterator)->OnKey(this, Key, State) == true)
		{
			return true;
		}
	}
	
	return false;
}

bool Widget::MouseButton(int Button, int State, float X, float Y)
{
	// iterate all sub widgets, look for an intersection and propagate the mouse event with corrected coordinates
	for(std::list< Widget * >::iterator SubWidgetIterator = m_SubWidgets.begin(); SubWidgetIterator != m_SubWidgets.end(); ++SubWidgetIterator)
	{
		const Vector2f & SubWidgetPosition((*SubWidgetIterator)->GetPosition());
		const Vector2f & SubWidgetSize((*SubWidgetIterator)->GetSize());
		
		if((X >= SubWidgetPosition.m_V.m_A[0]) && (X < SubWidgetPosition.m_V.m_A[0] + SubWidgetSize.m_V.m_A[0]) && (Y >= SubWidgetPosition.m_V.m_A[1]) && (Y < SubWidgetPosition.m_V.m_A[1] + SubWidgetSize.m_V.m_A[1]))
		{
			if((*SubWidgetIterator)->MouseButton(Button, State, X - SubWidgetPosition.m_V.m_A[0], Y - SubWidgetPosition.m_V.m_A[1]) == true)
			{
				return true;
			}
		}
	}
	// if no sub widget wanted the mouse event, propagate to our own listeners
	for(std::list< MouseButtonListener * >::iterator MouseButtonListenerIterator = m_MouseButtonListeners.begin(); MouseButtonListenerIterator != m_MouseButtonListeners.end(); ++MouseButtonListenerIterator)
	{
		if((*MouseButtonListenerIterator)->OnMouseButton(this, Button, State, X, Y) == true)
		{
			return true;
		}
	}
	
	return false;
}

void Widget::MouseMotion(float X, float Y)
{
	std::list< Widget * >::iterator SubWidgetIterator(m_SubWidgets.begin());
	
	// iterate all sub widgets, look for an intersection and propagate the mouse event with corrected coordinates
	while(SubWidgetIterator != m_SubWidgets.end())
	{
		const Vector2f & LeftTopCorner((*SubWidgetIterator)->GetPosition());
		Vector2f RightBottomCorner(LeftTopCorner + (*SubWidgetIterator)->GetSize());
		
		if((X >= LeftTopCorner.m_V.m_A[0]) && (X < RightBottomCorner.m_V.m_A[0]) && (Y >= LeftTopCorner.m_V.m_A[1]) && (Y < RightBottomCorner.m_V.m_A[1]))
		{
			if(m_HoverWidget != *SubWidgetIterator)
			{
				// save the HoverWidget so we can set it before calling MouseLeave
				Widget * HoverWidget(m_HoverWidget);
				
				m_HoverWidget = *SubWidgetIterator;
				m_HoverWidget->AddDestroyListener(this);
				if(HoverWidget != 0)
				{
					HoverWidget->MouseLeave();
					HoverWidget->RemoveDestroyListener(this);
				}
				m_HoverWidget->MouseEnter();
			}
			(*SubWidgetIterator)->MouseMotion(X - LeftTopCorner.m_V.m_A[0], Y - LeftTopCorner.m_V.m_A[1]);
			
			break;
		}
		++SubWidgetIterator;
	}
	// if all sub widget have been iterated through, the old hover widget is left
	if((SubWidgetIterator == m_SubWidgets.end()) && (m_HoverWidget != 0))
	{
		Widget * HoverWidget(m_HoverWidget);
		
		m_HoverWidget = 0;
		HoverWidget->MouseLeave();
		HoverWidget->RemoveDestroyListener(this);
	}
	// after this we may call all our listeners
	for(std::list< MouseMotionListener * >::iterator MouseMotionListenerIterator = m_MouseMotionListeners.begin(); MouseMotionListenerIterator != m_MouseMotionListeners.end(); ++MouseMotionListenerIterator)
	{
		(*MouseMotionListenerIterator)->OnMouseMotion(this, X, Y);
	}
}

void Widget::MouseEnter(void)
{
	for(std::list< MouseMotionListener * >::iterator MouseMotionListenerIterator = m_MouseMotionListeners.begin(); MouseMotionListenerIterator != m_MouseMotionListeners.end(); ++MouseMotionListenerIterator)
	{
		(*MouseMotionListenerIterator)->OnMouseEnter(this);
	}
}

void Widget::MouseLeave(void)
{
	if(m_HoverWidget != 0)
	{
		Widget * HoverWidget(m_HoverWidget);
		
		m_HoverWidget = 0;
		HoverWidget->MouseLeave();
		HoverWidget->RemoveDestroyListener(this);
	}
	for(std::list< MouseMotionListener * >::iterator MouseMotionListenerIterator = m_MouseMotionListeners.begin(); MouseMotionListenerIterator != m_MouseMotionListeners.end(); ++MouseMotionListenerIterator)
	{
		(*MouseMotionListenerIterator)->OnMouseLeave(this);
	}
}

void Widget::OnDestroy(Widget * EventSource)
{
	if(EventSource == m_KeyFocus)
	{
		m_KeyFocus = 0;
	}
	if(EventSource == m_HoverWidget)
	{
		m_HoverWidget = 0;
	}
}

void Widget::AddDestroyListener(DestroyListener * DestroyListener)
{
	m_DestroyListeners.push_back(DestroyListener);
}

void Widget::AddKeyListener(KeyListener * KeyListener)
{
	m_KeyListeners.push_back(KeyListener);
}

void Widget::AddMouseButtonListener(MouseButtonListener * MouseButtonListener)
{
	m_MouseButtonListeners.push_back(MouseButtonListener);
}

void Widget::AddMouseMotionListener(MouseMotionListener * MouseMotionListener)
{
	m_MouseMotionListeners.push_back(MouseMotionListener);
}

void Widget::RemoveDestroyListener(DestroyListener * DestroyListenerToRemove)
{
	for(std::list< DestroyListener * >::iterator DestroyListenerIterator = m_DestroyListeners.begin(); DestroyListenerIterator != m_DestroyListeners.end(); ++DestroyListenerIterator)
	{
		if(*DestroyListenerIterator == DestroyListenerToRemove)
		{
			m_DestroyListeners.erase(DestroyListenerIterator);
			
			return;
		}
	}
}
