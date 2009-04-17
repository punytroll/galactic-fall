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

#ifndef WIDGET_H
#define WIDGET_H

#include <iostream>
#include <list>
#include <stack>
#include <string>

#include "callbacks/events.h"
#include "math/vector2f.h"

class Color;
class DimensionListener;
class KeyEventInformation;
class KeyListener;
class MouseButtonListener;
class MouseMotionListener;
class UserInterface;

class Widget
{
public:
	friend class UserInterface;
	
	Widget(Widget * SupWidget = 0, const std::string & Name = "");
	virtual ~Widget(void);
	virtual void Draw(void) const;
	void SetBackgroundColor(const Color & BackgroundColor);
	void SetDisabledBackgroundColor(const Color & DisabledBackgroundColor);
	void UnsetBackgroundColor(void);
	void UnsetDisabledBackgroundColor(void);
	void SetPosition(const Vector2f & Position);
	void SetSize(const Vector2f & Size);
	void AddSubWidget(Widget * SubWidget);
	void RemoveSubWidget(Widget * SubWidget);
	void RaiseSubWidget(Widget * SubWidget);
	void Destroy(void);
	void SetKeyFocus(Widget * KeyFocus);
	void GrabKeyFocus(void);
	void SetName(const std::string & Name);
	// getters
	bool GetAnchorBottom(void) const;
	bool GetAnchorLeft(void) const;
	bool GetAnchorRight(void) const;
	bool GetAnchorTop(void) const;
	const Color * GetBackgroundColor(void) const;
	bool GetEnabled(void) const;
	const Vector2f & GetPosition(void) const;
	Vector2f GetGlobalPosition(void) const;
	const Vector2f & GetSize(void) const;
	const std::string & GetName(void) const;
	Widget * GetRootWidget(void);
	const Widget * GetRootWidget(void) const;
	Widget * GetSupWidget(void);
	const Widget * GetSupWidget(void) const;
	Widget * GetSubWidget(const std::string & Name);
	const Widget * GetSubWidget(const std::string & Name) const;
	const std::list< Widget * > & GetSubWidgets(void) const;
	Widget * GetKeyFocus(void);
	bool IsVisible(void) const;
	// setters
	void SetAnchorBottom(bool AnchorBottom);
	void SetAnchorLeft(bool AnchorLeft);
	void SetAnchorRight(bool AnchorRight);
	void SetAnchorTop(bool AnchorTop);
	void SetEnabled(bool Enabled);
	void SetVisible(bool Visible);
	// receive input
	bool MouseButton(int Button, int State, float X, float Y);
	bool Key(const KeyEventInformation & KeyEventInformation);
	void MouseMotion(float X, float Y);
	// MouseEnter may depend on the fact that m_HoverWidget on the m_SupWidget is set to this
	// MouseEnter on the new hover widget is called after MouseLeave on the old hover widget
	void MouseEnter(void);
	// MouseLeave may depend on the fact that m_HoverWidget on the m_SupWidget is still set to this
	// MouseLeave on the old hover widget is called before MouseEnter on the new hover widget
	void MouseLeave(void);
	// add signal listeners
	void AddDimensionListener(DimensionListener * DimensionListener);
	void AddKeyListener(KeyListener * KeyListener);
	void AddMouseButtonListener(MouseButtonListener * MouseButtonListener);
	void AddMouseMotionListener(MouseMotionListener * MouseMotionListener);
	// remove signal listeners
	void RemoveDimensionListener(DimensionListener * DimensionListener);
	// events
	ConnectionHandle ConnectDestroyCallback(Callback0< void > DestroyCallback);
	void DisconnectDestroyCallback(ConnectionHandle & ConnectionHandle);
	// static manager functions
	static std::list< Widget * > & GetDestroyedWidgets(void);
protected:
	static void PushClippingRectangle(const Vector2f & Position, const Vector2f & Size);
	static void PopClippingRectangle(void);
	static void DrawClippingRectangle(void);
private:
	bool m_Enabled;
	Color * m_DisabledBackgroundColor;
	std::string m_Name;
	Widget * m_SupWidget;
	Widget * m_HoverWidget;
	Color * m_BackgroundColor;
	Vector2f m_Position;
	Vector2f m_Size;
	std::list< Widget * > m_SubWidgets;
	bool m_Visible;
	bool m_AnchorBottom;
	bool m_AnchorLeft;
	bool m_AnchorRight;
	bool m_AnchorTop;
	Widget * m_KeyFocus;
	// events
	Event0< void > _DestroyEvent;
	// listeners
	std::list< DimensionListener * > m_DimensionListeners;
	std::list< KeyListener * > m_KeyListeners;
	std::list< MouseButtonListener * > m_MouseButtonListeners;
	std::list< MouseMotionListener * > m_MouseMotionListeners;
	// static manager properties
	static std::list< Widget * > m_DestroyedWidgets;
	static std::stack< std::pair< Vector2f, Vector2f > > m_ClippingRectangles;
};

