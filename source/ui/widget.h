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

namespace UI
{
	class Event;
	class KeyEvent;
	class MouseButtonEvent;
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
		void GrabKeyFocus(void);
		void SetName(const std::string & Name);
		// getters
		const Color * GetBackgroundColor(void) const;
		const Vector2f & GetPosition(void) const;
		Vector2f GetGlobalPosition(void) const;
		const Vector2f & GetSize(void) const;
		const std::string & GetName(void) const;
		std::string GetPath(void) const;
		Widget * GetRootWidget(void);
		Widget * GetSupWidget(void);
		Widget * GetSubWidget(const std::string & Name);
		const std::list< Widget * > & GetSubWidgets(void) const;
		bool IsVisible(void) const;
		// setters
		void SetAnchorBottom(bool AnchorBottom);
		void SetAnchorLeft(bool AnchorLeft);
		void SetAnchorRight(bool AnchorRight);
		void SetAnchorTop(bool AnchorTop);
		void SetEnabled(bool Enabled);
		void SetVisible(bool Visible);
		// receive input
		void MouseMoved(float X, float Y);
		// connect and disconnect events
		Connection ConnectDestroyingCallback(std::function< void (void) > Callback);
		Connection ConnectKeyCallback(std::function< void (UI::KeyEvent &) > Callback);
		Connection ConnectMouseButtonCallback(std::function< void (UI::MouseButtonEvent &) > Callback);
		Connection ConnectMouseEnterCallback(std::function< void (UI::Event &) > Callback);
		Connection ConnectMouseLeaveCallback(std::function< void (UI::Event &) > Callback);
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
		static void _PushClippingRectangle(const Vector2f & Position, const Vector2f & Size);
		static void _PopClippingRectangle(void);
		static void _DrawClippingRectangle(void);
	private:
		void _UnsetHoverWidget(void);
		void _SetHoverWidget(UI::Widget * HoverWidget);
		bool _AnchorBottom;
		bool _AnchorLeft;
		bool _AnchorRight;
		bool _AnchorTop;
		Color * _BackgroundColor;
		Color * _DisabledBackgroundColor;
		bool _Enabled;
		Widget * _HoverWidget;
		Widget * _KeyFocus;
		std::string _Name;
		Vector2f _Position;
		Vector2f _Size;
		std::list< Widget * > _SubWidgets;
		Widget * _SupWidget;
		bool _Visible;
		// events
		::Event< void > _DestroyingEvent;
		::Event< void, UI::KeyEvent & > _KeyEvent;
		::Event< void, UI::MouseButtonEvent & > _MouseButtonEvent;
		::Event< void, UI::Event & > _MouseEnterEvent;
		::Event< void, UI::Event & > _MouseLeaveEvent;
		::Event< void, float, float > _MouseMovedEvent;
		::Event< void > _PositionChangedEvent;
		::Event< void > _SizeChangedEvent;
		::Event< void, float, float > _UpdatingEvent;
		// static manager properties
		static std::list< Widget * > _DestroyedWidgets;
		static std::stack< std::pair< Vector2f, Vector2f > > _ClippingRectangles;
	};

	inline const Vector2f & Widget::GetPosition(void) const
	{
		return _Position;
	}

	inline const Vector2f & Widget::GetSize(void) const
	{
		return _Size;
	}

	inline const std::string & Widget::GetName(void) const
	{
		return _Name;
	}

	inline std::list< Widget * > & Widget::GetDestroyedWidgets(void)
	{
		return _DestroyedWidgets;
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

	inline void Widget::SetName(const std::string & Name)
	{
		_Name = Name;
	}

	inline void Widget::SetVisible(bool Visible)
	{
		_Visible = Visible;
	}
}

#endif
