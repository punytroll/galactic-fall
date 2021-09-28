/**
 * galactic-fall
 * Copyright (C) 2008-2018  Aram Altschudjian, Hagen Möbius
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

#include <cassert>

#include "globals.h"
#include "message.h"
#include "message_dispatcher.h"
#include "object.h"
#include "object_aspect_messages.h"
#include "system_statistics.h"

MessageDispatcher::~MessageDispatcher(void)
{
	assert(_MessageQueue.empty() == true);
}

void MessageDispatcher::DispatchMessages(void)
{
	auto DispatchedMessages(0ul);
	
	while(_MessageQueue.empty() == false)
	{
		auto Receiver(_MessageQueue.front()->GetReceiver());
		
		if(Receiver != nullptr)
		{
			assert(Receiver->GetAspectMessages() != nullptr);
			Receiver->GetAspectMessages()->PushMessage(_MessageQueue.front());
			++DispatchedMessages;
		}
		else
		{
			delete _MessageQueue.front();
		}
		_MessageQueue.pop_front();
	}
	g_SystemStatistics->SetDispatchedMessagesThisFrame(DispatchedMessages);
}
