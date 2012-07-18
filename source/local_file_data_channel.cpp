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

#include "local_file_data_channel.h"

LocalFileDataChannel::LocalFileDataChannel(const Arxx::URI & URI, const std::string & RelativePathPrefix) :
	Arxx::DataChannel(URI),
	m_RelativePathPrefix(RelativePathPrefix)
{
	if(URI.sGetScheme() != "file")
	{
		throw std::runtime_error("Created LocalFileDataChannel with scheme \"" + URI.sGetScheme() + "\" but \"file\" was expected.");
	}
}

bool LocalFileDataChannel::bFetchData(const Arxx::URI & URI, Arxx::Buffer & Buffer)
{
	if(URI.sGetScheme() != "file")
	{
		std::cerr << " *** LocalFileDataChannel::bFetchData called with scheme \"" << URI.sGetScheme() << "\" but \"file\" was expected." << std::endl;
		
		return false;
	}
	if((URI.sGetAuthority() != "") && (URI.sGetAuthority() != "localhost") && (URI.sGetAuthority() != "127.0.0.1"))
	{
		std::cerr << " *** LocalFileDataChannel::bFetchData called with authority \"" << URI.sGetAuthority() << "\" but \"\", \"localhost\" or \"127.0.0.1\" was expected." << std::endl;
		
		return false;
	}
	
	Arxx::URI BetterURI(URI);
	
	if(BetterURI.sGetPath()[0] != '/')
	{
		BetterURI.vSetPath(m_RelativePathPrefix + '/' + BetterURI.sGetPath());
	}
	
	std::ifstream IStream(BetterURI.sGetPath().c_str());
	
	if(!IStream)
	{
		std::cerr << " *** LocalFileDataChannel::bFetchData: Could not open file with URI \"" << BetterURI << "\"." << std::endl;
		
		return false;
	}
	Buffer.vSetLength(0);
	
	Arxx::BufferWriter BufferWriter(Buffer);
	
	BufferWriter << std::make_pair(static_cast< Arxx::Buffer::size_type >(0xFFFFFFFF), reinterpret_cast< std::istream * >(&IStream));
	IStream.close();
	
	return true;
}
