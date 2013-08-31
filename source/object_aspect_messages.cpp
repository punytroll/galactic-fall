/**
 * galactic-fall
 * Copyright (C) 2008  Hagen Möbius
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

#include "object_aspect_messages.h"

ObjectAspectMessages::~ObjectAspectMessages(void)
{
	assert(_Messages.size() == 0);
}

Message * ObjectAspectMessages::PopMessage(void)
{
	if(_Messages.empty() == true)
	{
		return 0;
	}
	else
	{
		// this will pass responsibility to the caller
		Message * Message(_Messages.front());
		
		_Messages.pop_front();
		
		return Message;
	}
}

void ObjectAspectMessages::PushMessage(Message * Message)
{
	// taking over responsibility for Message
	_Messages.push_back(Message);
}
