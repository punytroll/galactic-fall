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

#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <string>

#include <events/connection.h>

class KeyEventInformation;

namespace Graphics
{
	class RenderContext;
}

namespace UI
{
	class Event;
	class KeyEvent;
	class MouseButtonEvent;
	class MouseMoveEvent;
	class SubWidgetEvent;
	class Widget;
	
	class UserInterface
	{
	public:
		UserInterface(void);
		virtual ~UserInterface(void);
		void Draw(Graphics::RenderContext * RenderContext) const;
		void DispatchDestroyingEvent(UI::Event & DestroyingEvent);
		void DispatchKeyEvent(UI::KeyEvent & KeyEvent);
		void DispatchHeightChangedEvent(UI::Event & HeightChangedEvent);
		void DispatchLeftChangedEvent(UI::Event & LeftChangedEvent);
		void DispatchMouseButtonEvent(UI::MouseButtonEvent & MouseButtonEvent);
		void DispatchMouseEnterEvent(UI::Event & MouseEnterEvent);
		void DispatchMouseLeaveEvent(UI::Event & MouseLeaveEvent);
		void DispatchMouseMoveEvent(UI::MouseMoveEvent & MouseMoveEvent);
		void DispatchSubWidgetAddedEvent(UI::SubWidgetEvent & SubWidgetAddedEvent);
		void DispatchSubWidgetRemovedEvent(UI::SubWidgetEvent & SubWidgetRemovedEvent);
		void DispatchTopChangedEvent(UI::Event & TopChangedEvent);
		void DispatchWidthChangedEvent(UI::Event & WidthChangedEvent);
		void Update(float RealTimeSeconds, float GameTimeSeconds);
		// setters
		void SetCaptureWidget(UI::Widget * Widget);
		void ReleaseCaptureWidget(void);
		// getters
		UI::Widget * GetWidget(const std::string & Path);
		UI::Widget * GetRootWidget(void);
		UI::Widget * GetCaptureWidget(void);
	private:
		// callbacks
		void _OnCaptureWidgetDestroying(UI::Event & DestroyingEvent);
		void _OnHoverWidgetDestroying(UI::Event & DestroyingEvent);
		void _OnRootWidgetDestroying(UI::Event & DestroyingEvent);
		// member variables
		UI::Widget * _CaptureWidget;
		Connection _CaptureWidgetDestroyingCallbackConnection;
		UI::Widget * _HoverWidget;
		UI::Widget * _RootWidget;
	};

	inline UI::Widget * UserInterface::GetCaptureWidget(void)
	{
		return _CaptureWidget;
	}

	inline UI::Widget * UserInterface::GetRootWidget(void)
	{
		return _RootWidget;
	}
}

#endif
