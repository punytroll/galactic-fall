/**
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

/**
 * This is version 1.0.1 of the string cast.
 **/

#include <sstream>

#include "string_cast.h"

template < >
int from_string_cast< int >(const std::string & String)
{
	std::istringstream StringStream(String);
	int Result;
	
	StringStream >> Result;
	
	return Result;
}

template < >
unsigned int from_string_cast< unsigned int >(const std::string & String)
{
	std::istringstream StringStream(String);
	unsigned int Result;
	
	StringStream >> Result;
	
	return Result;
}

template < >
unsigned long from_string_cast< unsigned long >(const std::string & String)
{
	std::istringstream StringStream(String);
	unsigned long Result;
	
	StringStream >> Result;
	
	return Result;
}

template < >
float from_string_cast< float >(const std::string & String)
{
	std::istringstream StringStream(String);
	float Result;
	
	StringStream >> Result;
	
	return Result;
}

template < >
double from_string_cast< double >(const std::string & String)
{
	std::istringstream StringStream(String);
	double Result;
	
	StringStream >> Result;
	
	return Result;
}

template < >
bool from_string_cast< bool >(const std::string & String)
{
	return String == "true";
}

template < >
std::string to_string_cast< unsigned int >(const unsigned int & Value)
{
	std::ostringstream StringStream;
	
	StringStream << Value;
	
	return StringStream.str();
}

template < >
std::string to_string_cast< float >(const float & Value)
{
	std::ostringstream StringStream;
	
	StringStream << Value;
	
	return StringStream.str();
}

template < >
std::string to_string_cast< double >(const double & Value)
{
	std::ostringstream StringStream;
	
	StringStream << Value;
	
	return StringStream.str();
}

template < >
std::string to_string_cast< void * >(void * const & Value)
{
	std::ostringstream StringStream;
	
	StringStream << Value;
	
	return StringStream.str();
}
