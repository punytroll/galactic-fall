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
 * This is version 1.2.2 of the xml stream.
 **/

#ifndef XML_STREAM_H
#define XML_STREAM_H

#include <ostream>
#include <stack>
#include <sstream>

class XMLStream
{
public:
	XMLStream(std::ostream & OutputStream);
	~XMLStream(void);
	void EndStream(void);
	void StartElement(void);
	void StartAttribute(void);
	void StartAttributeValue(void);
	void StartText(void);
	void EndElement(void);
	XMLStream & operator<<(const std::string & Value);
	XMLStream & operator<<(const char * Value);
	XMLStream & operator<<(int Value);
	XMLStream & operator<<(unsigned int Value);
	XMLStream & operator<<(unsigned long Value);
	XMLStream & operator<<(float Value);
	XMLStream & operator<<(bool Value);
	XMLStream & operator<<(const void * Value);
	XMLStream & operator<<(XMLStream & (* Function)(XMLStream &));
private:
	void EndElementHeader(void);
	void EndElementName(void);
	void EndAttribute(void);
	void EndAttributeValue(void);
	std::ostream & m_OutputStream;
	std::stack< std::string > m_ElementStack;
	std::stringstream m_ElementName;
	bool m_Errornous;
	bool m_InElementName;
	bool m_InElementHeader;
	bool m_InAttribute;
	bool m_InAttributeValue;
};

XMLStream & element(XMLStream & XMLStream);
XMLStream & attribute(XMLStream & XMLStream);
XMLStream & value(XMLStream & XMLStream);
XMLStream & text(XMLStream & XMLStream);
XMLStream & end(XMLStream & XMLStream);

#endif
