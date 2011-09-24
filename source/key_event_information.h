/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
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

#ifndef KEY_EVENT_INFORMATION_H
#define KEY_EVENT_INFORMATION_H

#include <X11/Xlib.h>

#include <string>

class KeyEventInformation
{
public:
	KeyEventInformation(XKeyEvent & KeyEvent);
	bool IsDown(void) const;
	bool IsUp(void) const;
	unsigned int GetKeyCode(void) const;
	const std::string & GetString(void) const;
	KeySym GetKeySymbol(void) const;
private:
	void PerformStringLookup(void) const;
	XKeyEvent & m_KeyEvent;
	mutable bool m_LookupStringPerformed;
	mutable std::string m_String;
	mutable KeySym m_KeySymbol;
};

inline bool KeyEventInformation::IsDown(void) const
{
	return m_KeyEvent.type == KeyPress;
}

inline bool KeyEventInformation::IsUp(void) const
{
	return m_KeyEvent.type == KeyRelease;
}

inline unsigned int KeyEventInformation::GetKeyCode(void) const
{
	return m_KeyEvent.keycode;
}

#endif
