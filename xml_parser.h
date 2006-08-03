/**
 * Copyright (C) 2006  Hagen M�bius
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
 * This is version 1.0.0 of the xml parser.
 **/

#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <istream>
#include <map>

class XMLParser
{
public:
	XMLParser(std::istream & inputStream);
	virtual ~XMLParser(void);
	void parse(void);
	virtual void elementStart(const std::string & elementName, const std::map< std::string, std::string > & attributes);
	virtual void elementEnd(const std::string & elementName);
private:
	std::istream & inputStream;
};

#endif
