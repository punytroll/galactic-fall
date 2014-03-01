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

#include <assert.h>

#include <X11/Xutil.h>

#include "key_event.h"

UI::KeyEvent::KeyEvent(void) :
	_KeyEvent(nullptr),
	_LookupStringPerformed(false)
{
}

const std::string & UI::KeyEvent::GetString(void)
{
	if(_LookupStringPerformed == false)
	{
		_PerformStringLookup();
	}
	
	return _String;
}

KeySym UI::KeyEvent::GetKeySymbol(void)
{
	if(_LookupStringPerformed == false)
	{
		_PerformStringLookup();
	}
	
	return _KeySymbol;
}

void UI::KeyEvent::_PerformStringLookup(void)
{
	char String[10];
	
	XLookupString(_KeyEvent, String, 10, &_KeySymbol, NULL);
	_String = String;
	_LookupStringPerformed = true;
}

void UI::KeyEvent::SetKeyEvent(XKeyEvent * KeyEvent)
{
	_KeyEvent = KeyEvent;
	_LookupStringPerformed = false;
}
