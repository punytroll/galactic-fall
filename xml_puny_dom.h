#ifndef XML_PUNY_DOM_H
#define XML_PUNY_DOM_H

#include <istream>
#include <vector>
#include <map>
#include <string>

class Document;

class Element
{
public:
	Element(Document * Document, Element * Parent, const std::string & Name, const std::map< std::string, std::string > & Attributes);
	const Element * GetParent(void) const;
	const std::string & GetName(void) const;
	const std::map< std::string, std::string > & GetAttributes(void) const;
	const std::string & GetAttribute(const std::string & AttributeName) const;
	bool HasAttribute(const std::string & AttributeName) const;
	const std::vector< Element * > & GetChilds(void) const;
	const Element * GetChild(std::vector< Element * >::size_type Index) const;
private:
	Document * m_Document;
	Element * m_Parent;
	std::string m_Name;
	std::map< std::string, std::string > m_Attributes;
	std::vector< Element * > m_Childs;
};

class Document
{
public:
	Document(std::istream & Stream);
	const Element * GetDocumentElement(void) const;
private:
	Element * m_DocumentElement;
};

inline const Element * Document::GetDocumentElement(void) const
{
	return m_DocumentElement;
}

#endif
