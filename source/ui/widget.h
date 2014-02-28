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

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <iostream>
#include <list>
#include <stack>
#include <string>

#include <algebra/vector2f.h>

#include "../events.h"

class Color;
class KeyEventInformation;

namespace UI
{
	class UserInterface;
	
	class Widget
	{
	public:
		friend class UI::UserInterface;
		
		Widget(Widget * SupWidget = 0, const std::string & Name = "");
		virtual ~Widget(void);
		virtual void Draw(void);
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
		Widget * GetSupWidget(void);
		Widget * GetSubWidget(const std::string & Name);
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
		void MouseMoved(float X, float Y);
		// MouseEnter may depend on the fact that m_HoverWidget on the m_SupWidget is set to this
		// MouseEnter on the new hover widget is called after MouseLeave on the old hover widget
		void MouseEnter(void);
		// MouseLeave may depend on the fact that m_HoverWidget on the m_SupWidget is still set to this
		// MouseLeave on the old hover widget is called before MouseEnter on the new hover widget
		void MouseLeave(void);
		// connect and disconnect events
		Connection ConnectDestroyingCallback(std::function< void (void) > Callback);
		Connection ConnectKeyCallback(std::function< bool (const KeyEventInformation &) > Callback);
		Connection ConnectMouseButtonCallback(std::function< bool (int, int, float, float) > Callback);
		Connection ConnectMouseEnterCallback(std::function< void (void) > Callback);
		Connection ConnectMouseLeaveCallback(std::function< void (void) > Callback);
		Connection ConnectMouseMovedCallback(std::function< void (float, float) > Callback);
		Connection ConnectPositionChangedCallback(std::function< void (void) > Callback);
		Connection ConnectSizeChangedCallback(std::function< void (void) > Callback);
		Connection ConnectUpdatingCallback(std::function< void (float, float) > Callback);
		void DisconnectDestroyingCallback(Connection & Connection);
		void DisconnectKeyCallback(Connection & Connection);
		void DisconnectMouseButtonCallback(Connection & Connection);
		void DisconnectMouseEnterCallback(Connection & Connection);
		void DisconnectMouseLeaveCallback(Connection & Connection);
		void DisconnectMouseMovedCallback(Connection & Connection);
		void DisconnectPositionChangedCallback(Connection & Connection);
		void DisconnectSizeChangedCallback(Connection & Connection);
		void DisconnectUpdatingCallback(Connection & Connection);
		// static manager functions
		static std::list< Widget * > & GetDestroyedWidgets(void);
	protected:
		static void PushClippingRectangle(const Vector2f & Position, const Vector2f & Size);
		static void PopClippingRectangle(void);
		static void DrawClippingRectangle(void);
	private:
		Color * _BackgroundColor;
		Color * _DisabledBackgroundColor;
		bool _Enabled;
		std::string m_Name;
		Widget * m_SupWidget;
		Widget * m_HoverWidget;
		Vector2f m_Position;
		Vector2f m_Size;
		std::list< Widget * > _SubWidgets;
		bool m_Visible;
		bool m_AnchorBottom;
		bool m_AnchorLeft;
		bool m_AnchorRight;
		bool m_AnchorTop;
		Widget * m_KeyFocus;
		// events
		Event< void > _DestroyingEvent;
		Event< bool, const KeyEventInformation & > _KeyEvent;
		Event< bool, int, int, float, float > _MouseButtonEvent;
		Event< void > _MouseEnterEvent;
		Event< void > _MouseLeaveEvent;
		Event< void, float, float > _MouseMovedEvent;
		Event< void > _PositionChangedEvent;
		Event< void > _SizeChangedEvent;
		Event< void, float, float > _UpdatingEvent;
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

	inline bool Widget::GetEnabled(void) const
	{
		return _Enabled;
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

	inline Widget * Widget::GetSupWidget(void)
	{
		return m_SupWidget;
	}

	inline Widget * Widget::GetSubWidget(const std::string & Name)
	{
		for(std::list< Widget * >::iterator Iterator = _SubWidgets.begin(); Iterator != _SubWidgets.end(); ++Iterator)
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
		return _SubWidgets;
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
		_Enabled = Enabled;
	}

	inline void Widget::SetVisible(bool Visible)
	{
		m_Visible = Visible;
	}
}

#endif
