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

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#include <events/connection.h>

class Object;

class Message
{
public:
	Message(const std::string & TypeIdentifier, Object * Sender, Object * Receiver);
	virtual ~Message(void);
	// getters
	Object * GetReceiver(void);
	Object * GetSender(void);
	const std::string & GetTypeIdentifier(void) const;
private:
	// event callbacks
	void _OnReceiverDestroying(void);
	void _OnSenderDestroying(void);
	// members
	Object * _Receiver;
	Connection _ReceiverDestroyingConnection;
	Object * _Sender;
	Connection _SenderDestroyingConnection;
	std::string _TypeIdentifier;
};

inline Object * Message::GetReceiver(void)
{
	return _Receiver;
}

inline Object * Message::GetSender(void)
{
	return _Sender;
}

inline const std::string & Message::GetTypeIdentifier(void) const
{
	return _TypeIdentifier;
}

#endif
