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

#include "xml_stream.h"

XMLStream::XMLStream(std::ostream & OutputStream) :
	_OutputStream(OutputStream),
	_Errornous(false),
	_InElementName(false),
	_InElementHeader(false),
	_InAttribute(false),
	_InAttributeValue(false)
{
}

XMLStream::~XMLStream(void)
{
	if(_Errornous == false)
	{
		_EndStream();
	}
}

void XMLStream::_EndStream(void)
{
	_EndElementName();
	while(_ElementStack.size() > 0)
	{
		_EndElement();
	}
}

void XMLStream::_StartElement(void)
{
	if(_InElementHeader == true)
	{
		_EndElementHeader();
	}
	_InElementName = true;
	_InElementHeader = true;
	_OutputStream << '<';
}

void XMLStream::_StartAttribute(void)
{
	if(_InElementHeader == true)
	{
		_EndElementName();
		_EndAttribute();
		_OutputStream << ' ';
		_InAttribute = true;
	}
}

void XMLStream::_StartAttributeValue(void)
{
	if(_InAttribute == true)
	{
		_OutputStream << "=\"";
		_InAttributeValue = true;
	}
}

void XMLStream::_StartText(void)
{
	if(_InElementHeader == true)
	{
		_EndElementHeader();
	}
}

void XMLStream::_EndElement(void)
{
	_EndElementName();
	if(_InElementHeader == true)
	{
		_EndAttribute();
		_OutputStream << "/>";
		_InElementHeader = false;
	}
	else
	{
		_OutputStream << "</" << _ElementStack.top() << '>';
	}
	_ElementStack.pop();
}

void XMLStream::_EndElementHeader(void)
{
	_EndElementName();
	_EndAttribute();
	if(_InElementHeader == true)
	{
		_OutputStream << '>';
		_InElementHeader = false;
	}
}

void XMLStream::_EndElementName(void)
{
	if((_InElementName == true) && (_ElementName.str().length() > 0))
	{
		_ElementStack.push(_ElementName.str());
		_ElementName.str("");
		_InElementName = false;
	}
}

void XMLStream::_EndAttribute(void)
{
	_EndAttributeValue();
	if(_InAttribute == true)
	{
		_OutputStream << "=\"\"";
		_InAttribute = false;
	}
}

void XMLStream::_EndAttributeValue(void)
{
	if(_InAttributeValue == true)
	{
		_OutputStream << '"';
		_InAttributeValue = false;
		_InAttribute = false;
	}
}

XMLStream & XMLStream::operator<<(const std::string & Value)
{
	if(_InElementName == true)
	{
		_ElementName << Value;
	}
	_OutputStream << Value;
	
	return *this;
}

XMLStream & XMLStream::operator<<(const char * Value)
{
	if(_InElementName == true)
	{
		_ElementName << Value;
	}
	_OutputStream << Value;
	
	return *this;
}

XMLStream & XMLStream::operator<<(int Value)
{
	if(_InElementName == true)
	{
		_ElementName << Value;
	}
	_OutputStream << Value;
	
	return *this;
}

XMLStream & XMLStream::operator<<(unsigned int Value)
{
	if(_InElementName == true)
	{
		_ElementName << Value;
	}
	_OutputStream << Value;
	
	return *this;
}

XMLStream & XMLStream::operator<<(unsigned long Value)
{
	if(_InElementName == true)
	{
		_ElementName << Value;
	}
	_OutputStream << Value;
	
	return *this;
}

XMLStream & XMLStream::operator<<(float Value)
{
	if(_InElementName == true)
	{
		_ElementName << Value;
	}
	_OutputStream << Value;
	
	return *this;
}

XMLStream & XMLStream::operator<<(double Value)
{
	if(_InElementName == true)
	{
		_ElementName << Value;
	}
	_OutputStream << Value;
	
	return *this;
}

XMLStream & XMLStream::operator<<(bool Value)
{
	if(Value == true)
	{
		if(_InElementName == true)
		{
			_ElementName << "true";
		}
		_OutputStream << "true";
	}
	else
	{
		if(_InElementName == true)
		{
			_ElementName << "false";
		}
		_OutputStream << "false";
	}
	
	return *this;
}

XMLStream & XMLStream::operator<<(const void * Value)
{
	_OutputStream << Value;
	
	return *this;
}

XMLStream & XMLStream::operator<<(XMLStream & (* Function)(XMLStream &))
{
	return Function(*this);
}

XMLStream & element(XMLStream & XMLStream)
{
	XMLStream._StartElement();

	return XMLStream;
}

XMLStream & attribute(XMLStream & XMLStream)
{
	XMLStream._StartAttribute();

	return XMLStream;
}

XMLStream & value(XMLStream & XMLStream)
{
	XMLStream._StartAttributeValue();

	return XMLStream;
}

XMLStream & text(XMLStream & XMLStream)
{
	XMLStream._StartText();

	return XMLStream;
}

XMLStream & end(XMLStream & XMLStream)
{
	XMLStream._EndElement();

	return XMLStream;
}
