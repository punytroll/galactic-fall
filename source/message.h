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

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#include "referencing.h"

class Object;

class Message
{
public:
	Message(const std::string & TypeIdentifier, Reference< Object > Sender, Reference< Object > Receiver);
	virtual ~Message(void);
	// getters
	Reference< Object > & GetReceiver(void);
	Reference< Object > & GetSender(void);
	const std::string & GetTypeIdentifier(void) const;
private:
	Reference< Object > _Receiver;
	Reference< Object > _Sender;
	std::string _TypeIdentifier;
};

inline Reference< Object > & Message::GetReceiver(void)
{
	return _Receiver;
}

inline Reference< Object > & Message::GetSender(void)
{
	return _Sender;
}

inline const std::string & Message::GetTypeIdentifier(void) const
{
	return _TypeIdentifier;
}

#endif
