/**
 * galactic-fall
 * Copyright (C) 2014  Hagen Möbius
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

#ifndef UI_KEY_EVENT_H
#define UI_KEY_EVENT_H

#include <string>

#include <X11/Xlib.h>

#include "event.h"

namespace UI
{
	class KeyEvent : public UI::Event
	{
	public:
		KeyEvent(void);
		// getters
		bool IsDown(void);
		bool IsUp(void);
		unsigned int GetKeyCode(void);
		const std::string & GetString(void);
		KeySym GetKeySymbol(void);
		// setters
		void SetKeyEvent(XKeyEvent * KeyEvent);
	private:
		// helper functions
		void _PerformStringLookup(void);
		// member variables
		XKeyEvent * _KeyEvent;
		KeySym _KeySymbol;
		bool _LookupStringPerformed;
		std::string _String;
	};
}

inline bool UI::KeyEvent::IsDown(void)
{
	return _KeyEvent->type == KeyPress;
}

inline bool UI::KeyEvent::IsUp(void)
{
	return _KeyEvent->type == KeyRelease;
}

inline unsigned int UI::KeyEvent::GetKeyCode(void)
{
	return _KeyEvent->keycode;
}

#endif
