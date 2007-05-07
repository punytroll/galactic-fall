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
 * This is version 1.2.1 of the xml stream.
 **/

#include "xml_stream.h"

XMLStream::XMLStream(std::ostream & OutputStream) :
	m_OutputStream(OutputStream),
	m_Errornous(false),
	m_InElementName(false),
	m_InElementHeader(false),
	m_InAttribute(false),
	m_InAttributeValue(false)
{
}

XMLStream::~XMLStream(void)
{
	if(m_Errornous == false)
	{
		EndStream();
	}
}

void XMLStream::EndStream(void)
{
	EndElementName();
	while(m_ElementStack.size() > 0)
	{
		EndElement();
	}
}

void XMLStream::StartElement(void)
{
	if(m_InElementHeader == true)
	{
		EndElementHeader();
	}
	m_InElementName = true;
	m_InElementHeader = true;
	m_OutputStream << '<';
}

void XMLStream::StartAttribute(void)
{
	if(m_InElementHeader == true)
	{
		EndElementName();
		EndAttribute();
		m_OutputStream << ' ';
		m_InAttribute = true;
	}
}

void XMLStream::StartAttributeValue(void)
{
	if(m_InAttribute == true)
	{
		m_OutputStream << "=\"";
		m_InAttributeValue = true;
	}
}

void XMLStream::StartText(void)
{
	if(m_InElementHeader == true)
	{
		EndElementHeader();
	}
}

void XMLStream::EndElement(void)
{
	EndElementName();
	if(m_InElementHeader == true)
	{
		EndAttribute();
		m_OutputStream << "/>";
		m_InElementHeader = false;
	}
	else
	{
		m_OutputStream << "</" << m_ElementStack.top() << '>';
	}
	m_ElementStack.pop();
}

void XMLStream::EndElementHeader(void)
{
	EndElementName();
	EndAttribute();
	if(m_InElementHeader == true)
	{
		m_OutputStream << '>';
		m_InElementHeader = false;
	}
}

void XMLStream::EndElementName(void)
{
	if((m_InElementName == true) && (m_ElementName.str().length() > 0))
	{
		m_ElementStack.push(m_ElementName.str());
		m_ElementName.str("");
		m_InElementName = false;
	}
}

void XMLStream::EndAttribute(void)
{
	EndAttributeValue();
	if(m_InAttribute == true)
	{
		m_OutputStream << "=\"\"";
		m_InAttribute = false;
	}
}

void XMLStream::EndAttributeValue(void)
{
	if(m_InAttributeValue == true)
	{
		m_OutputStream << '"';
		m_InAttributeValue = false;
		m_InAttribute = false;
	}
}

XMLStream & XMLStream::operator<<(const std::string & Value)
{
	if(m_InElementName == true)
	{
		m_ElementName << Value;
	}
	m_OutputStream << Value;
	
	return *this;
}

XMLStream & XMLStream::operator<<(const char * Value)
{
	if(m_InElementName == true)
	{
		m_ElementName << Value;
	}
	m_OutputStream << Value;
	
	return *this;
}

XMLStream & XMLStream::operator<<(int Value)
{
	if(m_InElementName == true)
	{
		m_ElementName << Value;
	}
	m_OutputStream << Value;
	
	return *this;
}

XMLStream & XMLStream::operator<<(unsigned long Value)
{
	if(m_InElementName == true)
	{
		m_ElementName << Value;
	}
	m_OutputStream << Value;
	
	return *this;
}

XMLStream & XMLStream::operator<<(float Value)
{
	if(m_InElementName == true)
	{
		m_ElementName << Value;
	}
	m_OutputStream << Value;
	
	return *this;
}

XMLStream & XMLStream::operator<<(bool Value)
{
	if(Value == true)
	{
		if(m_InElementName == true)
		{
			m_ElementName << "true";
		}
		m_OutputStream << "true";
	}
	else
	{
		if(m_InElementName == true)
		{
			m_ElementName << "false";
		}
		m_OutputStream << "false";
	}
	
	return *this;
}

XMLStream & XMLStream::operator<<(const void * Value)
{
	m_OutputStream << Value;
	
	return *this;
}

XMLStream & XMLStream::operator<<(XMLStream & (* Function)(XMLStream &))
{
	return Function(*this);
}

XMLStream & element(XMLStream & XMLStream)
{
	XMLStream.StartElement();

	return XMLStream;
}

XMLStream & attribute(XMLStream & XMLStream)
{
	XMLStream.StartAttribute();

	return XMLStream;
}

XMLStream & value(XMLStream & XMLStream)
{
	XMLStream.StartAttributeValue();

	return XMLStream;
}

XMLStream & text(XMLStream & XMLStream)
{
	XMLStream.StartText();

	return XMLStream;
}

XMLStream & end(XMLStream & XMLStream)
{
	XMLStream.EndElement();

	return XMLStream;
}
