#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <istream>
#include <map>

class XMLParser
{
public:
	XMLParser(std::istream & InputStream);
	virtual ~XMLParser(void);
	void Parse(void);
	virtual void ElementStart(const std::string & Name, const std::map< std::string, std::string > & Attributes);
	virtual void ElementEnd(const std::string & Name);
private:
	std::istream & m_InputStream;
};

#endif
