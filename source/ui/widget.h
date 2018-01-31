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

#include <events/event.h>

#include "dimension.h"

namespace Graphics
{
	class ColorRGBO;
	class RenderContext;
}

namespace UI
{
	class Event;
	class KeyEvent;
	class MouseButtonEvent;
	class MouseMoveEvent;
	class SubWidgetEvent;
	class UserInterface;
	
	class Widget
	{
	public:
		friend class UI::UserInterface;
		
		Widget(Widget * SupWidget = 0, const std::string & Name = "");
		virtual ~Widget(void);
		virtual void Draw(Graphics::RenderContext * RenderContext);
		void SetBackgroundColor(const Graphics::ColorRGBO & BackgroundColor);
		void SetDisabledBackgroundColor(const Graphics::ColorRGBO & DisabledBackgroundColor);
		void UnsetBackgroundColor(void);
		void UnsetDisabledBackgroundColor(void);
		void AddSubWidget(Widget * SubWidget);
		void RemoveSubWidget(Widget * SubWidget);
		void RaiseSubWidget(Widget * SubWidget);
		void LowerSubWidget(Widget * SubWidget);
		void Destroy(void);
		void GrabKeyFocus(void);
		void SetName(const std::string & Name);
		// getters
		const Graphics::ColorRGBO * GetBackgroundColor(void) const;
		float GetBottom(void);
		bool GetEnabled(void) const;
		float GetHeight(void);
		Widget * GetHoveredWidget(void);
		Vector2f GetGlobalPosition(void);
		float GetLeft(void);
		const std::string & GetName(void) const;
		std::string GetPath(void) const;
		float GetRight(void);
		Widget * GetRootWidget(void);
		Widget * GetSupWidget(void);
		Widget * GetSubWidget(const std::string & Name);
		const std::list< Widget * > & GetSubWidgets(void) const;
		float GetTop(void);
		float GetWidth(void);
		bool IsEnabled(void) const;
		bool IsHovered(void) const;
		bool IsVisible(void) const;
		// setters
		void SetAnchorBottom(bool AnchorBottom);
		void SetAnchorLeft(bool AnchorLeft);
		void SetAnchorRight(bool AnchorRight);
		void SetAnchorTop(bool AnchorTop);
		void SetEnabled(bool Enabled);
		void SetHeight(Expressions::Expression && Height);
		void SetLeft(Expressions::Expression && Left);
		void SetTop(Expressions::Expression && Top);
		void SetVisible(bool Visible);
		void SetWidth(Expressions::Expression && Width);
		// connect events
		Connection ConnectDestroyingCallback(std::function< void (UI::Event &) > Callback);
		Connection ConnectHeightChangedCallback(std::function< void (UI::Event &) > Callback);
		Connection ConnectKeyCallback(std::function< void (UI::KeyEvent &) > Callback);
		Connection ConnectLeftChangedCallback(std::function< void (UI::Event &) > Callback);
		Connection ConnectMouseButtonCallback(std::function< void (UI::MouseButtonEvent &) > Callback);
		Connection ConnectMouseEnterCallback(std::function< void (UI::Event &) > Callback);
		Connection ConnectMouseLeaveCallback(std::function< void (UI::Event &) > Callback);
		Connection ConnectMouseMoveCallback(std::function< void (UI::MouseMoveEvent &) > Callback);
		Connection ConnectSubWidgetAddedCallback(std::function< void (UI::SubWidgetEvent &) > Callback);
		Connection ConnectSubWidgetRemovedCallback(std::function< void (UI::SubWidgetEvent &) > Callback);
		Connection ConnectTopChangedCallback(std::function< void (UI::Event &) > Callback);
		Connection ConnectUpdatingCallback(std::function< void (float, float) > Callback);
		Connection ConnectWidthChangedCallback(std::function< void (UI::Event &) > Callback);
		// static manager functions
		static std::list< Widget * > & GetDestroyedWidgets(void);
	protected:
		static void _PopClippingRectangle(Graphics::RenderContext * RenderContext);
		static void _PushClippingRectangle(Graphics::RenderContext * RenderContext, float Left, float Top, float Bottom, float Right);
	private:
		void _UnsetHoverWidget(void);
		void _SetHoverWidget(UI::Widget * HoverWidget);
		bool _AnchorBottom;
		bool _AnchorLeft;
		bool _AnchorRight;
		bool _AnchorTop;
		Graphics::ColorRGBO * _BackgroundColor;
		Graphics::ColorRGBO * _DisabledBackgroundColor;
		bool _Enabled;
		UI::Dimension _Height;
		Widget * _HoverWidget;
		Widget * _KeyFocus;
		UI::Dimension _Left;
		std::string _Name;
		std::list< Widget * > _SubWidgets;
		Widget * _SupWidget;
		UI::Dimension _Top;
		bool _Visible;
		UI::Dimension _Width;
		// events
		::Event< void, UI::Event & > _DestroyingEvent;
		::Event< void, UI::Event & > _HeightChangedEvent;
		::Event< void, UI::KeyEvent & > _KeyEvent;
		::Event< void, UI::Event & > _LeftChangedEvent;
		::Event< void, UI::MouseButtonEvent & > _MouseButtonEvent;
		::Event< void, UI::Event & > _MouseEnterEvent;
		::Event< void, UI::Event & > _MouseLeaveEvent;
		::Event< void, UI::MouseMoveEvent & > _MouseMoveEvent;
		::Event< void, UI::SubWidgetEvent & > _SubWidgetAddedEvent;
		::Event< void, UI::SubWidgetEvent & > _SubWidgetRemovedEvent;
		::Event< void, UI::Event & > _TopChangedEvent;
		::Event< void, float, float > _UpdatingEvent;
		::Event< void, UI::Event & > _WidthChangedEvent;
		// static manager properties
		static std::list< Widget * > _DestroyedWidgets;
		static std::stack< std::tuple< float, float, float, float > > _ClippingRectangles;
	};
	
	inline float Widget::GetBottom(void)
	{
		return _Top.GetValue() + _Height.GetValue();
	}
	
	inline bool Widget::GetEnabled(void) const
	{
		return _Enabled;
	}
	
	inline float Widget::GetHeight(void)
	{
		return _Height.GetValue();
	}
	
	inline Widget * Widget::GetHoveredWidget(void)
	{
		return _HoverWidget;
	}
	
	inline float Widget::GetLeft(void)
	{
		return _Left.GetValue();
	}

	inline const std::string & Widget::GetName(void) const
	{
		return _Name;
	}

	inline std::list< Widget * > & Widget::GetDestroyedWidgets(void)
	{
		return _DestroyedWidgets;
	}
	
	inline float Widget::GetRight(void)
	{
		return _Left.GetValue() + _Width.GetValue();
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
	
	inline float Widget::GetTop(void)
	{
		return _Top.GetValue();
	}
	
	inline float Widget::GetWidth(void)
	{
		return _Width.GetValue();
	}
	
	inline bool Widget::IsEnabled(void) const
	{
		return _Enabled;
	}
	
	inline bool Widget::IsHovered(void) const
	{
		if(_SupWidget != nullptr)
		{
			return this == _SupWidget->_HoverWidget;
		}
		else
		{
			return false;
		}
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
