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
		const Color * GetBackgroundColor(void) const;
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
		bool _AnchorBottom;
		bool _AnchorLeft;
		bool _AnchorRight;
		bool _AnchorTop;
		Color * _BackgroundColor;
		Color * _DisabledBackgroundColor;
		bool _Enabled;
		Widget * _HoverWidget;
		Widget * _KeyFocus;
		std::string m_Name;
		Vector2f m_Position;
		Vector2f m_Size;
		std::list< Widget * > _SubWidgets;
		Widget * _SupWidget;
		bool _Visible;
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
		auto SupWidget(this);
		
		while(SupWidget->_SupWidget != nullptr)
		{
			SupWidget = SupWidget->_SupWidget;
		}
		
		return SupWidget;
	}

	inline Widget * Widget::GetSupWidget(void)
	{
		return _SupWidget;
	}

	inline Widget * Widget::GetSubWidget(const std::string & Name)
	{
		for(auto Widget : _SubWidgets)
		{
			if(Widget->GetName() == Name)
			{
				return Widget;
			}
		}
		
		return nullptr;
	}

	inline const std::list< Widget * > & Widget::GetSubWidgets(void) const
	{
		return _SubWidgets;
	}

	inline Widget * Widget::GetKeyFocus(void)
	{
		return _KeyFocus;
	}

	inline bool Widget::IsVisible(void) const
	{
		return _Visible;
	}

	inline void Widget::SetAnchorBottom(bool AnchorBottom)
	{
		_AnchorBottom = AnchorBottom;
	}

	inline void Widget::SetAnchorLeft(bool AnchorLeft)
	{
		_AnchorLeft = AnchorLeft;
	}

	inline void Widget::SetAnchorRight(bool AnchorRight)
	{
		_AnchorRight = AnchorRight;
	}

	inline void Widget::SetAnchorTop(bool AnchorTop)
	{
		_AnchorTop = AnchorTop;
	}

	inline void Widget::SetEnabled(bool Enabled)
	{
		_Enabled = Enabled;
	}

	inline void Widget::SetVisible(bool Visible)
	{
		_Visible = Visible;
	}
}

#endif
