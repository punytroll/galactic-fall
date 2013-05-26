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
 * This is version 1.3.0 of the xml stream.
 **/

#include <assert.h>

#include <sstream>

#include "xml_stream.h"

int main(int argc, char ** argv)
{
	{
		std::stringstream StringStream;
		
		{
			XMLStream Stream(StringStream);
			
			Stream << element << "string";
		}
		assert(StringStream.str() == "<string/>");
	}
	{
		std::stringstream StringStream;
		
		{
			XMLStream Stream(StringStream);
			
			Stream << element << "string" << text << "TEXT";
		}
		assert(StringStream.str() == "<string>TEXT</string>");
	}
	{
		std::stringstream StringStream;
		
		{
			XMLStream Stream(StringStream);
			
			Stream << element << "double" << attribute << "value" << value << 1.2 << end;
		}
		assert(StringStream.str() == "<double value=\"1.2\"/>");
	}
	
	return 0;
}