inline bool Widget::GetAnchorBottom(void) const
{
	return m_AnchorBottom;
}

inline bool Widget::GetAnchorLeft(void) const
{
	return m_AnchorLeft;
}

inline bool Widget::GetAnchorRight(void) const
{
	return m_AnchorRight;
}

inline bool Widget::GetAnchorTop(void) const
{
	return m_AnchorTop;
}

inline const Color * Widget::GetBackgroundColor(void) const
{
	return m_BackgroundColor;
}

inline bool Widget::GetEnabled(void) const
{
	return m_Enabled;
}

inline const Vector2f & Widget::GetPosition(void) const
{
	return m_Position;
}

inline const Vector2f & Widget::GetSize(void) const
{
	return m_Size;
}

inline const std::string & Widget::GetName(void) const
{
	return m_Name;
}

inline std::list< Widget * > & Widget::GetDestroyedWidgets(void)
{
	return m_DestroyedWidgets;
}

inline Widget * Widget::GetRootWidget(void)
{
	Widget * SupWidget(this);
	
	while(SupWidget->m_SupWidget != 0)
	{
		SupWidget = SupWidget->m_SupWidget;
	}
	
	return SupWidget;
}

inline const Widget * Widget::GetRootWidget(void) const
{
	const Widget * SupWidget(this);
	
	while(SupWidget->m_SupWidget != 0)
	{
		SupWidget = SupWidget->m_SupWidget;
	}
	
	return SupWidget;
}

inline Widget * Widget::GetSupWidget(void)
{
	return m_SupWidget;
}

inline const Widget * Widget::GetSupWidget(void) const
{
	return m_SupWidget;
}

inline Widget * Widget::GetSubWidget(const std::string & Name)
{
	for(std::list< Widget * >::iterator Iterator = m_SubWidgets.begin(); Iterator != m_SubWidgets.end(); ++Iterator)
	{
		if((*Iterator)->GetName() == Name)
		{
			return *Iterator;
		}
	}
	
	return 0;
}

inline const Widget * Widget::GetSubWidget(const std::string & Name) const
{
	for(std::list< Widget * >::const_iterator Iterator = m_SubWidgets.begin(); Iterator != m_SubWidgets.end(); ++Iterator)
	{
		if((*Iterator)->GetName() == Name)
		{
			return *Iterator;
		}
	}
	
	return 0;
}

inline const std::list< Widget * > & Widget::GetSubWidgets(void) const
{
	return m_SubWidgets;
}

inline Widget * Widget::GetKeyFocus(void)
{
	return m_KeyFocus;
}

inline bool Widget::IsVisible(void) const
{
	return m_Visible;
}

inline void Widget::SetAnchorBottom(bool AnchorBottom)
{
	m_AnchorBottom = AnchorBottom;
}

inline void Widget::SetAnchorLeft(bool AnchorLeft)
{
	m_AnchorLeft = AnchorLeft;
}

inline void Widget::SetAnchorRight(bool AnchorRight)
{
	m_AnchorRight = AnchorRight;
}

inline void Widget::SetAnchorTop(bool AnchorTop)
{
	m_AnchorTop = AnchorTop;
}

inline void Widget::SetEnabled(bool Enabled)
{
	m_Enabled = Enabled;
}

inline void Widget::SetVisible(bool Visible)
{
	m_Visible = Visible;
}

#endif
