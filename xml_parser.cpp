#include "xml_parser.h"

XMLParser::XMLParser(std::istream & InputStream) :
	m_InputStream(InputStream)
{
}

XMLParser::~XMLParser(void)
{
}

void XMLParser::Parse(void)
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
	
	while(m_InputStream.get(cChar))
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
					ElementEnd(sElementName);
					sElementName = "";
					bIsEndingElement = false;
				}
				else
				{
					ElementStart(sElementName, Attributes);
					if(bElementEnds == true)
					{
						ElementEnd(sElementName);
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

void XMLParser::ElementStart(const std::string & Name, const std::map< std::string, std::string > & attributes)
{
}

void XMLParser::ElementEnd(const std::string & Name)
{
}
