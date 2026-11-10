/**
 * galactic-fall
 * Copyright (C) 2008-2018  Hagen Möbius
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

#include "message.h"
#include "object.h"

Message::Message(const std::string & TypeIdentifier, Object * Sender, Object * Receiver) :
	_Receiver(Receiver),
	_Sender(Sender),
	_TypeIdentifier(TypeIdentifier)
{
	assert(_Receiver != nullptr);
	m_ReceiverDestroyingConnection = _Receiver->ConnectDestroyingCallback(std::bind(&Message::_OnReceiverDestroying, this));
	assert(m_ReceiverDestroyingConnection.IsValid() == true);
	assert(_Sender != nullptr);
	m_SenderDestroyingConnection = _Sender->ConnectDestroyingCallback(std::bind(&Message::_OnSenderDestroying, this));
	assert(m_SenderDestroyingConnection.IsValid() == true);
}

Message::~Message(void)
{
	if(_Receiver != nullptr)
	{
		assert(m_ReceiverDestroyingConnection.IsValid() == true);
		m_ReceiverDestroyingConnection.Disconnect();
		_Receiver = nullptr;
	}
	assert(m_ReceiverDestroyingConnection.IsValid() == false);
	if(_Sender != nullptr)
	{
		assert(m_SenderDestroyingConnection.IsValid() == true);
		m_SenderDestroyingConnection.Disconnect();
		_Sender = nullptr;
	}
	assert(m_SenderDestroyingConnection.IsValid() == false);
}

void Message::_OnReceiverDestroying(void)
{
	assert(_Receiver != nullptr);
	_Receiver = nullptr;
	assert(m_ReceiverDestroyingConnection.IsValid() == true);
	m_ReceiverDestroyingConnection.Disconnect();
	assert(m_ReceiverDestroyingConnection.IsValid() == false);
}

void Message::_OnSenderDestroying(void)
{
	assert(_Sender != nullptr);
	_Sender = nullptr;
	assert(m_SenderDestroyingConnection.IsValid() == true);
	m_SenderDestroyingConnection.Disconnect();
	assert(m_SenderDestroyingConnection.IsValid() == false);
}
