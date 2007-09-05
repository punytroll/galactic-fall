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

#ifndef LOCAL_FILE_DATA_CHANNEL_H
#define LOCAL_FILE_DATA_CHANNEL_H

#include <fstream>
#include <iostream>

#include <BufferWriter.h>
#include <DataChannel.h>

class LocalFileDataChannel : public Arxx::DataChannel
{
public:
	LocalFileDataChannel(const Arxx::URI & URI, const std::string & RelativePathPrefix);
	virtual bool bFetchData(const Arxx::URI & URI, Arxx::Buffer & Buffer, Arxx::FetchStatus & FetchStatus);
private:
	std::string m_RelativePathPrefix;
};

#endif
