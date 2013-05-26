#ifndef LITTLEM_XML_STREAM_H
#define LITTLEM_XML_STREAM_H

#include <ostream>
#include <stack>
#include <sstream>

class xmlstream
{
public:
	xmlstream(std::ostream & OutputStream);
	~xmlstream(void);
	void vEndStream(void);
	void vStartElement(void);
	void vStartAttribute(void);
	void vStartAttributeValue(void);
	void vStartText(void);
	void vEndElement(void);
	xmlstream & operator<<(const std::string & Value);
	xmlstream & operator<<(const char * Value);
	xmlstream & operator<<(int Value);
	xmlstream & operator<<(unsigned long Value);
	xmlstream & operator<<(float Value);
	xmlstream & operator<<(bool Value);
	xmlstream & operator<<(xmlstream & (* function)(xmlstream &));
private:
	void vEndElementHeader(void);
	void vEndElementName(void);
	void vEndAttribute(void);
	void vEndAttributeValue(void);
	std::ostream & m_OutputStream;
	std::stack< std::string > m_ElementStack;
	std::stringstream m_ElementName;
	bool m_bErrornous;
	bool m_bInElementName;
	bool m_bInElementHeader;
	bool m_bInAttribute;
	bool m_bInAttributeValue;
};

xmlstream & element(xmlstream & XMLStream);
xmlstream & attribute(xmlstream & XMLStream);
xmlstream & value(xmlstream & XMLStream);
xmlstream & text(xmlstream & XMLStream);
xmlstream & end(xmlstream & XMLStream);

#endif
