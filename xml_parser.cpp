/**
 * Copyright (C) 2006  Hagen M�ius
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
 * 
 * With modifications for escpae velocity.
 *  Renamed the xmlparser.h and xmlparser.cpp to xml_parser.h and xml_parser.cpp.
 **/

#include "xml_parser.h"

XMLParser::XMLParser(std::istream & inputStream) :
	inputStream(inputStream)
{
}

XMLParser::~XMLParser(void)
{
}

void XMLParser::parse(void)
{
	std::map< std::string, std::string > Attributes;
	std::string sElementName;
	std::string sAttributeName;
	std::string sBuffer;
	char cChar;
	bool bInAttributeValue(false);
	bool bInElementHeader(false);
	bool bInIdentifier(false);
	bool bIsEndingElement(false);
	bool bElementEnds(false);
	
	while(inputStream.get(cChar))
	{
		switch(cChar)
		{
		case '\n':
		case '\t':
		case ' ':
			{
				if((bInElementHeader == false) || (bInAttributeValue == true))
				{
					sBuffer += cChar;
				}
				else
				{
					if(bInIdentifier == true)
					{
						bInIdentifier = false;
						if(sElementName.length() == 0)
						{
							sElementName = sBuffer;
						}
						else
						{
							sAttributeName = sBuffer;
						}
						sBuffer = "";
					}
				}
				
				break;
			}
		case '=':
			{
				if(bInIdentifier == true)
				{
					sAttributeName = sBuffer;
					bInIdentifier = false;
					sBuffer = "";
				}
				
				break;
			}
		case '"':
			{
				if(bInAttributeValue == false)
				{
					bInAttributeValue = true;
				}
				else
				{
					
					bInAttributeValue = false;
					Attributes[sAttributeName] = sBuffer;
					sBuffer = "";
				}
				
				break;
			}
		case '<':
			{
				bInElementHeader = true;
				
				break;
			}
		case '>':
			{
				if(bInIdentifier == true)
				{
					sElementName = sBuffer;
					bInIdentifier = false;
					sBuffer = "";
				}
				if(bIsEndingElement == true)
				{
					elementEnd(sElementName);
					sElementName = "";
					bIsEndingElement = false;
				}
				else
				{
					elementStart(sElementName, Attributes);
					if(bElementEnds == true)
					{
						elementEnd(sElementName);
						bElementEnds = false;
					}
					sElementName = "";
					Attributes.clear();
				}
				
				break;
			}
		case '/':
			{
				if((bInElementHeader == true) && (bInAttributeValue == false))
				{
					if(bInIdentifier == true)
					{
						sElementName = sBuffer;
						sBuffer = "";
						bInIdentifier = false;
					}
					if(sElementName.length() == 0)
					{
						bIsEndingElement = true;
					}
					else
					{
						bElementEnds = true;
					}
				}
				else
				{
					sBuffer += '/';
				}
				
				break;
			}
		default:
			{
				if((bInElementHeader == true) && (bInAttributeValue == false))
				{
					bInIdentifier = true;
				}
				sBuffer += cChar;
				
				break;
			}
		}
	}
}

void XMLParser::elementStart(const std::string & elementName, const std::map< std::string, std::string > & attributes)
{
}

void XMLParser::elementEnd(const std::string & elementName)
{
}
