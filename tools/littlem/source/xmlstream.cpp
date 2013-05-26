#include "xmlstream.h"

xmlstream::xmlstream(std::ostream & OutputStream) :
	m_OutputStream(OutputStream),
	m_bErrornous(false),
	m_bInElementName(false),
	m_bInElementHeader(false),
	m_bInAttribute(false),
	m_bInAttributeValue(false)
{
}

xmlstream::~xmlstream(void)
{
	if(m_bErrornous == false)
	{
		vEndStream();
	}
}

void xmlstream::vEndStream(void)
{
	vEndElementName();
	while(m_ElementStack.size() > 0)
	{
		vEndElement();
	}
	m_OutputStream << std::endl;
}

void xmlstream::vStartElement(void)
{
	if(m_bInElementHeader == true)
	{
		vEndElementHeader();
	}
	m_bInElementName = true;
	m_bInElementHeader = true;
	m_OutputStream << '<';
}

void xmlstream::vStartAttribute(void)
{
	if(m_bInElementHeader == true)
	{
		vEndElementName();
		vEndAttribute();
		m_OutputStream << ' ';
		m_bInAttribute = true;
	}
}

void xmlstream::vStartAttributeValue(void)
{
	if(m_bInAttribute == true)
	{
		m_OutputStream << "=\"";
		m_bInAttributeValue = true;
	}
}

void xmlstream::vStartText(void)
{
}

void xmlstream::vEndElement(void)
{
	vEndElementName();
	if(m_bInElementHeader == true)
	{
		vEndAttribute();
		m_OutputStream << "/>";
		m_bInElementHeader = false;
	}
	else
	{
		m_OutputStream << "</" << m_ElementStack.top() << '>';
	}
	m_ElementStack.pop();
}

xmlstream & xmlstream::operator<<(const std::string & Value)
{
	if(m_bInElementName == true)
	{
		m_ElementName << Value;
	}
	m_OutputStream << Value;
	
	return *this;
}

xmlstream & xmlstream::operator<<(const char * Value)
{
	if(m_bInElementName == true)
	{
		m_ElementName << Value;
	}
	m_OutputStream << Value;
	
	return *this;
}

xmlstream & xmlstream::operator<<(int Value)
{
	if(m_bInElementName == true)
	{
		m_ElementName << Value;
	}
	m_OutputStream << Value;
	
	return *this;
}

xmlstream & xmlstream::operator<<(unsigned long Value)
{
	if(m_bInElementName == true)
	{
		m_ElementName << Value;
	}
	m_OutputStream << Value;
	
	return *this;
}

xmlstream & xmlstream::operator<<(float Value)
{
	if(m_bInElementName == true)
	{
		m_ElementName << Value;
	}
	m_OutputStream << Value;
	
	return *this;
}

xmlstream & xmlstream::operator<<(bool Value)
{
	if(Value == true)
	{
		if(m_bInElementName == true)
		{
			m_ElementName << "true";
		}
		m_OutputStream << "true";
	}
	else
	{
		if(m_bInElementName == true)
		{
			m_ElementName << "false";
		}
		m_OutputStream << "false";
	}
	
	return *this;
}

xmlstream & xmlstream::operator<<(xmlstream & (* function)(xmlstream &))
{
	return function(*this);
}

void xmlstream::vEndElementHeader(void)
{
	vEndElementName();
	vEndAttribute();
	if(m_bInElementHeader == true)
	{
		m_OutputStream << '>';
		m_bInElementHeader = false;
	}
}

void xmlstream::vEndElementName(void)
{
	if((m_bInElementName == true) && (m_ElementName.str().length() > 0))
	{
		m_ElementStack.push(m_ElementName.str());
		m_ElementName.str("");
		m_bInElementName = false;
	}
}

void xmlstream::vEndAttribute(void)
{
	vEndAttributeValue();
	if(m_bInAttribute == true)
	{
		m_OutputStream << "=\"\"";
		m_bInAttribute = false;
	}
}

void xmlstream::vEndAttributeValue(void)
{
	if(m_bInAttributeValue == true)
	{
		m_OutputStream << '"';
		m_bInAttributeValue = false;
		m_bInAttribute = false;
	}
}

xmlstream & element(xmlstream & XMLStream)
{
	XMLStream.vStartElement();

	return XMLStream;
}

xmlstream & attribute(xmlstream & XMLStream)
{
	XMLStream.vStartAttribute();

	return XMLStream;
}

xmlstream & value(xmlstream & XMLStream)
{
	XMLStream.vStartAttributeValue();

	return XMLStream;
}

xmlstream & text(xmlstream & XMLStream)
{
	XMLStream.vStartText();

	return XMLStream;
}

xmlstream & end(xmlstream & XMLStream)
{
	XMLStream.vEndElement();

	return XMLStream;
}
