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

#include "callbacks/callbacks.h"
#include "color.h"
#include "math.h"
#include "mouse_button_listener.h"
#include "mouse_motion_listener.h"
#include "widget.h"

std::list< Widget * > Widget::m_DestroyedWidgets;
std::stack< std::pair< Vector2f, Vector2f > > Widget::m_ClippingRectangles;

Widget::Widget(Widget * SupWidget, const std::string & Name) :
	m_Enabled(true),
	m_DisabledBackgroundColor(0),
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
	m_BackgroundColor = 0;
	delete m_DisabledBackgroundColor;
	m_DisabledBackgroundColor = 0;
}

void Widget::Draw(void) const
{
	Color * Color((GetEnabled() == true) ? (m_BackgroundColor) : (m_DisabledBackgroundColor));
	
	if(Color != 0)
	{
		glColor4fv(Color->GetColor().m_V.m_A);
		glBegin(GL_QUADS);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.0f, m_Size.m_V.m_A[1]);
		glVertex2f(m_Size.m_V.m_A[0], m_Size.m_V.m_A[1]);
		glVertex2f(m_Size.m_V.m_A[0], 0.0f);
		glEnd();
	}
	if(m_SubWidgets.empty() == false)
	{
		for(std::list< Widget * >::const_reverse_iterator SubWidgetIterator = m_SubWidgets.rbegin(); SubWidgetIterator != m_SubWidgets.rend(); ++SubWidgetIterator)
		{
			Widget * SubWidget(*SubWidgetIterator);
			
			if(SubWidget->IsVisible() == true)
			{
				glPushMatrix();
				PushClippingRectangle(SubWidget->GetPosition(), SubWidget->GetSize());
				glTranslatef(SubWidget->m_Position.m_V.m_A[0], SubWidget->m_Position.m_V.m_A[1], 0.0f);
				DrawClippingRectangle();
				SubWidget->Draw();
				PopClippingRectangle();
				glPopMatrix();
			}
		}
		// restore the clipping rectangle for this widget
		DrawClippingRectangle();
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
	delete m_BackgroundColor;
	m_BackgroundColor = new Color(BackgroundColor);
}

void Widget::SetDisabledBackgroundColor(const Color & DisabledBackgroundColor)
{
	delete m_DisabledBackgroundColor;
	m_DisabledBackgroundColor = new Color(DisabledBackgroundColor);
}

void Widget::UnsetBackgroundColor(void)
{
	delete m_BackgroundColor;
	m_BackgroundColor = 0;
}

void Widget::UnsetDisabledBackgroundColor(void)
{
	delete m_DisabledBackgroundColor;
	m_DisabledBackgroundColor = 0;
}

void Widget::SetPosition(const Vector2f & Position)
{
	m_Position = Position;
	_PositionChangedEvent();
}

void Widget::SetSize(const Vector2f & Size)
{
	// early bailing out if this is a no-op
	if(Size == m_Size)
	{
		return;
	}
	
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
			if(SubWidget->GetAnchorLeft() == true)
			{
				SubWidgetNewSize[0] -= Offset[0];
			}
			else
			{
				SubWidgetNewPosition[0] -= Offset[0];
			}
		}
		if(SubWidget->GetAnchorBottom() == true)
		{
			if(SubWidget->GetAnchorTop() == true)
			{
				SubWidgetNewSize[1] -= Offset[1];
			}
			else
			{
				SubWidgetNewPosition[1] -= Offset[1];
			}
		}
		SubWidget->SetPosition(SubWidgetNewPosition);
		SubWidget->SetSize(SubWidgetNewSize);
		++SubWidgetIterator;
	}
	_SizeChangedEvent();
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
	if(SubWidget == m_HoverWidget)
	{
		m_HoverWidget = 0;
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
	// fire Destroy event before aything is actually destroyed
	_DestroyEvent();
	// now destroy
	// first the sub widgets, they will remove themselves from this widget
	while(m_SubWidgets.size() > 0)
	{
		m_SubWidgets.front()->Destroy();
	}
	// now remove ourself from the sup widget
	if(m_SupWidget != 0)
	{
		m_SupWidget->RemoveSubWidget(this);
	}
	// now append ourself to the list of destroyed widgets to be delete'd by the garbage collection
	m_DestroyedWidgets.push_back(this);
}

bool Widget::Key(const KeyEventInformation & TheKeyEventInformation)
{
	if((m_KeyFocus != 0) && (m_KeyFocus->GetEnabled() == true))
	{
		if(m_KeyFocus->Key(TheKeyEventInformation) == true)
		{
			return true;
		}
	}
	
	bool Result(false);
	
	for(Event1< bool, const KeyEventInformation & >::CallbackIterator CallbackIterator = _KeyEvent.GetCallbackIterator(); CallbackIterator.IsValid() == true; ++CallbackIterator)
	{
		if(CallbackIterator(TheKeyEventInformation) == true)
		{
			Result = true;
		}
	}
	
	return Result;
}

