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
	XMLStream & operator<<(unsigned long Value);
	XMLStream & operator<<(float Value);
	XMLStream & operator<<(bool Value);
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
