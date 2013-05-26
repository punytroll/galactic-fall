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

#ifndef XML_STREAM_H
#define XML_STREAM_H

#include <ostream>
#include <stack>
#include <sstream>

class XMLStream
{
	friend XMLStream & attribute(XMLStream & XMLStream);
	friend XMLStream & element(XMLStream & XMLStream);
	friend XMLStream & end(XMLStream & XMLStream);
	friend XMLStream & text(XMLStream & XMLStream);
	friend XMLStream & value(XMLStream & XMLStream);
public:
	XMLStream(std::ostream & OutputStream);
	~XMLStream(void);
	XMLStream & operator<<(const std::string & Value);
	XMLStream & operator<<(const char * Value);
	XMLStream & operator<<(int Value);
	XMLStream & operator<<(unsigned int Value);
	XMLStream & operator<<(unsigned long Value);
	XMLStream & operator<<(float Value);
	XMLStream & operator<<(double Value);
	XMLStream & operator<<(bool Value);
	XMLStream & operator<<(const void * Value);
	XMLStream & operator<<(XMLStream & (* Function)(XMLStream &));
private:
	void _EndAttribute(void);
	void _EndAttributeValue(void);
	void _EndElement(void);
	void _EndElementHeader(void);
	void _EndElementName(void);
	void _EndStream(void);
	void _StartElement(void);
	void _StartAttribute(void);
	void _StartAttributeValue(void);
	void _StartText(void);
	std::ostream & _OutputStream;
	std::stack< std::string > _ElementStack;
	std::stringstream _ElementName;
	bool _Errornous;
	bool _InElementName;
	bool _InElementHeader;
	bool _InAttribute;
	bool _InAttributeValue;
};

XMLStream & element(XMLStream & XMLStream);
XMLStream & attribute(XMLStream & XMLStream);
XMLStream & value(XMLStream & XMLStream);
XMLStream & text(XMLStream & XMLStream);
XMLStream & end(XMLStream & XMLStream);

#endif