bool Widget::MouseButton(int Button, int State, float X, float Y)
{
	// iterate all sub widgets, look for an intersection and propagate the mouse event with corrected coordinates
	for(std::list< Widget * >::iterator SubWidgetIterator = m_SubWidgets.begin(); SubWidgetIterator != m_SubWidgets.end(); ++SubWidgetIterator)
	{
		const Vector2f & SubWidgetPosition((*SubWidgetIterator)->GetPosition());
		const Vector2f & SubWidgetSize((*SubWidgetIterator)->GetSize());
		
		if(((*SubWidgetIterator)->IsVisible() == true) && ((*SubWidgetIterator)->GetEnabled() == true) && (X >= SubWidgetPosition.m_V.m_A[0]) && (X < SubWidgetPosition.m_V.m_A[0] + SubWidgetSize.m_V.m_A[0]) && (Y >= SubWidgetPosition.m_V.m_A[1]) && (Y < SubWidgetPosition.m_V.m_A[1] + SubWidgetSize.m_V.m_A[1]))
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
		
		if(((*SubWidgetIterator)->IsVisible() == true) && ((*SubWidgetIterator)->GetEnabled() == true) && (X >= LeftTopCorner.m_V.m_A[0]) && (X < RightBottomCorner.m_V.m_A[0]) && (Y >= LeftTopCorner.m_V.m_A[1]) && (Y < RightBottomCorner.m_V.m_A[1]))
		{
			// test whether the new hover widget equals the old
			if(m_HoverWidget != *SubWidgetIterator)
			{
				// befor unsetting the hover widget inform the old hover widget that the mouse cursor is leaving
				if(m_HoverWidget != 0)
				{
					m_HoverWidget->MouseLeave();
				}
				// now set the new hover widget
				m_HoverWidget = *SubWidgetIterator;
				// inform the new hover widget about the entering of the mouse cursor
				m_HoverWidget->MouseEnter();
			}
			(*SubWidgetIterator)->MouseMotion(X - LeftTopCorner.m_V.m_A[0], Y - LeftTopCorner.m_V.m_A[1]);
			
			break;
		}
		++SubWidgetIterator;
	}
	// if all sub widget have been iterated through, the old hover widget has been left by the mouse cursor
	if((SubWidgetIterator == m_SubWidgets.end()) && (m_HoverWidget != 0))
	{
		m_HoverWidget->MouseLeave();
		m_HoverWidget = 0;
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
	}
	for(std::list< MouseMotionListener * >::iterator MouseMotionListenerIterator = m_MouseMotionListeners.begin(); MouseMotionListenerIterator != m_MouseMotionListeners.end(); ++MouseMotionListenerIterator)
	{
		(*MouseMotionListenerIterator)->OnMouseLeave(this);
	}
}

ConnectionHandle Widget::ConnectDestroyCallback(Callback0< void > Callback)
{
	return _DestroyEvent.Connect(Callback);
}

ConnectionHandle Widget::ConnectKeyCallback(Callback1< bool, const KeyEventInformation & > Callback)
{
	return _KeyEvent.Connect(Callback);
}

ConnectionHandle Widget::ConnectPositionChangedCallback(Callback0< void > Callback)
{
	return _PositionChangedEvent.Connect(Callback);
}

ConnectionHandle Widget::ConnectSizeChangedCallback(Callback0< void > Callback)
{
	return _SizeChangedEvent.Connect(Callback);
}

void Widget::DisconnectDestroyCallback(ConnectionHandle & ConnectionHandle)
{
	_DestroyEvent.Disconnect(ConnectionHandle);
}

void Widget::DisconnectKeyCallback(ConnectionHandle & ConnectionHandle)
{
	_KeyEvent.Disconnect(ConnectionHandle);
}

void Widget::DisconnectPositionChangedCallback(ConnectionHandle & ConnectionHandle)
{
	_PositionChangedEvent.Disconnect(ConnectionHandle);
}

void Widget::DisconnectSizeChangedCallback(ConnectionHandle & ConnectionHandle)
{
	_SizeChangedEvent.Disconnect(ConnectionHandle);
}

void Widget::AddMouseButtonListener(MouseButtonListener * MouseButtonListener)
{
	m_MouseButtonListeners.push_back(MouseButtonListener);
}

void Widget::AddMouseMotionListener(MouseMotionListener * MouseMotionListener)
{
	m_MouseMotionListeners.push_back(MouseMotionListener);
}

void Widget::PushClippingRectangle(const Vector2f & Position, const Vector2f & Size)
{
	Vector2f LeftTop(true);
	Vector2f RightBottom(Size);
	
	if(m_ClippingRectangles.empty() == false)
	{
		LeftTop[0] = std::max(0.0f, m_ClippingRectangles.top().first[0] - Position[0]);
		LeftTop[1] = std::max(0.0f, m_ClippingRectangles.top().first[1] - Position[1]);
		RightBottom[0] = std::min(RightBottom[0], m_ClippingRectangles.top().second[0] - Position[0]);
		RightBottom[1] = std::min(RightBottom[1], m_ClippingRectangles.top().second[1] - Position[1]);
	}
	m_ClippingRectangles.push(std::make_pair(LeftTop, RightBottom));
}

void Widget::PopClippingRectangle(void)
{
	m_ClippingRectangles.pop();
}

void Widget::DrawClippingRectangle(void)
{
	double LeftPlane[4] = { 1.0, 0.0, 0.0, -m_ClippingRectangles.top().first[0] };
	double TopPlane[4] = { 0.0, 1.0, 0.0, -m_ClippingRectangles.top().first[1] };
	double RightPlane[4] = { -1.0, 0.0, 0.0, m_ClippingRectangles.top().second[0] };
	double BottomPlane[4] = { 0.0, -1.0, 0.0, m_ClippingRectangles.top().second[1] };
	
	glClipPlane(GL_CLIP_PLANE0, LeftPlane);
	glClipPlane(GL_CLIP_PLANE1, TopPlane);
	glClipPlane(GL_CLIP_PLANE2, RightPlane);
	glClipPlane(GL_CLIP_PLANE3, BottomPlane);
}
